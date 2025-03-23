#pragma once

namespace Engine
{
	template <typename T>
	struct Vector2D
	{
		T X, Y;

		Vector2D() : X(0), Y(0) {}
		Vector2D(T x, T y) : X(x), Y(y) {}

		Vector2D operator+(const Vector2D& other) const
		{
			return Vector2D(X + other.X, Y + other.Y);
		}

		Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}

		Vector2D operator*(T scalar) const
		{
			return Vector2D(X * scalar, Y * scalar);
		}

		Vector2D operator/(T scalar) const
		{
			return Vector2D(X / scalar, Y / scalar);
		}

		Vector2D& operator+=(const Vector2D& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		Vector2D& operator-=(const Vector2D& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		Vector2D& operator*=(T scalar)
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}

		Vector2D& operator/=(T scalar)
		{
			X /= scalar;
			Y /= scalar;
			return *this;
		}

		bool operator==(const Vector2D& other) const
		{
			return X == other.X && Y == other.Y;
		}

		bool operator!=(const Vector2D& other) const
		{
			return !(*this == other);
		}
	};

	inline static float Lerp(float a, float b, float f)
	{
		return a + (b - a) * f;
	}

	inline static Vector2D<float> Lerp(Vector2D<float> a, Vector2D<float> b, float f)
	{
		return Vector2D<float>(Lerp(a.X, b.X, f), Lerp(a.Y, b.Y, f));
	}

}