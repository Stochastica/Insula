#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>

/**
 * Defined to make PNG io work
 */
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*) NULL

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include "integration.hpp"
#include "scene/Mandelbrot.hpp"

#define EXIT_NORMAL 0
#define EXIT_FAILURE 1

namespace ins
{

template <typename T>
std::vector<T> ptree_getArray
(
  boost::property_tree::ptree const& tree,
  boost::property_tree::ptree::key_type const& key
)
{
	std::vector<T> result;
	for (auto& item: tree.get_child(key))
		result.push_back(item.second.get_value<T>());
	return result;
}
/**
 * @brief Reads a json array from a property tree
 */
template <typename T>
boost::optional<std::vector<T>> ptree_getArray_optional
                             (
                               boost::property_tree::ptree const& tree,
                               boost::property_tree::ptree::key_type const& key
                             )
{
	if (auto child = tree.get_child_optional(key))
	{
		std::vector<T> result;
		for (auto& item: child.get())
			result.push_back(item.second.get_value<T>());
		return result;
	}
	else
	{
		return boost::optional<std::vector<T>>();
	}
}

bool processFile(
  boost::filesystem::path outputDir,
  boost::filesystem::path file,
  TaskParameters const& pp
)
{
	namespace bpt = boost::property_tree;
	bpt::ptree tree;
	bpt::read_json(file.c_str(), tree);

	int i = 0; // Scene id

	for (auto const& scene: tree)
	{
		if (scene.first != "scene")
		{
			std::cerr << "Warning: Unknown class " << scene.first << std::endl;
			continue;
		}

		std::string fileName;
		std::string type; // Scene type
		Image image;
		// Load image
		{
			fileName = "scene" + std::to_string(i) + ".png";
			image.width = 800;
			image.height = 600;
			if (auto parameters = scene.second.get_child_optional("parameters"))
			{
				fileName = parameters->get<std::string>("filename", fileName);
				type = parameters->get<std::string>("type", type);
				if (auto dimensions = ptree_getArray_optional<int>(scene.second, "dimensions"))
				{
					image.width = dimensions.get()[0];
					image.height = dimensions.get()[1];
				}
				image.width = parameters->get<int>("imgWidth", image.width);
				image.height = parameters->get<int>("imgHeight", image.height);
			}
			image.pixelsR = new uint8_t[image.width * image.height];
			image.pixelsG = new uint8_t[image.width * image.height];
			image.pixelsB = new uint8_t[image.width * image.height];
			image.pixelsA = new uint8_t[image.width * image.height];
		}
		std::cout << "Scene name: " << fileName
		          << ", Type: " << type << std::endl;

		// Read Gradient

		typedef boost::gil::rgba8_image_t::value_type Pixel;
		static_assert(sizeof(Pixel) == sizeof(uint32_t), "Must be of RGBA8 format");

		if (type == "Mandelbrot")
		{
			real centreX = -.7;
			real centreY = 0;
			real radius = 1.5;
			int iterations = 64;
			int cycles = 16;
			real escapeRadius = 20;

			if (auto tree = scene.second.get_child_optional("Mandelbrot"))
			{
				if (auto centre = ptree_getArray_optional<real>(tree.get(), "centre"))
				{
					centreX = centre.get()[0];
					centreY = centre.get()[1];
				}
				radius = tree->get<real>("radius", radius);
				iterations = tree->get<int>("iterations", iterations);
				cycles = tree->get<int>("cycles", cycles);
				escapeRadius = tree->get<real>("escapeRadius", escapeRadius);
			}
			real diffY = radius * (image.height / (real) image.width);
			Mandelbrot m = Mandelbrot(image.width, image.height,
			                          complex(centreX - radius, centreY - diffY),
			                          complex(centreX + radius, centreY + diffY),
			                          iterations, cycles, escapeRadius);
			Sampler s = Sampler(image.width, image.height, &m);
			render(&image, pp, &s);
		}
		else
		{
			std::cerr << "Unknown Scene type\n";
			continue;
		}

		boost::gil::png_write_view((outputDir / fileName).string(),
		                           boost::gil::planar_rgba_view(image.width,
		                               image.height,
		                               image.pixelsR,
		                               image.pixelsG,
		                               image.pixelsB,
		                               image.pixelsA,
		                               image.width)
		                          );

		delete[] image.pixelsR;
		delete[] image.pixelsG;
		delete[] image.pixelsB;
		delete[] image.pixelsA;
		++i;
	}

	return true;
}

}

int main(int argc, char* argv[])
{
	using namespace ins;

	boost::program_options::variables_map options;
	{
		namespace bpo = boost::program_options;
		std::vector<int> dimensionsDefault;
		dimensionsDefault.push_back(800);
		dimensionsDefault.push_back(600);

		/*
		 * Interpret command line arguemnts with boost::program_options
		 */
		bpo::options_description desc("Options");
		desc.add_options()
		("help,h", "Show help")
		("output,o", bpo::value<std::string>()->default_value(boost::filesystem::current_path().string(), "Current Directory"), "Output directory")
		("input,i", bpo::value<std::string>(), "Input File")
		("threads,t", bpo::value<int>()->default_value(0), "Number of Threads")
		("bucketsize,b", bpo::value<int>()->default_value(16), "Size of Rendering Chunk")
		;
		bpo::store(bpo::parse_command_line(argc, argv, desc), options);

		/*
		 * Depend on command line arguments, execute different routines.
		 */
		if (options.count("help"))
		{
			std::cout << desc << std::endl;
			return EXIT_NORMAL;
		}
	}

	auto output = boost::filesystem::path(options["output"].as<std::string>());

	if (!boost::filesystem::exists(output))
	{
		std::cerr << "Output Directory does not exist. Halt/\n";
		return EXIT_FAILURE;
	}
	auto input = boost::filesystem::path(options["input"].as<std::string>());

	TaskParameters pp;
	pp.nThreads = options["threads"].as<int>();
	pp.bucketSize = options["bucketsize"].as<int>();

	if (processFile(output, input, pp))
		return EXIT_NORMAL;
	else
		return EXIT_FAILURE;
}
