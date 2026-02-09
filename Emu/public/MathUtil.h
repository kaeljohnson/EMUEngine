#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>

namespace Math2D
{
	/**
	* @struct Point2D
	*
	* @brief Basic 2D point structure with common operations.
	*
	* @template T The type of the coordinates (e.g., float, int).
	* 
	* @param X The X coordinate of the point.
	* @param Y The Y coordinate of the point.
	*/
	template <typename T>
	struct Point2D
	{
		T X, Y; /// The X and Y coordinates of the point.

		Point2D() : X(0), Y(0) {}
		Point2D(T x, T y) : X(x), Y(y) {}

		/**
		* @brief Adds two Point2D objects.
		* 
		* @param other The other Point2D to add.
		* 
		* @return A new Point2D representing the sum.
		*/
		Point2D operator+(const Point2D& other) const
		{
			return Point2D(X + other.X, Y + other.Y);
		}

		/**
		* @brief Subtracts another Point2D from this one.
		* 
		* @param other The other Point2D to subtract.
		* 
		* @return A new Point2D representing the difference.
		*/
		Point2D operator-(const Point2D& other) const
		{
			return Point2D(X - other.X, Y - other.Y);
		}

		/**
		* @brief Multiplies this Point2D by a scalar.
		* 
		* @param scalar The scalar to multiply by.
		* 
		* @return A new Point2D representing the product.
		*/
		Point2D operator*(T scalar) const
		{
			return Point2D(X * scalar, Y * scalar);
		}

		/**
		* @brief Divides this Point2D by a scalar.
		* 
		* @param scalar The scalar to divide by. 
		* 
		* @return A new Point2D representing the quotient.
		*/
		Point2D operator/(T scalar) const
		{
			return Point2D(X / scalar, Y / scalar);
		}

		/**
		* @brief Adds another Point2D to this one in place.
		* 
		* @param other The other Point2D to add.
		* 
		* @return A reference to this Point2D after addition.
		*/
		Point2D& operator+=(const Point2D& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		/**
		* @brief Subtracts another Point2D from this one in place.
		* 
		* @param other The other Point2D to subtract.
		* 
		* @return A reference to this Point2D after subtraction.
		*/
		Point2D& operator-=(const Point2D& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		/**
		* @brief Multiplies this Point2D by a scalar in place.
		* 
		* @param scalar The scalar to multiply by.
		* 
		* @return A reference to this Point2D after multiplication.
		*/
		Point2D& operator*=(T scalar)
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}

		/**
		* @brief Divides this Point2D by a scalar in place.
		* 
		* @param scalar The scalar to divide by.
		* 
		* @return A reference to this Point2D after division.
		*/
		Point2D& operator/=(T scalar)
		{
			X /= scalar;
			Y /= scalar;
			return *this;
		}

		/**
		* @brief Checks if this Point2D is equal to another.
		* 
		* @param other The other Point2D to compare with.
		* 
		* @return true if the points are equal, false otherwise.
		*/
		bool operator==(const Point2D& other) const
		{
			return X == other.X && Y == other.Y;
		}

		/**
		* @brief Checks if this Point2D is not equal to another.
		* 
		* @param other The other Point2D to compare with.
		* 
		* @return true if the points are not equal, false otherwise.
		*/
		bool operator!=(const Point2D& other) const
		{
			return !(*this == other);
		}
	};

	/**
	* @struct Point2DFloatHash
	
	* @brief Hash for Point2D<float> to allow hashing.
	*/
	struct Point2DFloatHash
	{
		/**
		* @brief Hashes a Point2D<float>.
		* 
		* @param v The Point2D<float> to hash.
		*/
		size_t operator()(const Point2D<float>& v) const noexcept 
		{
			size_t h1 = std::hash<float>()(v.X);
			size_t h2 = std::hash<float>()(v.Y);
			return h1 ^ (h2 << 1);
		}
	};

	/**
	* @struct Vector2D
	* 
	* @brief Basic 2D vector structure with common operations.
	*/
	template <typename T>
    struct Vector2D
	{
		Point2D<T> X, Y; /// The X and Y components of the vector.

		Vector2D() : X(0), Y(0) {}

