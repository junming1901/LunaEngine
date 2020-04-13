#pragma once

struct Vec2
{
	union
	{
		float m[2];

		struct
		{
			float x, y;
		};
	};

	float Dot(const Vec2& rhs) const;
	
	float Normalize();
	Vec2 GetNormalized();

	float Length() const;
	float LengthSq() const;

	Vec2& operator+=(const Vec2& rhs);
	Vec2& operator-=(const Vec2& rhs);

	Vec2& operator*=(float rhs);
	Vec2& operator/=(float rhs);
};

// Dot Product
float operator*(const Vec2& lhs, const Vec2& rhs);