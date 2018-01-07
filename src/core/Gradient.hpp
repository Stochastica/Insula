#ifndef INSULA_CORE_GRADIENT_HPP_
#define INSULA_CORE_GRADIENT_HPP_

#include <boost/property_tree/ptree.hpp>

#include "../util/vector.hpp"

namespace ins
{

class Gradient final
{
public:
	/**
	 * @param[out] nOut Number of sampled color points
	 * @param[in] nIn Number of control points
	 * @param[in] control Control point x
	 */
	Gradient(int nOut, int nIn,
	         real const* const control,
	         real const* const colorR,
	         real const* const colorG,
	         real const* const colorB,
	         real const* const colorA);
	~Gradient();

	int getLength() const noexcept { return n; }

	Color4 at(int i, real frac) const noexcept;
private:
	Color4* colors;
	int n;

};

/**
 * @brief Reads a gradient from a key in a property tree. If the key is not
 * found, returns the default gradient.
 */
Gradient readGradient(
  boost::property_tree::ptree const& tree,
  boost::property_tree::ptree::key_type const& key
);

// Implementations

inline Color4 Gradient::at(int i, real frac) const noexcept
{
	Color4 c0 = colors[i];
	Color4 c1 = i == n - 1 ? colors[0] : colors[i + 1];

	return clamp01((1 - frac) * c0 + frac * c1);
}

} // namespace ins

#endif // !INSULA_CORE_GRADIENT_HPP_
