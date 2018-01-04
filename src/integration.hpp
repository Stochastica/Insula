#ifndef INSULA__INTEGRATION_HPP_
#define INSULA__INTEGRATION_HPP_

#include <functional>

#include "core/Image.hpp"
#include "core/TaskParameters.hpp"
#include "core/insula.hpp"
#include "scene/Scene.hpp"

namespace ins
{

class Sampler
{
public:
	Sampler(int width, int height, Scene* scene):
		width(width), height(height),
		scene(scene)
	{}

	Color4 at(Vector2i const&) const;
private:
	int width, height;
	Scene* scene;
};


/**
 * @warning Image must be pre-allocated
 */
void render(Image* const, TaskParameters const&,
            Sampler* const);


} // namespace ins

#endif // !INSULA__INTEGRATION_HPP_
