#ifndef INSULA__SAMPLING_HPP_
#define INSULA__SAMPLING_HPP_

#include <memory>
#include <random>

#include "util/vector.hpp"
#include "scene/Scene.hpp"

namespace ins
{

class Sampler
{
public:
	Sampler(int width, int height, std::shared_ptr<Scene> scene):
		width(width), height(height),
		invWidth(1 / (real) width), invHeight(1 / (real) height),
		scene(scene)
	{}
	virtual ~Sampler() = default;

	/**
	 * @brief Obtain the color at the given pixel coordinates
	 */
	virtual Color4 at(Vector2i const&) const = 0;

protected:
	int width, height;
	real invWidth, invHeight;
	std::shared_ptr<Scene> scene;
};

/**
 * @brief Renders with only one sample/pixel.
 */
class SamplerSimple final: public Sampler
{
public:
	SamplerSimple(int width, int height, std::shared_ptr<Scene> scene):
		Sampler(width, height, scene)
	{
	}

	Color4 at(Vector2i const&) const override;
};

class SamplerSuper final: public Sampler
{
public:
	/**
	 * @param[in] spp Sample/Pixel
	 */
	SamplerSuper(int width, int height, std::shared_ptr<Scene> scene,
	             int spp, unsigned long seed);
	~SamplerSuper();

	Color4 at(Vector2i const&) const override;
private:
	static std::uniform_real_distribution<real> randDist;

	int spp;
	real invSpp;
	real* samplesX;
	real* samplesY;
};

} // namespace ins

#endif // !INSULA__SAMPLING_HPP_
