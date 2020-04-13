#include "stdafx.h"
#include "Vec2.h"
#include <math.h>

float operator*(const Vec2& lhs, const Vec2& rhs)
{
	return lhs.Dot(rhs);
}

float Vec2::Dot(const Vec2& rhs) const
{
	return x * rhs.x + y * rhs.y;
}

float Vec2::Normalize()
{
	float length = Length();

	if (length == 0.0f)
	{
		return 0.0f;
	}

	x /= length;
	y /= length;

	return length;
}

Vec2 Vec2::GetNormalized()
{
	Vec2 res = *this;
	res.Normalize();

	return res;
}

float Vec2::Length() const
{
	return sqrtf(LengthSq());
}

float Vec2::LengthSq() const
{
	return x * x + y * y;
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;

	return *this;
}

Vec2& Vec2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;

	return *this;
}
