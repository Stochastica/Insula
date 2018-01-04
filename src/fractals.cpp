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
	int i = 0;

	complex z(0, 0);
	while (std::norm(z) < escapeRadius && i < iterations)
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
		real fractional = 1.5 - std::log2(std::log2(std::norm(z)));
		return std::fmod(i + fractional, cycles) / cycles;
	}
	else
	{
		return 1;
	}
}

} // namespace ins
