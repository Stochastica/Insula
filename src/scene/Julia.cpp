#include "Julia.hpp"

namespace ins
{

Color4 Julia::sample(complex in) const
{
	// Transform z0 into the region
	complex z(
		in.real() * (max.real() - min.real()) + min.real(),
		in.imag() * (max.imag() - min.imag()) + min.imag()
	);
	// Escape Time algorithm (Fractional)
	int i = 0; // NIterations

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
		color = gradient->at(i % cycles, fractional);
	}
	else
	{
		color = gradient->at(iterations - 1, 0);
	}
	return color;
}
} // namespace ins
