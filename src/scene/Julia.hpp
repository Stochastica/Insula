#ifndef INSULA_SCENE_JULIA_HPP_
#define INSULA_SCENE_JULIA_HPP_

#include "Scene.hpp"
#include "../core/Gradient.hpp"

namespace ins
{

class Julia final: public Scene
{
public:
	Julia(Gradient* gradient, int imgWidth, int imgHeight,
	           complex min, complex max, complex c,
	           int iterations, real escapeRadius):
	  gradient(gradient),
	  imgWidth(imgWidth), imgHeight(imgHeight),
	  min(min), max(max), c(c),
	  iterations(iterations), cycles(gradient->getLength()),
	  escapeRadius(escapeRadius)
	{
	}

	Color4 sample(complex z) const override;

private:
	Gradient* gradient;
	int imgWidth, imgHeight;
	complex min, max;
	complex c;
	int iterations;
	int cycles;
	real escapeRadius;
};
} // namespace ins

#endif // !INSULA_SCENE_JULIA_HPP_
