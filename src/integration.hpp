#ifndef INSULA__INTEGRATION_HPP_
#define INSULA__INTEGRATION_HPP_

#include <functional>

#include "sampling.hpp"
#include "core/Image.hpp"
#include "core/TaskParameters.hpp"
#include "core/insula.hpp"

namespace ins
{

/**
 * @brief Renders the image using the given sampler.
 * @warning Image must be pre-allocated
 */
void render(Image* const, TaskParameters const&, Sampler* const);


} // namespace ins

#endif // !INSULA__INTEGRATION_HPP_