		/**
		* @brief Constructs a Vector2D from two Point2D components.
		* 
		* @param x The X component as a Point2D.
		* @param y The Y component as a Point2D.
		*/
		Vector2D(Point2D<T> x, Point2D<T> y) : X(x), Y(y) {}

		/**
		* @brief Adds two Vector2D objects.
		* 
		* @param other The other Vector2D to add.
		* 
		* @return A new Vector2D representing the sum.
		*/
		Vector2D operator+(const Vector2D& other) const
		{
			return Vector2D(X + other.X, Y + other.Y);
		}

		/**
		* @brief Subtracts another Vector2D from this one.
		* 
		* @param other The other Vector2D to subtract.
		* 
		* @return A new Vector2D representing the difference.
		*/
		Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}

		/**
		* @brief Multiplies this Vector2D by a scalar.
		* 
		* @param scalar The scalar to multiply by.
		* 
		* @return A new Vector2D representing the product.
		*/
		Vector2D operator*(T scalar) const
		{
			return Vector2D(X * scalar, Y * scalar);
		}

		/**
		* @brief Divides this Vector2D by a scalar.
		* 
		* @param scalar The scalar to divide by.
		* 
		* @return A new Vector2D representing the quotient.
		*/
		Vector2D operator/(T scalar) const
		{
			return Vector2D(X / scalar, Y / scalar);
		}

		/**
		* @brief Adds another Vector2D to this one in place.
		* 
		* @param other The other Vector2D to add.
		* 
		* @return A reference to this Vector2D after addition.
		*/
		Vector2D& operator+=(const Vector2D& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		/**
		* @brief Subtracts another Vector2D from this one in place.
		* 
		* @param other The other Vector2D to subtract.
		* 
		* @return A reference to this Vector2D after subtraction.
		*/
		Vector2D& operator-=(const Vector2D& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		/**
		* @brief Multiplies this Vector2D by a scalar in place.
		* 
		* @param scalar The scalar to multiply by.
		* 
		* @return A reference to this Vector2D after multiplication.
		*/
		Vector2D& operator*=(T scalar)
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}

		/**
		* @brief Divides this Vector2D by a scalar in place.
		* 
		* @param scalar The scalar to divide by.
		* 
		* @return A reference to this Vector2D after division.
		*/
		Vector2D& operator/=(T scalar)
		{
			X /= scalar;
			Y /= scalar;
			return *this;
		}

		/**
		* @brief Checks if this Vector2D is equal to another.
		* 
		* @param other The other Vector2D to compare with.
		* 
		* @return true if the vectors are equal, false otherwise.
		*/
		bool operator==(const Vector2D& other) const
		{
			return X == other.X && Y == other.Y;
		}

		/**
		* @brief Checks if this Vector2D is not equal to another.
		* 
		* @param other The other Vector2D to compare with.
		* 
		* @return true if the vectors are not equal, false otherwise.
		*/
		bool operator!=(const Vector2D& other) const
		{
			return !(*this == other);
		}

		/**
		* @brief Assigns a Point2D to this Vector2D.
		* 
		* @param point The Point2D to assign.
		* 
		* @return A reference to this Vector2D after assignment.
		*/
		Vector2D& operator=(const Point2D<T>& point)
		{
			X = point.X;
			Y = point.Y;
			return *this;
		}
	};

	/**
	* @brief Linearly interpolates between two float values.
	* 
	* @param a The start value.
	* @param b The end value.
	* @param f The interpolation factor (0.0 to 1.0).
	* 
	* @return The interpolated float value.
	*/
	inline static float Lerp(float a, float b, float f)
	{
		return a + (b - a) * f;
	}

	/**
	* @brief Linearly interpolates between two Point2D<float> points.
	* 
	* @param a The start point.
	* @param b The end point.
	* @param f The interpolation factor (0.0 to 1.0).
	* 
	* @return The interpolated Point2D<float>.
	*/
	inline static Point2D<float> Lerp(Point2D<float> a, Point2D<float> b, float f)
	{
		return Point2D<float>(Lerp(a.X, b.X, f), Lerp(a.Y, b.Y, f));
	}

