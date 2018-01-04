#ifndef INSULA_SCENE_SCENE_HPP_
#define INSULA_SCENE_SCENE_HPP_

#include "../core/insula.hpp"
#include "../util/vector.hpp"

namespace ins
{

class Scene
{
public:
	~Scene() = default;
	/**
	 * @brief Obtain 1 sample from the coordinate z.
	 * @param[in] A complex number within [0,1] x [0,1]
	 */
	virtual Color4 sample(complex z) const = 0;
};

} // namespace ins

#endif // !INSULA_SCENE_SCENE_HPP_
