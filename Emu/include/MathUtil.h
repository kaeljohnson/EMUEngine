#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>

namespace Math2D
{
	/*
		Basic 2D point structure with common operations.
		
		Param1: X - The X coordinate of the point.
		Param2: Y - The Y coordinate of the point.
	*/
	template <typename T>
	struct Point2D
	{
		T X, Y;

		Point2D() : X(0), Y(0) {}
		Point2D(T x, T y) : X(x), Y(y) {}

		Point2D operator+(const Point2D& other) const
		{
			return Point2D(X + other.X, Y + other.Y);
		}

		Point2D operator-(const Point2D& other) const
		{
			return Point2D(X - other.X, Y - other.Y);
		}

		Point2D operator*(T scalar) const
		{
			return Point2D(X * scalar, Y * scalar);
		}

		Point2D operator/(T scalar) const
		{
			return Point2D(X / scalar, Y / scalar);
		}

		Point2D& operator+=(const Point2D& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		Point2D& operator-=(const Point2D& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		Point2D& operator*=(T scalar)
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}

		Point2D& operator/=(T scalar)
		{
			X /= scalar;
			Y /= scalar;
			return *this;
		}

		bool operator==(const Point2D& other) const
		{
			return X == other.X && Y == other.Y;
		}

		bool operator!=(const Point2D& other) const
		{
			return !(*this == other);
		}
	};

	// Hash for Point2D<float> to allow hashing.
	struct Point2DFloatHash
	{
		size_t operator()(const Point2D<float>& v) const noexcept {
			size_t h1 = std::hash<float>()(v.X);
			size_t h2 = std::hash<float>()(v.Y);
			return h1 ^ (h2 << 1);
		}
	};

	/*
		Basic 2D vector structure with common operations.
		Param1: X - The X component of the vector.
		Param2: Y - The Y component of the vector.
	*/
	template <typename T>
    struct Vector2D
	{
		Point2D<T> X, Y;

		Vector2D() : X(0), Y(0) {}
		Vector2D(Point2D<T> x, Point2D<T> y) : X(x), Y(y) {}

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

		Vector2D& operator=(const Point2D<T>& point)
		{
			X = point.X;
			Y = point.Y;
			return *this;
		}
	};

	inline static float Lerp(float a, float b, float f)
	{
		return a + (b - a) * f;
	}

	inline static Point2D<float> Lerp(Point2D<float> a, Point2D<float> b, float f)
	{
		return Point2D<float>(Lerp(a.X, b.X, f), Lerp(a.Y, b.Y, f));
	}

    // Geometric structures

	/*
		Edge structure representing a line segment with trailing and leading points.
		When used, edges should be thought of as puzzle pieces, where they connect 
		to edges whose start and end points match either 
		the (trailingPoint, startPoint) or(endPoint, leadingPoint) pairs.
		Trailing and leading points are not intended to be part of the actual edge,
		but rather to facilitate the connection between edges when forming chains.
		Note that the MergeGridLinesIntoChains algorithm merges from trailing point to leading point.

		param1: trailingPoint - The point before the startPoint of the edge.
		param2: startPoint - The starting point of the edge.
		param3: endPoint - The ending point of the edge.
		param4: leadingPoint - The point after the endPoint of the edge.
	*/
	struct Edge
	{
		Point2D<float> m_trailingPoint;
		Point2D<float> m_startPoint;
		Point2D<float> m_endPoint;
		Point2D<float> m_leadingPoint;

		bool operator==(const Edge& other) const
		{
			return m_startPoint == other.m_startPoint &&
				m_endPoint == other.m_endPoint &&
				m_trailingPoint == other.m_trailingPoint &&
				m_leadingPoint == other.m_leadingPoint;
		}

		Edge& operator=(const Edge& other)
		{
			m_trailingPoint = other.m_trailingPoint;
			m_startPoint = other.m_startPoint;
			m_endPoint = other.m_endPoint;
			m_leadingPoint = other.m_leadingPoint;
			return *this;
		}
	};

	struct EdgeHash
	{
		size_t operator()(const Edge& e) const noexcept
		{
			size_t h1 = Point2DFloatHash{}(e.m_startPoint);
			size_t h2 = Point2DFloatHash{}(e.m_endPoint);
			size_t h3 = Point2DFloatHash{}(e.m_trailingPoint);
			size_t h4 = Point2DFloatHash{}(e.m_leadingPoint);
			return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
		}
	};

	/*
		Chain structure representing a merged polyline formed by connecting edges.
		param1: m_points - The points forming the merged polyline.
		param2: m_originalEdges - The original edges that formed this chain.
		param3: m_loop - Whether the chain is a loop.
	*/
	struct Chain
	{
		std::vector<Point2D<float>> m_points;         // merged polyline
		std::vector<Edge> m_originalEdges;	          // original edges that formed this chain

		bool m_loop = false;                          // whether the chain is a loop

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

	using Point = Point2D<float>;

	struct PointPairHash
	{
		size_t operator()(const std::pair<Point, Point>& p) const noexcept
		{
			size_t h1 = Point2DFloatHash{}(p.first);
			size_t h2 = Point2DFloatHash{}(p.second);
			return h1 ^ (h2 << 1);
		}
	};

	using AdjList = std::unordered_map<const std::pair<Point, Point>, const Edge*, PointPairHash>;


	/*
		Helper for MergeGridLinesIntoChains to build adjacency graph from edges.
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

	/*
		Walks the the edges in both directions from the start edge, collecting points and edges,
		and marking edges as visited. Sets loop to true if a loop is detected.
		Param1: start - The starting edge to walk from.
		Param2: adj - The adjacency list mapping edge connections.
		Param3: visitedEdges - The set of already visited edges.
		Param4: outPoints - The output vector to collect points.
		Param5: outEdges - The output vector to collect edges.
		Param6: loop - Output flag indicating if a loop was detected.
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

	/*
		Merges edges into chains by connecting edges based on their start and end points.
		Param1: edges - The input edges to be merged into chains.
		Returns: A vector of merged chains.
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