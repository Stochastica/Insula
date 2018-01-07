#ifndef INSULA_UTIL_VECTOR_HPP_
#define INSULA_UTIL_VECTOR_HPP_

#include "../core/insula.hpp"

namespace ins
{

template <typename F>
class Vector2 final
{
public:
	Vector2() = default;
	Vector2(F x, F y)
	{
		e[0] = x;
		e[1] = y;
	}

	F& x() noexcept
	{
		return e[0];
	}
	F x() const noexcept
	{
		return e[0];
	}
	F& y() noexcept
	{
		return e[1];
	}
	F y() const noexcept
	{
		return e[1];
	}
private:
	F e[2];
};

template <typename F>
class Vector4 final
{
public:
	Vector4() = default;
	Vector4(F a, F b, F c, F d)
	{
		e[0] = a;
		e[1] = b;
		e[2] = c;
		e[3] = d;
	}

	F& operator()(int i) noexcept
	{
		return e[i];
	}
	F operator()(int i) const noexcept
	{
		return e[i];
	}
private:
	F e[4];
};

template <typename T> constexpr Vector4<T>
operator+(Vector4<T> const& v0, Vector4<T> const& v1) noexcept
{
	return Vector4<T>(v0(0) + v1(0),
	                  v0(1) + v1(1),
	                  v0(2) + v1(2),
	                  v0(3) + v1(3));
}
template <typename T> constexpr Vector4<T>
operator*(T l, Vector4<T> const& v) noexcept
{
	return Vector4<T>(l * v(0), l * v(1), l * v(2), l * v(3));
}
template <typename T> constexpr Vector4<T>
clamp01(Vector4<T> const& v) noexcept
{
	return Vector4<T>(
	         v(0) > 1 ? 1 : v(0) < 0 ? 0 : v(0),
	         v(1) > 1 ? 1 : v(1) < 0 ? 0 : v(1),
	         v(2) > 1 ? 1 : v(2) < 0 ? 0 : v(2),
	         v(3) > 1 ? 1 : v(3) < 0 ? 0 : v(3)
	       );
}


typedef Vector2<int> Vector2i;
typedef Vector4<real> Color4;

} // namespace ins

#endif // !INSULA_UTIL_VECTOR_HPP_
