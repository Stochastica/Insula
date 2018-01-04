#ifndef INSULA_SCENE_MANDELBROT_HPP_
#define INSULA_SCENE_MANDELBROT_HPP_

#include "Scene.hpp"

namespace ins
{

class Mandelbrot final: public Scene
{
public:
	Mandelbrot(int imgWidth, int imgHeight,
	           complex min, complex max,
	           int iterations, int cycles, real escapeRadius):
	  imgWidth(imgWidth), imgHeight(imgHeight),
	  min(min), max(max),
	  iterations(iterations), cycles(cycles),
	  escapeRadius(escapeRadius)
	{
	}

	Color4 sample(complex z) const override;

private:
	int imgWidth, imgHeight;
	complex min, max;
	int iterations;
	int cycles;
	real escapeRadius;
};

} // namespace ins

#endif // !INSULA_SCENE_MANDELBROT_HPP_
