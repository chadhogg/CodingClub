/// \file AbstractArt.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/abstractart
/// \note I decided to attack this problem because I misread it as saying that the polygons were convex.
///   With that not being true, it becomes much more difficult.  In fact, I'm quickly losing interest.
/// \note It is currently still in progress.

#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>

using Vertex = std::pair<int, int>;

using Polygon = std::vector<Vertex>;

/// \brief Reads some data from standard input.
/// \tparam T The type to be read.
/// \return The value that was read.
template<typename T>
T read() {
    T value;
    std::cin >> value;
    return value;
}

/// \brief Reads information about a polygon from standard input.
/// \return A polygon object.
Polygon inputPolygon () {
    unsigned int numVertices = read<unsigned int> ();
    Polygon vertices;
    vertices.reserve (numVertices);
    while (vertices.size () < numVertices) {
        int x = read<int> ();
        int y = read<int> ();
        vertices.push_back (Vertex (x, y));
    }
    return vertices;
}

/// \brief Reads information about all polygons from standard input.
/// \return A vector of polygons.
std::vector<Polygon> inputPolygons () {
    std::vector<Polygon> polygons;
    unsigned int numPolygons = read<unsigned int> ();
    while (polygons.size () < numPolygons) {
        polygons.push_back (inputPolygon ());
    }
    return polygons;
}

/// Tests whether or not a polygon's vertices are listed in counter-clockwise order.
/// \param[in] poly The polygon.
/// \return True if is CCW wound, or false otherwise.
/// \note This idea comes from a James Norton blog post.
bool isPolygonCCW (Polygon const& poly) {
    int sum = 0;
    for (unsigned int index {0U}; index < poly.size (); ++index) {
        unsigned int next = (index + 1) % poly.size ();
        sum += (poly.at (next).first - poly.at (index).first) * (poly.at (next).second + poly.at (index).second);
    }
    return sum < 0;
}

void correctWinding (std::vector<Polygon>& polys) {
    for (unsigned int index {0U}; index < polys.size (); ++index) {
        if (!isPolygonCCW (polys.at (index))) {
            std::reverse (polys.at (index).begin () + 1, polys.at (index).end ());
        }
    }
}

/// \brief Computes the signed area of the triangle with three vertices.
/// \param[in] a The first vertex.
/// \param[in] b The second vertex.
/// \param[in] c The third vertex
/// \return The signed area of the triangle.
/// \note This works by finding the vectors b->a and c->a, assuming their z components are 0, and computing their cross product.
///   The result has zero for its x and y components, so they are removed.  Then it is refactored to minimize the
///   number of multiplications:
/// v1 = b - a = (bx - ax, by - ay)
/// v2 = c - a = (cx - ax, cy - ay)
/// v1 x v2 = (0, 0, v1x * v2y - v1y * v2x)
/// |v1 x v2| = v1x * v2y - v1y * v2x
///  = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax)
///  = bxcy - aybx - axcy + axay - (bycx - axby - aycx + axay)
///  = bxcy - aybx - axcy + axay - bycx + axby + aycx - axay
///  = axby - axcy - aybx + aycx + bxcy - bycx
///  = ax (by - cy) - aybx + aycx + bxcy - bycx
///  = ax (by - cy) + bx (cy - ay) + cx (ay - by)
/// And then since the magnitude of the cross product gives us the area of the parallelopiped, half of it gives us the triangle area.
static inline double signedTriangleArea (Vertex const& a, Vertex const& b, Vertex const& c) {
    return 0.5 * (a.first * (b.second - c.second) + b.first * (c.second - a.second) + c.first * (a.second - b.second));
}

/// \brief Computes the area of a triangle.
/// \param[in] a The first vertex.
/// \param[in] b The second vertex.
/// \param[in] c The third vertex.
/// \return The area of the triangle.
static inline double triangleArea (Vertex const& a, Vertex const& b, Vertex const& c) {
    return abs (signedTriangleArea (a, b, c));
}

/// \brief Determines whether or not a vertex is convex (has an angle below 180 degrees).
/// \param[in] before The vertex before that one.
/// \param[in] current The vertex we are asking about.
/// \param[in] after The following vertex.
/// \return True if that vertex is convex; false otherwise.
/// \note This works because if the vertices are specified in counter-clockwise order the area will be positive.
///   And if they are specified in clockwise order it will be negative.
static inline bool isVertexConvex (Vertex const& before, Vertex const& current, Vertex const& after) {
    return signedTriangleArea (before, current, after) > 0;
}

