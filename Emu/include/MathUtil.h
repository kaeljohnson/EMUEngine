#pragma once

namespace Engine
{
	inline static float Lerp(float a, float b, float f)
	{
		return (a * (1.0f - f)) + (b * f);
	}

	struct Vector2D
	{
		float X, Y;

		Vector2D() : X(0), Y(0) {}
		Vector2D(float x, float y) : X(x), Y(y) {}

		Vector2D operator+(const Vector2D& other) const
		{
			return Vector2D(X + other.X, Y + other.Y);
		}

		Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}

		Vector2D operator*(float scalar) const
		{
			return Vector2D(X * scalar, Y * scalar);
		}

		Vector2D operator/(float scalar) const
		{
			return Vector2D(X / scalar, Y / scalar);
		}

		Vector2D& operator+=(const Vector2D& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}
	};

}