#include "sampling.hpp"

#include <cassert>
#include <iostream>

namespace ins
{

Color4 SamplerSimple::at(Vector2i const& v) const
{
	complex z(
	  v.x() / (real) width,
	  v.y() / (real) height
	);
	return scene->sample(z);
}

std::uniform_real_distribution<real> SamplerSuper::randDist(-.5, .5);

SamplerSuper::SamplerSuper(int width, int height, std::shared_ptr<Scene> scene,
                           int spp, unsigned long seed):
	Sampler(width, height, scene), spp(spp), invSpp(1 / (real) spp),
	samplesX(new real[spp]), samplesY(new real[spp])
{
	assert(spp > 0);
	std::mt19937 engine(seed);
	for (int i = 0; i < spp; ++i)
	{
		samplesX[i] = randDist(engine);
		samplesY[i] = randDist(engine);
	}
}
SamplerSuper::~SamplerSuper()
{
	delete[] samplesX;
	delete[] samplesY;
}
Color4 SamplerSuper::at(Vector2i const& v) const
{
	Color4 sample(0, 0, 0, 0);
	for (int i = 0; i < spp; ++i)
	{
		complex z(
		  (v.x() + samplesX[i]) / (real) width,
		  (v.y() + samplesY[i]) / (real) height
		);
		sample += scene->sample(z);
	}
	sample *= invSpp;
	return sample;
}

} // namespace ins
