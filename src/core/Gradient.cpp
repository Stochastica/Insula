#include "Gradient.hpp"

#include <boost/math/interpolators/barycentric_rational.hpp>

#include "io.hpp"

namespace ins
{

Gradient::Gradient(int nOut, int nIn,
                   real const* const control,
                   real const* const colorR,
                   real const* const colorG,
                   real const* const colorB,
                   real const* const colorA):
	n(nOut), colors(new Color4[nOut])
{
	assert(nOut > 0);
	assert(nIn > 0);

	typedef boost::math::barycentric_rational<real> bri;
	bri interpolateR(control, colorR, nIn);
	bri interpolateG(control, colorG, nIn);
	bri interpolateB(control, colorB, nIn);
	bri interpolateA(control, colorA, nIn);

	real ratio = 1 / (real) nOut;

	for (int i = 0; i < nOut; ++i)
	{
		real x = i * ratio;
		colors[i](0) = interpolateR(x);
		colors[i](1) = interpolateG(x);
		colors[i](2) = interpolateB(x);
		colors[i](3) = interpolateA(x);
	}
}
Gradient::~Gradient()
{
	delete[] colors;
}

Gradient readGradient(
  boost::property_tree::ptree const& tree,
  boost::property_tree::ptree::key_type const& key
)
{
	if (auto child = tree.get_child_optional(key))
	{
		int nOut = child->get<int>("nOut", 1024);
		std::vector<real> controls, colorR, colorG, colorB, colorA;
		std::vector<real> rawIn;

		rawIn = ptree_getArray<real>(child.get(), "colors");
		int nIn = rawIn.size() / 5;
		controls.resize(nIn);
		colorR.resize(nIn);
		colorG.resize(nIn);
		colorB.resize(nIn);
		colorA.resize(nIn);

		for (int i = 0; i < nIn; ++i)
		{
			controls[i] = rawIn[i * 5];
			colorR[i] = rawIn[i * 5 + 1];
			colorG[i] = rawIn[i * 5 + 2];
			colorB[i] = rawIn[i * 5 + 3];
			colorA[i] = rawIn[i * 5 + 4];
		}
		/*
		auto tColors = child->get_child("colors");
		auto it = child.get().begin();
		while (it != child.get().end())
		{
			colorR.push_back(it->second.get_value<real>());
			++it;
			controls.push_back(it->second.get_value<real>());
			++it;
			colorG.push_back(it->second.get_value<real>());
			++it;
			colorB.push_back(it->second.get_value<real>());
			++it;
			colorA.push_back(it->second.get_value<real>());
			++it;
		}
		*/
		return Gradient(nOut, controls.size(),
		                controls.data(),
		                colorR.data(),
		                colorG.data(),
		                colorB.data(),
		                colorA.data());
	}
	else
	{
		int const nOut = 64;
		int const nIn = 7;
		real control[nIn];
		real colorR[nIn];
		real colorG[nIn];
		real colorB[nIn];
		real colorA[nIn];
#define SET_POINT(index, c, cR, cG, cB) \
		control[index] = c; \
		colorR[index] = cR; \
		colorG[index] = cG; \
		colorB[index] = cB; \
		colorA[index] = 1

		// This data is from https://en.wikipedia.org/wiki/File:Mandel_zoom_00_mandelbrot_set.jpg
		SET_POINT(0,    0, 0, 0, 0);
		SET_POINT(1, .070, .391, .027,    0);
		SET_POINT(2, .230, .793, .418, .125);
		SET_POINT(3, .490,    1,    1, .926);
		SET_POINT(4, .713,    0, .664,    1);
		SET_POINT(5, .928, .188, .008, .191);
		SET_POINT(6,    1, 0, 0, 0);
#undef SET_POINT
		return Gradient(nOut, nIn, control,
		                colorR, colorG, colorB, colorA);
	}
}

} // namespace ins
