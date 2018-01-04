#ifndef INSULA__INTEGRATION_HPP_
#define INSULA__INTEGRATION_HPP_

#include <functional>

#include "fractals.hpp"
#include "core/insula.hpp"
#include "core/TaskParameters.hpp"

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

/**
 * @warning Image must be pre-allocated
 */
void render(Image* const, TaskParameters const&,
            PixelProvider* const);


} // namespace ins

#endif // !INSULA__INTEGRATION_HPP_
