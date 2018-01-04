#ifndef INSULA__FRACTALS_HPP_
#define INSULA__FRACTALS_HPP_

#include <cstddef>

#include "core/insula.hpp"
#include "util/vector.hpp"

namespace ins
{

class Gradient
{
public:
	Gradient();
	Gradient(Gradient const&) = delete;
	Gradient(Gradient&&) = delete;

	Color4& at(int i) noexcept { return colors[i]; }
	Color4 at(int i) const noexcept { return colors[i]; }
private:
	Color4* colors;
	int size;
};

class PixelProvider
{
public:
	~PixelProvider() = default;

	virtual Color4 pixel(Vector2i) const = 0;
};

class Mandelbrot final: public PixelProvider
{
public:
	Mandelbrot(int imgWidth, int imgHeight,
	           complex min, complex max, int iterations):
	  imgWidth(imgWidth), imgHeight(imgHeight),
	  min(min), max(max),
	  iterations(iterations)
	{
	}

	Color4 pixel(Vector2i) const override;

private:
	real at(complex c) const;

	int imgWidth, imgHeight;
	complex min, max;
	int iterations;
};

} // namespace ins

#endif // !INSULA__FRACTALS_HPP_
