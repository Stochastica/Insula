#include "SceneTest.hpp"

namespace ins
{

Color4 SceneTest::sample(complex z) const
{
	real intPart, fracPart;
	fracPart = std::modf(z.real() * gradient->getLength(),
			&intPart);
	if (z.imag() > 0.5)
		return gradient->at((int) intPart, 0);
	else
		return gradient->at((int) intPart, fracPart);
}

} // namespace ins
