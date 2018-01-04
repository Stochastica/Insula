#include "fractals.hpp"

namespace ins
{

Color4 Mandelbrot::pixel(Vector2i p) const
{
	// Convert pixel to complex numbers
	real x = p.x() / ((real) imgWidth) * (max.real() - min.real()) + min.real();
	real y = p.y() / ((real) imgHeight) * (max.imag() - min.imag()) + min.imag();

	real val = at(complex(x, y));
	return Color4(val, val, val, 1);
}
real Mandelbrot::at(complex c) const
{
	static constexpr real const EscapeRadius = (real)(1 << 30);
	int i = 0;

	complex z(0, 0);
	while (std::norm(z) < EscapeRadius && i < iterations)
	{
		z = z * z + c;
		++i;
	}
	if (i == 0)
	{
		return 0;
	}
	else if (i < iterations)
	{
		/*
		 * Use normalised iterations count to calculate fractional part of
		 * iteration
		 */
		real logAbs = std::log(std::norm(z)) / 2;
		real fractional = -std::log2(logAbs / std::log(2));
		i = i % 16;
		return (i - fractional) / 16;
	}
	else
	{
		return 1;
	}
}

} // namespace ins