	/**
	* @struct Edge
	* 
	* @brief Struct representing a line segment with trailing and leading points.
	* When used, edges should be thought of as puzzle pieces, where they connect 
	* to edges whose start and end points match either 
	* the (trailingPoint, startPoint) or(endPoint, leadingPoint) pairs.
	* Trailing and leading points are not intended to be part of the actual edge,
	* but rather to facilitate the connection between edges when forming chains.
	* Note that the MergeGridLinesIntoChains algorithm merges from trailing point to leading point.
	*
	* @param trailingPoint The point before the startPoint of the edge.
	* @param startPoint The starting point of the edge.
	* @param endPoint The ending point of the edge.
	* @param leadingPoint The point after the endPoint of the edge.
	*/
	struct Edge
	{
		Point2D<float> m_trailingPoint; /// The point before the startPoint of the edge.
		Point2D<float> m_startPoint;	/// The starting point of the edge.
		Point2D<float> m_endPoint;		/// The ending point of the edge.
		Point2D<float> m_leadingPoint;	/// The point after the endPoint of the edge.

		/**
		* @brief Checks if this Edge is equal to another.
		* 
		* @param other The other Edge to compare with.
		* 
		* @return true if the edges are equal, false otherwise.
		*/
		bool operator==(const Edge& other) const
		{
			return m_startPoint == other.m_startPoint &&
				m_endPoint == other.m_endPoint &&
				m_trailingPoint == other.m_trailingPoint &&
				m_leadingPoint == other.m_leadingPoint;
		}

		/**
		* @brief Assigns another Edge to this Edge.
		* 
		* @param other The other Edge to assign.
		* 
		* @return A reference to this Edge after assignment.
		*/
		Edge& operator=(const Edge& other)
		{
			m_trailingPoint = other.m_trailingPoint;
			m_startPoint = other.m_startPoint;
			m_endPoint = other.m_endPoint;
			m_leadingPoint = other.m_leadingPoint;
			return *this;
		}
	};

	/**
	* @struct EdgeHash
	* 
	* @brief Hash for Edge to allow hashing in unordered containers.
	*/
	struct EdgeHash
	{
		/**
		* @brief Hashes an Edge.
		* 
		* @param e The Edge to hash.
		* 
		* @return The hash value of the Edge.
		*/
		size_t operator()(const Edge& e) const noexcept
		{
			size_t h1 = Point2DFloatHash{}(e.m_startPoint);
			size_t h2 = Point2DFloatHash{}(e.m_endPoint);
			size_t h3 = Point2DFloatHash{}(e.m_trailingPoint);
			size_t h4 = Point2DFloatHash{}(e.m_leadingPoint);
			return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
		}
	};

	/**
	* @struct Chain
	* 
	* @brief Chain structure representing a merged polyline formed by connecting edges.
	* 
	* @param m_points - The points forming the merged polyline.
	* @param m_originalEdges - The original edges that formed this chain.
	* @param m_loop - Whether the chain is a loop.
	*/
	struct Chain
	{
		std::vector<Point2D<float>> m_points;         /// merged polyline
		std::vector<Edge> m_originalEdges;	          /// original edges that formed this chain

		bool m_loop = false;                          /// whether the chain is a loop

		/**
		* @brief Checks if this Chain is equal to another.
		* 
		* @param other The other Chain to compare with.
		* 
		* @return true if the chains are equal, false otherwise.
		*/
		bool operator==(const Chain& other) const
		{
			if (m_points.size() != other.m_points.size())
				return false;

			for (size_t i = 0; i < m_points.size(); ++i)
			{
				if (m_points[i] != other.m_points[i])
					return false;
			}

			for (size_t i = 0; i < m_originalEdges.size(); ++i)
			{
				if (m_originalEdges[i] != other.m_originalEdges[i])
					return false;
			}

			return true;
		}
	};

	using Point = Point2D<float>; /// Alias for Point2D<float>

	/**
	* @struct PointPairHash
	* 
	* @brief Hash for a pair of Points to allow hashing in unordered containers.
	*/
	struct PointPairHash
	{
		/**
		* @brief Hashes a pair of Points.
		* 
		* @param p The pair of Points to hash.
		* 
		* @return The hash value of the pair of Points.
		*/
		size_t operator()(const std::pair<Point, Point>& p) const noexcept
		{
			size_t h1 = Point2DFloatHash{}(p.first);
			size_t h2 = Point2DFloatHash{}(p.second);
			return h1 ^ (h2 << 1);
		}
	};

