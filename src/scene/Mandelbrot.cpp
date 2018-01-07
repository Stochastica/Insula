#include "Mandelbrot.hpp"

namespace ins
{

Color4 Mandelbrot::sample(complex z0) const
{
	// Transform z0 into the region
	complex c(
		z0.real() * (max.real() - min.real()) + min.real(),
		z0.imag() * (max.imag() - min.imag()) + min.imag()
	);
	// Escape Time algorithm (Fractional)
	int i = 0; // NIterations

	complex z(0, 0);
	while (std::norm(z) < escapeRadius && i < iterations)
	{
		z = z * z + c;
		++i;
	}

	Color4 color;
	if (i == 0)
	{
		color = gradient->at(0, 0);
	}
	else if (i < iterations)
	{
		/*
		 * Use normalised iterations count to calculate fractional part of
		 * iteration
		 */
		real fractional = 1.5 - std::log2(std::log2(std::norm(z)));
		color = gradient->at(i, fractional);
	}
	else
	{
		color = gradient->at(iterations - 1, 0);
	}
	return color;
}

} // namespace ins
