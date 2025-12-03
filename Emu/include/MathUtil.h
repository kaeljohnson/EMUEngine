#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <iostream>


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

    // Hash for Vector2D<float> to allow hashing.
    struct Vector2DFloatHash
    {
        size_t operator()(const Vector2D<float>& v) const noexcept {
            size_t h1 = std::hash<float>()(v.X);
            size_t h2 = std::hash<float>()(v.Y);
            return h1 ^ (h2 << 1);
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

    // Geometric structures
    struct Edge
    {
		size_t m_id; // optional identifier
        Vector2D<float> m_startPoint;
        Vector2D<float> m_endPoint;
    };

	struct Adjacency
	{
		std::vector<Vector2D<float>> m_previousPoints;
		std::vector<Vector2D<float>> m_nextPoints;
	};

    /*struct EdgeHash
    {
        size_t operator()(const Edge& e) const noexcept 
        {
            size_t h1 = Vector2DFloatHash{}(e.a);
            size_t h2 = Vector2DFloatHash{}(e.b);
            size_t h3 = std::hash<size_t>{}(e.id);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };*/

    struct Chain
    {
        std::vector<Vector2D<float>> m_points;        // merged polyline
        std::vector<Edge> m_originalEdges;	          // original edges that formed this chain

		bool m_loop = false;                          // whether the chain is a loop
		Vector2D<float> m_loopVertex = { INFINITY, INFINITY }; // vertex where loop closes (if applicable)
    };

    inline std::vector<Chain> DirectionalMergEdgesToChains(const std::vector<Edge>& edges)
    {
        std::unordered_map<Vector2D<float>, Adjacency, Vector2DFloatHash> graph;
		for (const Edge& e : edges)
		{
			graph[e.m_startPoint].m_nextPoints.push_back(e.m_endPoint);
			graph[e.m_endPoint].m_previousPoints.push_back(e.m_startPoint);
		} 

        auto edgeKey = [](const Vector2D<float>& a, const Vector2D<float>& b)
            {
                return std::pair<Vector2D<float>, Vector2D<float>>(a, b);
            };

        struct PairHash
        {
            std::size_t operator()(const std::pair<Vector2D<float>, Vector2D<float>>& p) const noexcept
            {
                Vector2DFloatHash h;
                return h(p.first) ^ (h(p.second) << 1);
            }
        };

        std::unordered_map<
            std::pair<Vector2D<float>, Vector2D<float>>, // pair of points as key.
            Edge,                                        // maps to edge so we can keep track of original edges and their entities.
            PairHash
        > edgeTable;

        for (const Edge& e : edges)
        {
            edgeTable[edgeKey(e.m_startPoint, e.m_endPoint)] = e;
            edgeTable[edgeKey(e.m_endPoint, e.m_startPoint)] = e;
        }

		std::unordered_set<Vector2D<float>, Vector2DFloatHash> visited;
        std::vector<Chain> chains;

        auto walk = [&](const Vector2D<float>& start) -> Chain
        {
                Chain chain;
				chain.m_points.clear();
				chain.m_originalEdges.clear();

				Vector2D<float> current = start;
				Vector2D<float> prev = { INFINITY, INFINITY };

                while (true)
                {
					for (auto& nextPoint : graph[current].m_nextPoints)
					{
                        chain.m_points.push_back(current);
                        visited.insert(current);

						if (nextPoint != prev && visited.count(nextPoint) == 0)
						{

							// Store the edge between current and next vertex.
							auto it = edgeTable.find(edgeKey(current, nextPoint));
							if (it != edgeTable.end())
								chain.m_originalEdges.push_back(it->second);

							prev = current;
							current = nextPoint;
							break;
						}
                        if (nextPoint != prev && visited.count(nextPoint) > 0)
                        {
							chain.m_loop = true;
                            chain.m_points.push_back(nextPoint); // close the loop
                            break;
                        }

					}


                }
        };
    }

    inline std::vector<Chain> MergeGridLinesIntoChains(const std::vector<Edge>& edges)
    {
        // 1. Build adjacency graph.
        std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>, Vector2DFloatHash> graph;

        for (const Edge& e : edges)
        {
            graph[e.m_startPoint].push_back(e.m_endPoint);
            graph[e.m_endPoint].push_back(e.m_startPoint);
        }

        // 2. Track visited vertices.
        std::unordered_set<Vector2D<float>, Vector2DFloatHash> visited;

		std::vector<Chain> chains;

        auto walk = [&](const Vector2D<float>& startPoint, const Vector2D<float>& currentPoint, bool& completeLoop) -> Chain
            {
				std::vector<Vector2D<float>>& neighbors = graph[currentPoint];

				Chain chain;
				chain.m_points.clear();
				chain.m_originalEdges.clear();

				// Vector2D<float> current = startPoint;
				Vector2D<float> prev = { INFINITY, INFINITY };
				if (neighbors.size() > 2) throw std::runtime_error("Node has more than 2 neighbors, cannot form simple grid chain.");
                if (neighbors.size() == 2)
                {
                    bool completeLoop1 = true;
					bool completeLoop2 = true;
					Chain chain1;
					Chain chain2;

					Chain chain1 = walk(neighbors[0], completeLoop1);
					Chain chain2 = walk(neighbors[1], completeLoop2);

					if (completeLoop1 && completeLoop2)
					{
						// start point was intersection, can concatenate on either side.
                        chain1.m_points.insert(chain1.m_points.end(), chain2.m_points.begin(), chain2.m_points.end());
                        return chain1;
					}
					else if (completeLoop1)
					{
						// Chain 2 must be concatenated to chain 1
                        chain1.m_points.insert(chain1.m_points.end(), chain2.m_points.begin(), chain2.m_points.end());
						return chain1;
					}
					else if (completeLoop2)
					{
						// Chain 1 must be concatenated to chain 2
						chain2.m_points.insert(chain2.m_points.end(), chain1.m_points.begin(), chain1.m_points.end());
						return chain2;
					}
					else
					{
						// Neither side completed a loop, concatenate chains
						chain1.m_points.insert(chain1.m_points.end(), chain2.m_points.begin(), chain2.m_points.end());
						return chain1;
					}

				}
				else
				{
                    if (visited.count(currentPoint) == 0)
                    {
                        bool completeLoopX = false;
						chain.m_points.push_back(walk(startPoint, neighbors[0], completeLoopX));
						visited.insert(currentPoint);

                    }
                }
            };


        // 4. Find all endpoints (degree == 1) ? start polylines here
        for (const auto& [point, neighbors] : graph)
        {
            if (neighbors.size() == 1 && visited.count(point) == 0)
            {
				bool completeLoop0 = true;
                chains.push_back(walk(point, completeLoop0));
            }
        }

		return chains;

    }

    inline std::vector<Chain> MergeEdgesToChains(const std::vector<Edge>& edges)
    {
        // 1. Build adjacency graph.
        std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>, Vector2DFloatHash> graph;

        for (const Edge& e : edges)
        {
            graph[e.m_startPoint].push_back(e.m_endPoint);
            graph[e.m_endPoint].push_back(e.m_startPoint);
        }

        auto edgeKey = [](const Vector2D<float>& a, const Vector2D<float>& b)
            {
                return std::pair<Vector2D<float>, Vector2D<float>>(a, b);
            };

        struct PairHash
        {
            std::size_t operator()(const std::pair<Vector2D<float>, Vector2D<float>>& p) const noexcept
            {
                Vector2DFloatHash h;
                return h(p.first) ^ (h(p.second) << 1);
            }
        };

        std::unordered_map<
			std::pair<Vector2D<float>, Vector2D<float>>, // pair of points as key.
			Edge,                                        // maps to edge so we can keep track of original edges and their entities.
            PairHash
        > edgeTable;

        for (const Edge& e : edges)
        {
            edgeTable[edgeKey(e.m_startPoint, e.m_endPoint)] = e;
            edgeTable[edgeKey(e.m_endPoint, e.m_startPoint)] = e;
        }

        // 3. Track visited vertices.
        std::unordered_set<Vector2D<float>, Vector2DFloatHash> visited;

        std::vector<Chain> chains;

        // Walk a path from a given start vertex
        auto walk = [&](const Vector2D<float>& start) -> Chain
            {
                Chain chain;
                chain.m_points.clear();
                chain.m_originalEdges.clear();

                Vector2D<float> current = start;
                Vector2D<float> prev = { INFINITY, INFINITY };

				std::cout << "Start: (" << start.X << ", " << start.Y << ")\n";

                while (true)
                {
                    std::cout << "Visiting: (" << current.X << ", " << current.Y << ")\n";
                    chain.m_points.push_back(current);
                    visited.insert(current);

                    const auto& neighbors = graph[current];

                    // Find next vertex
                    Vector2D<float> next = { INFINITY, INFINITY };

                    for (auto& n : neighbors)
                    {
                        if (n != prev && visited.count(n) == 0)
                        {
                            next = n;
                            break;
                        }
                        if (n != prev && n == start)
                        {
                            chain.m_loop = true;
							//if (n != start) throw std::runtime_error("Connecting vertex must be at start");
                            chain.m_loopVertex = start;
							next = n;
                            break;
                        }
                    }

                    // If no valid next, chain ends
                    if (next.X == INFINITY)
                        break;

                    // Store the edge between current and next vertex.
                    auto it = edgeTable.find(edgeKey(current, next));
                    if (it != edgeTable.end())
                        chain.m_originalEdges.push_back(it->second);

                    prev = current;
                    current = next;

                    if (chain.m_loop)
                    {
						// throw std::runtime_error("Loops not fully supported yet.");
                        break;
                    }
                }

                return chain;
            };


        for (const auto& [point, neighbors] : graph)
        {
			std::cout << "Point (" << point.X << ", " << point.Y << ") has " << neighbors.size() << " neighbors." << std::endl;
        }

        // 4. Find all endpoints (degree == 1) ? start polylines here
        for (const auto& [point, neighbors] : graph)
        {
            if (neighbors.size() == 1 && visited.count(point) == 0)
            {
                chains.push_back(walk(point));
            }
        }

        // 5. Any remaining points must be loops or leftover segments
        for (const auto& [point, neighbors] : graph)
        {
            if (visited.count(point) == 0)
            {
                chains.push_back(walk(point));
            }
        }

        return chains;
    }
}
