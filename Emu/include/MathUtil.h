#pragma once

#include <type_traits>

namespace Engine
{
	inline static float Lerp(float a, float b, float f)
	{
		return (a * (1.0f - f)) + (b * f);
	}

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

		template <typename U>
		Vector2D<typename std::common_type<T, U>::type> operator+(const Vector2D<U>& other) const
		{
			using ReturnType = typename std::common_type<T, U>::type;
			return Vector2D<ReturnType>(X + other.X, Y + other.Y);
		}

		Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}

		template <typename U>
		Vector2D<typename std::common_type<T, U>::type> operator-(const Vector2D<U>& other) const
		{
			using ReturnType = typename std::common_type<T, U>::type;
			return Vector2D<ReturnType>(X - other.X, Y - other.Y);
		}

		Vector2D operator*(T scalar) const
		{
			return Vector2D(X * scalar, Y * scalar);
		}

		template <typename U>
		Vector2D<typename std::common_type<T, U>::type> operator*(U scalar) const
		{
			using ReturnType = typename std::common_type<T, U>::type;
			return Vector2D<ReturnType>(X * scalar, Y * scalar);
		}

		Vector2D operator/(T scalar) const
		{
			return Vector2D(X / scalar, Y / scalar);
		}
		
		template <typename U>
		Vector2D<typename std::common_type<T, U>::type> operator/(U scalar) const
		{
			using ReturnType = typename std::common_type<T, U>::type;
			return Vector2D<ReturnType>(X / scalar, Y / scalar);
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


	inline void Clamp(float& valueToBeClamped, const float minValue, const float maxValue)
	{
		if (valueToBeClamped < minValue)
		{
			valueToBeClamped = minValue;
		}
		else if (valueToBeClamped > maxValue)
		{
			valueToBeClamped = maxValue;
		}
	}

	inline void clamp(Vector2D<float>& valueToBeClamped, const Vector2D<float>& minValue, const Vector2D<float>& maxValue)
	{
		Clamp(valueToBeClamped.X, minValue.X, maxValue.X);
		Clamp(valueToBeClamped.Y, minValue.Y, maxValue.Y);
	}
}