/// \brief Determines whether not a point is inside a triangle.
/// \param[in] a The first vertex of the triangle.
/// \param[in] b The second vertex of the triangle.
/// \param[in] c The third vertex of the triangle.
/// \return If p is inside triangle a-b-c.
/// \note This uses the fact that if the point is inside the triangle every one of these triangles will be in
///   counter-clockwise order, and if it isn't at least one of them will be clockwise.
static inline bool isInside (Vertex const& a, Vertex const& b, Vertex const& c, Vertex const& p) {
    return signedTriangleArea (a, b, p) > 0 &&
            signedTriangleArea (b, c, p) > 0 &&
            signedTriangleArea (c, a, p) > 0;
}

/// \brief Searches a polygon for an ear.
/// \param[in] poly The polygon.
/// \return The index of the principal vertex of an ear.
/// \note This algorithm is based on a paper by David Eberly.  The claim is that if no vertices lie in the triangle formed
///   around the principal vertex, then every part of the diagonal falls inside the polygon.  That seems right to me.
static inline unsigned int findEar (Polygon const& poly) {
    for (unsigned int currentIndex {0U}; currentIndex < poly.size (); ++currentIndex) {
        unsigned int nextIndex = (currentIndex + 1) % poly.size ();
        unsigned int prevIndex = (currentIndex + poly.size () - 1) % poly.size ();
        if (isVertexConvex (poly.at (prevIndex), poly.at (currentIndex), poly.at (nextIndex))) {
            bool foundInside = false;
            for (unsigned int otherIndex {0U}; otherIndex < poly.size (); ++otherIndex) {
                if (otherIndex != currentIndex && otherIndex != nextIndex && otherIndex != prevIndex &&
                    isInside (poly.at (prevIndex), poly.at (currentIndex), poly.at (nextIndex), poly.at (otherIndex))) {
                    foundInside = true;
                    break;
                }
            }
            if (!foundInside) {
                return currentIndex;
            }
        }
    }
    throw std::runtime_error("Should not be possible to not have ear.");
}

/// \brief Converts a polygon into a collection of triangles.
/// \param[in] poly A polygon (which is copied).
/// \return A vector of triangles equivalent to the original polygon.
/// \note This works by repeatedly clipping an ear off of the polygon until it becomes a triangle itself.
std::vector<Polygon> triangulatePolygon (Polygon poly) {
    std::vector<Polygon> triangles;
    while (poly.size () > 3) {
        unsigned int principal = findEar (poly);
        Polygon tri;
        tri.push_back (poly.at ((principal + poly.size() - 1) % poly.size ()));
        tri.push_back (poly.at (principal));
        tri.push_back (poly.at ((principal + 1) % poly.size ()));
        triangles.push_back (tri);
        poly.erase (std::next (poly.begin (), principal));
    }
    triangles.push_back (poly);
    return triangles;
}

/// \brief Computes the area of a polygon.
/// \param[in] poly The polygon.
/// \return Its area.
double computeArea (Polygon const& poly) {
    double total {0.0};
    std::vector<Polygon> triangles = triangulatePolygon (poly);
    for (Polygon const& poly : triangles) {
        total += signedTriangleArea (poly.at (0), poly.at (1), poly.at (2));
    }
    return total;
}

/// \brief Computes the area of a collection of polygons.
/// \param[in] polys The polygons.
/// \return The sum of their areas.
double computeAreas (std::vector<Polygon> const& polys) {
    double total {0.0};
    for (Polygon poly : polys) {
        total += computeArea (poly);
    }
    return total;
}

// TODO implement this (with lots of helper functions)
void unionPolygons (std::vector<Polygon>& polys) {
    bool madeChange = true;
    while (madeChange) {
        madeChange = false;
        for (unsigned int firstIndex {0U}; firstIndex < polys.size (); ++firstIndex) {
            for (unsigned int secondIndex {firstIndex + 1}; secondIndex < polys.size (); ++secondIndex) {
                // if first poly lies completely within second poly
                //    replace first poly with second poly, erase second copy of second poly
                // else if second poly lies completely within first poly
                //    erase second poly
                // else if parts of first poly lie inside second poly and other parts do not
                //    make a new poly, containing any vertex of first poly that is outside of second poly
                //    while haven't gotten back to the initial point of new poly
                //        if most recently added vertex was from first poly
                //            if none of second poly's edges intersect segment from most recently added vertex to next in first poly
                //                add next vertex of first poly to new poly
                //            else
                //                add first intersection point to new poly
                //                consider this a vertex of second poly for purposes of where to go next
                //        else
                //            just like above, but swapping first/second polys
                //    replace first poly with new poly
                //    erase second poly
                //    madeChange = true;
            }
        }
    }
}


/// \brief Runs the program.
/// \return Always 0.
int main () {
    std::vector<Polygon> polygons = inputPolygons ();
    correctWinding (polygons);
    std::cout << computeAreas (polygons) << "\n";
}