	using AdjList = std::unordered_map<const std::pair<Point, Point>, const Edge*, PointPairHash>; /// Adjacency list mapping edge connections

	/**
	* @brief Helper for MergeGridLinesIntoChains to build adjacency graph from edges.
	* 
	* @param edges The input edges to build the adjacency graph from.
	* 
	* @return The constructed adjacency list.
	*/
	inline AdjList buildAdjacencyGraph(const std::vector<Edge>& edges)
	{
		AdjList adj;
		for (const auto& e : edges)
		{
			const auto it = adj.find({ e.m_startPoint, e.m_endPoint });

			adj[{e.m_startPoint, e.m_endPoint}] = &e;
		}
		return adj;
	}

	/**
	* @brief Walks the the edges in both directions from the start edge, collecting points and edges,
	* and marking edges as visited. Sets loop to true if a loop is detected.
	*
	* @param start The starting edge to walk from.
	* @param adj The adjacency list mapping edge connections.
	* @param visitedEdges The set of already visited edges.
	* @param outPoints The output vector to collect points.
	* @param outEdges The output vector to collect edges.
	* @param loop Output flag indicating if a loop was detected.
	*/
	inline void WalkGridLines(const Edge& start,
		AdjList& adj,
		std::unordered_set<Edge, EdgeHash>& visitedEdges,
		std::vector<Point>& outPoints,
		std::vector<Edge>& outEdges,
		bool& loop)
	{
		Edge current = start;

		// Walk forward
		while (true)
		{
			const auto it = adj.find({ current.m_endPoint, current.m_leadingPoint });
			if (it == adj.end()) // No loop
			{
				// no next edge but need to add the final point
				outPoints.push_back(current.m_endPoint);
				break;
			}

			const Edge nextEdge = *it->second;
			if (visitedEdges.count(nextEdge))
			{
				// Already visited
				// endpoint accounted for already because of loop.
				if (current == start)
				{
					loop = true;
				}
				break;

			}

			outPoints.push_back(nextEdge.m_startPoint);

			outEdges.push_back(nextEdge);
			visitedEdges.insert(nextEdge);
			current = nextEdge;
		}

		current = start;

		std::reverse(outPoints.begin(), outPoints.end());

		// Walk backward
		while (true)
		{
			const auto it = adj.find({ current.m_trailingPoint, current.m_startPoint });
			if (it == adj.end()) // No loop
			{
				// No previous edge
				outPoints.push_back(current.m_startPoint);
				break;
			}

			Edge prevEdge = *it->second;
			if (visitedEdges.count(prevEdge))
			{
				// Already visited
				if (current == start)
				{
					loop = true;
				}
				break;
			}

			outPoints.push_back(prevEdge.m_endPoint);

			outEdges.push_back(prevEdge);
			visitedEdges.insert(prevEdge);
			current = prevEdge;
		}

		std::reverse(outPoints.begin(), outPoints.end());
	}

	/**
	* @brief Merges edges into chains by connecting edges based on their start and end points.
	* 
	* @param edges - The input edges to be merged into chains.
	* 
	* @return A vector of merged chains.
	*/
	inline std::vector<Math2D::Chain> MergeGridLinesIntoChains(const std::vector<Edge>& edges)
	{
		AdjList adj = buildAdjacencyGraph(edges);

		std::unordered_set<Edge, EdgeHash> visitedEdges;
		std::vector<Math2D::Chain> chains;

		for (const auto& e : edges)
		{
			if (visitedEdges.count(e))
				continue;

			std::vector<Point> points;
			std::vector<Edge> chainEdges;
			bool loop = false;

			// Walk grid lines
			WalkGridLines(e, adj, visitedEdges, points, chainEdges, loop);

			Math2D::Chain chain;
			chain.m_points = std::move(points);
			chain.m_originalEdges = std::move(chainEdges);
			chain.m_loop = loop;

			chains.push_back(std::move(chain));
		}

		return chains;
	}
}