#ifndef INSULA_CORE_IO_HPP_
#define INSULA_CORE_IO_HPP_

#include <boost/property_tree/ptree.hpp>

// IO utilities for boost::property_tree

namespace ins
{

template <typename T>
std::vector<T> ptree_getArray
(
  boost::property_tree::ptree const& tree,
  boost::property_tree::ptree::key_type const& key
)
{
	std::vector<T> result;
	for (auto& item: tree.get_child(key))
		result.push_back(item.second.get_value<T>());
	return result;
}
/**
 * @brief Reads a json array from a property tree
 */
template <typename T>
boost::optional<std::vector<T>> ptree_getArray_optional
                             (
                               boost::property_tree::ptree const& tree,
                               boost::property_tree::ptree::key_type const& key
                             )
{
	if (auto child = tree.get_child_optional(key))
	{
		std::vector<T> result;
		for (auto& item: child.get())
			result.push_back(item.second.get_value<T>());
		return result;
	}
	else
	{
		return boost::optional<std::vector<T>>();
	}
}

} // namespace ins

#endif // !INSULA_CORE_IO_HPP_
