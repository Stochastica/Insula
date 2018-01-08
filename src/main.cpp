#include <iostream>

#include <boost/filesystem.hpp>
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
#include "core/io.hpp"
#include "scene/Julia.hpp"
#include "scene/Mandelbrot.hpp"
#include "scene/SceneTest.hpp"

#define EXIT_NORMAL 0
#define EXIT_FAILURE 1

namespace ins
{


bool processFile(
  boost::filesystem::path output,
  boost::property_tree::ptree const& ptree,
  TaskParameters const& pp
)
{
	namespace bpt = boost::property_tree;

	bool flag = false;

	std::string type; // Scene type
	Image image;
	// Load image
	{
		image.width = 800;
		image.height = 600;
		if (auto dimensions = ptree_getArray_optional<int>(ptree, "dimensions"))
		{
			image.width = dimensions.get()[0];
			image.height = dimensions.get()[1];
		}
		type = ptree.get<std::string>("type", type);
		image.pixelsR = new uint8_t[image.width * image.height];
		image.pixelsG = new uint8_t[image.width * image.height];
		image.pixelsB = new uint8_t[image.width * image.height];
		image.pixelsA = new uint8_t[image.width * image.height];
	}

	std::shared_ptr<Scene> scene;
	std::shared_ptr<Sampler> sampler;

	typedef boost::gil::rgba8_image_t::value_type Pixel;
	static_assert(sizeof(Pixel) == sizeof(uint32_t), "Must be of RGBA8 format");

	Gradient gradient = readGradient(ptree, "gradient");

	// Load Scene
	if (type == "Test")
	{
		scene = std::shared_ptr<Scene>(new SceneTest(&gradient));
	}
	else if (type == "Mandelbrot")
	{
		real centreX = -.7;
		real centreY = 0;
		real radius = 1.5;
		int iterations = 64;
		int cycles = 16;
		real escapeRadius = 20;

		if (auto tree = ptree.get_child_optional("Mandelbrot"))
		{
			if (auto centre = ptree_getArray_optional<real>(tree.get(), "centre"))
			{
				centreX = centre.get()[0];
				centreY = centre.get()[1];
			}
			radius = tree->get<real>("radius", radius);
			iterations = tree->get<int>("iterations", iterations);
			escapeRadius = tree->get<real>("escapeRadius", escapeRadius);
		}
		real diffY = radius * (image.height / (real) image.width);
		scene = std::shared_ptr<Scene>(new Mandelbrot(&gradient, image.width, image.height,
		                               complex(centreX - radius, centreY - diffY),
		                               complex(centreX + radius, centreY + diffY),
		                               iterations, escapeRadius));
	}
	else if (type == "Julia")
	{
		real centreX = 0;
		real centreY = 0;
		real radius = 1.5;
		int iterations = 64;
		int cycles = 16;
		real escapeRadius = 20;
		complex c(-0.8,0.156);

		if (auto tree = ptree.get_child_optional("Julia"))
		{
			if (auto centre = ptree_getArray_optional<real>(tree.get(), "centre"))
			{
				centreX = centre.get()[0];
				centreY = centre.get()[1];
			}
			radius = tree->get<real>("radius", radius);
			iterations = tree->get<int>("iterations", iterations);
			escapeRadius = tree->get<real>("escapeRadius", escapeRadius);
			if (auto tC = ptree_getArray_optional<real>(tree.get(), "c"))
			{
				c = complex(tC.get()[0], tC.get()[1]);
			}
		}
		real diffY = radius * (image.height / (real) image.width);
		scene = std::shared_ptr<Scene>(new Julia(&gradient, image.width, image.height,
		                               complex(centreX - radius, centreY - diffY),
		                               complex(centreX + radius, centreY + diffY),
		                               c,
		                               iterations, escapeRadius));
	}
	else
	{
		std::cerr << "Fatal: Unknown Scene type: " << type << std::endl;
		goto exit;
	}

	// Load sampler
	if (auto tSampler = ptree.get_child_optional("sampler"))
	{
		std::string type = tSampler->get<std::string>("type");
		if (type == "Simple")
		{
			sampler = std::shared_ptr<Sampler>(new SamplerSimple(image.width, image.height, scene));
		}
		else if (type == "Super")
		{
			sampler = std::shared_ptr<Sampler>(new SamplerSuper(image.width, image.height, scene,
			                                   tSampler->get<int>("spp", 8),
			                                   tSampler->get<unsigned long>("seed", 0)
			                                                   ));
		}
		else
		{
			std::cerr << "Fatal: Unknown Sampler type: " << type << std::endl;
			goto exit;
		}
	}
	else
	{
		sampler = std::shared_ptr<Sampler>(new SamplerSuper(image.width, image.height, scene, 8, 0));
	}
	render(&image, pp, sampler.get());
	boost::gil::png_write_view(output.string(),
	                           boost::gil::planar_rgba_view(image.width,
	                               image.height,
	                               image.pixelsR,
	                               image.pixelsG,
	                               image.pixelsB,
	                               image.pixelsA,
	                               image.width)
	                          );

exit:
	delete[] image.pixelsR;
	delete[] image.pixelsG;
	delete[] image.pixelsB;
	delete[] image.pixelsA;

	return flag;
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
		("output,o", bpo::value<std::string>()->default_value(""), "Output file")
		("input,i", bpo::value<std::string>(), "Input File")
		("threads,t", bpo::value<int>()->default_value(0, "Automatic"), "Number of Threads")
		("bucketsize,b", bpo::value<int>()->default_value(32), "Size of Rendering Chunk")
		;
		bpo::store(bpo::parse_command_line(argc, argv, desc), options);

		/*
		 * Depend on command line arguments, execute different routines.
		 */
		if (options.count("help"))
		{
			std::cout << INSULA_NAME << " " << INSULA_VERSION << std::endl
			          << desc << std::endl;
			return EXIT_NORMAL;
		}
	}

	namespace bfs = boost::filesystem;

	auto input = boost::filesystem::path(options["input"].as<std::string>());
	auto outputFile = options["output"].as<std::string>();
	bfs::path output;
	if (outputFile == "")
		output = bfs::path(input.stem().string() + ".png");
	else
		output = bfs::path(outputFile);

	TaskParameters pp;
	pp.nThreads = options["threads"].as<int>();
	pp.bucketSize = options["bucketsize"].as<int>();

	boost::property_tree::ptree ptree;
	boost::property_tree::read_json(input.c_str(), ptree);

	if (processFile(output, ptree, pp))
		return EXIT_NORMAL;
	else
		return EXIT_FAILURE;
}
