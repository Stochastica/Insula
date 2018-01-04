#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/program_options.hpp>

/**
 * Defined to make PNG io work
 */
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*) NULL

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include "fractals.hpp"
#include "integration.hpp"

#define EXIT_NORMAL 0
#define EXIT_FAILURE 1

namespace ins
{

bool processFile(
  boost::filesystem::path outputDir,
  boost::filesystem::path file,
  TaskParameters const& pp
)
{
	namespace bpt = boost::property_tree;
	bpt::ptree tree;
	bpt::read_xml(file.c_str(), tree);

	int i = 0;

	for (auto const& scene: tree)
	{
		if (scene.first != "scene")
		{
			std::cerr << "Warning: Unknown class " << scene.first << std::endl;
			continue;
		}

		std::string fileName;
		Image image;
		// Load image
		{
			auto parameters = scene.second.get_child_optional("parameters");
			fileName = "scene" + std::to_string(i) + ".png";
			image.width = 800;
			image.height = 600;
			if (parameters)
			{
				fileName = scene.second.get<std::string>("filename", fileName);
				image.width = scene.second.get<int>("imgWidth", image.width);
				image.height = scene.second.get<int>("imgHeight", image.height);
			}
			image.pixelsR = new uint8_t[image.width * image.height];
			image.pixelsG = new uint8_t[image.width * image.height];
			image.pixelsB = new uint8_t[image.width * image.height];
			image.pixelsA = new uint8_t[image.width * image.height];
		}
		std::cout << "Scene name: " << fileName << std::endl;

		typedef boost::gil::rgba8_image_t::value_type Pixel;
		static_assert(sizeof(Pixel) == sizeof(uint32_t), "Must be of RGBA8 format");

		for (auto const& object: scene.second)
		{
			if (object.first == "Mandelbrot")
			{
				real centreX = object.second.get<real>("centreX");
				real centreY = object.second.get<real>("centreY");
				real diffX = object.second.get<real>("diffX");
				real diffY = object.second.get<real>("diffY");
				int iterations = object.second.get<int>("iterations", 1024);

				Mandelbrot m = Mandelbrot(image.width, image.height,
				                          complex(centreX - diffX, centreY - diffY),
				                          complex(centreX + diffX, centreY + diffY),
				                          iterations);
				render(&image, pp, &m);

				goto write;
			}
			else if (object.first != "parameters")
			{
				std::cerr << "Warning: Unknown object " << object.first << std::endl;
			}
		}

write:
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
	auto input = boost::filesystem::path(options["input"].as<std::string>());

	TaskParameters pp;
	pp.nThreads = options["threads"].as<int>();
	pp.bucketSize = options["bucketsize"].as<int>();

	if (processFile(output, input, pp))
		return EXIT_NORMAL;
	else
		return EXIT_FAILURE;
}
