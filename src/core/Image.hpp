#ifndef INSULA_CORE_IMAGE_HPP_
#define INSULA_CORE_IMAGE_HPP_

#include <cstddef>

namespace ins
{

struct Image
{
	uint8_t* pixelsR;
	uint8_t* pixelsG;
	uint8_t* pixelsB;
	uint8_t* pixelsA;
	int width;
	int height;
};

} // namespace ins

#endif // !INSULA_CORE_IMAGE_HPP_
