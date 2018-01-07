#ifndef INSULA_SCENE_SCENETEST_HPP_
#define INSULA_SCENE_SCENETEST_HPP_

#include "Scene.hpp"
#include "../core/Gradient.hpp"

namespace ins
{

class SceneTest final: public Scene
{
public:
	SceneTest(Gradient* gradient): gradient(gradient) {}

	Color4 sample(complex z) const override;

private:
	Gradient* gradient;
};

} // namespace ins

#endif // !INSULA_SCENE_SCENETEST_HPP_
