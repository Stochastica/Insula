#ifndef INSULA_SCENE_MANDELBROT_HPP_
#define INSULA_SCENE_MANDELBROT_HPP_

#include "Scene.hpp"
#include "../core/Gradient.hpp"

namespace ins
{

class Mandelbrot final: public Scene
{
public:
	Mandelbrot(Gradient* gradient, int imgWidth, int imgHeight,
	           complex min, complex max,
	           int iterations, real escapeRadius):
	  gradient(gradient),
	  imgWidth(imgWidth), imgHeight(imgHeight),
	  min(min), max(max),
	  iterations(iterations), cycles(gradient->getLength()),
	  escapeRadius(escapeRadius)
	{
	}

	Color4 sample(complex z) const override;

private:
	Gradient* gradient;
	int imgWidth, imgHeight;
	complex min, max;
	int iterations;
	int cycles;
	real escapeRadius;
};

} // namespace ins

#endif // !INSULA_SCENE_MANDELBROT_HPP_
