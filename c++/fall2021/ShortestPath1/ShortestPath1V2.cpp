/// \file ShortestPath1V2.cpp
/// \author Chad Hogg
/// \brief Another attempt at https://open.kattis.com/problems/shortestpath1
/// My other code was getting so messy from my various attempts to optimize it
///   that it seemed cleanest to just start fresh.
/// \note Currently Kattis is reporting a Run Time Exception for this.  Of
///   course, I can't reproduce it with any of the test cases I generate.

#include <unordered_map>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>

#define NDEBUG

/// The number that represents a vertex, which can range from 0 to 100000.
typedef unsigned int Vertex;
/// The number that represents an edge weight, which can range from 0 to 1000.
typedef unsigned int Weight;
/// The distance from one vertex to another.
typedef unsigned int Distance;



/// \brief A fairly sparse weighted graph, represented as an adjacency-list.
/// More precisely, it doesn't store a list at all.  For each vertex, it stores
///   a map from destinations to weights.  The non-existence of a vertex in
///   that map implies the non-existence of an edge to it.
class WeightedGraph {
public:

  /// \brief Constructs a new weighted graph.
  /// \param[in] numVertices The number of vertices it will have.  Each is
  ///   implied to have a unique label chosen from [0, numVertices).
  /// \param[in] numEdges The number of edges it will have.  Each has a
  ///   non-negative weight.
  /// \param[inout] is An input stream from which edge data can be read.  It
  ///   should contain numEdges lines, each of which contains a vertex label,
  ///   another vertex label, and finally a weight.  Each vertex label pair
  ///   should be unique.
  /// \post The edges have been extracted from the stream and stored.
  WeightedGraph (unsigned int numVertices, unsigned int numEdges, std::istream& is)
    : m_numVertices {numVertices}, m_edges {} {
    m_edges.reserve (numVertices);
    // Each vertex gets an empty map of outgoing edges.
    for (Vertex vertex {0U}; vertex < numVertices; ++vertex) {
      // NOTE: I don't quite understand how this code I yoinked from the web
      //   works.  I'm trying to say to make the new unordered_map directly in
      //   place instead of making it, copying it, and discarding the copy.
      m_edges.emplace (std::piecewise_construct,
		       std::forward_as_tuple (vertex),
		       std::tuple<> {});
    }
    // Read all of the edges from input.
    for (unsigned int edgeNum {0U}; edgeNum < numEdges; ++edgeNum) {
      Vertex source, destination;
      Weight weight;
      is >> source >> destination >> weight;
      assert (source < numVertices);
      assert (destination < numVertices);
      assert (m_edges[source].count (destination) == 0U);
      m_edges[source][destination] = weight;
    }
  }

  /// \brief Gets the outgoing edges for a vertex.
  /// \param[in] source The source vertex.
  /// \return A reference to a map of destination vertex to weight for every
  ///   outgoing edge that source vertex has.  The reference will become
  ///   invalid when this object goes out of scope.
  inline const std::unordered_map<Vertex, Weight>& getOutgoingEdges (Vertex source) const {
    assert (source < m_numVertices);
    return m_edges.at (source);
  }

  /// \brief Gets the number of vertices in the graph.
  /// \return The number of vertices.
  inline unsigned int getNumVertices () const {
    return m_numVertices;
  }
  
private:
  /// The number of vertices, which also happens to be 1 greater than the
  ///   largest vertex label.
  unsigned int m_numVertices;
  /// A map from source vertex to a map from destination vertex to weight.
  std::unordered_map<Vertex, std::unordered_map<Vertex, Weight>> m_edges;
};


/// \brief A class for comparing Vertex-Distance pairs with shorter distances
///   getting higher priorities.
class DistanceComparator {
public:
  /// \brief Compares two Vertex-Distance pairs.
  /// \param[in] x The first pair.
  /// \param[in] y The second pair.
  /// \return True if the first pair should come later in an ordering (have
  ///    lower priority) [have a longer distance].
  inline bool operator() (const std::pair<Vertex, Distance>& x,
			  const std::pair<Vertex, Distance>& y) const {
    return x.second > y.second;
  }
};


/// \brief A class that stores the shortest distances in a graph, finding them
///   using Dijkstra's algorithm.
/// Invariant: A vertex can only be in one of three states:
///   (1) It does not exist in either m_tentativeDistances or
///     m_finalizedDistances, because no path to it has yet been found.
///   (2) It exists only in m_tentativeDistances, because some paths to it have
///     been found but other shorter ones might exist.
///   (3) It exists only in m_finalizedDistances, because the shortest path to
///     it has definitely been found.
/// Once the object is fully constructed, only cases (1) and (3) are possible.
class ShortestDistances {
public:

  /// \brief Solves the single-source, all-destinations shortest path problem
  ///   for non-negative weights, without providing the actual paths.
  /// \param[in] graph The graph to work in.
  /// \param[in] start The single source from which distances are desired.
  /// \post The shortest distance to every vertex reachable from start has been
  ///   calculated and stored.
  ShortestDistances (const WeightedGraph& graph, Vertex start)
    : m_start (start), m_tentativeDistances {}, m_finalizedDistances {}, m_priorityQueue {} {
    m_tentativeDistances.reserve (graph.getNumVertices ());
    m_finalizedDistances.reserve (graph.getNumVertices ());
    m_tentativeDistances[m_start] = 0U;
    m_priorityQueue.emplace_back (m_start, 0U);

    while (!m_tentativeDistances.empty ()) {
      Vertex current = finalizeClosestVertex ();
      processNeighbors (graph, current);
    }
    m_priorityQueue.clear ();
  }

  /// \brief Gets the distance to a vertex.
  /// \param[in] destination The requested vertex, which should be reachable.
  /// \return The distance along the shortest path from the source to it.
  inline Distance getDistance (Vertex destination) const {
    if (m_finalizedDistances.count (destination) == 0U) {
      throw std::runtime_error (destination + " is not reachable from " + m_start);
    }
    return m_finalizedDistances.at (destination);
  }

  /// \brief Gets whether or not a vertex is reachable.
  /// \param[in] destination The requested vertex.
  /// \return True if the destination is reachable; false otherwise.
  inline bool isReachable (Vertex destination) const {
    return (m_finalizedDistances.count (destination) == 1);
  }
  
private:

  /// \brief Determines the not-yet-finalized vertex with lowest tentative
  ///   distance, and finalizes it.
  /// This is because the vertex with lowest tentative distance can't possibly
  ///   have a shorter path to it than the ones that have already been
  ///   considered.
  /// \return The vertex that was finalized.
  /// \post The vertex on the frontier with lowest distance no longer has a
  ///   tentative distance but instead has a finalized distance.
  Vertex finalizeClosestVertex () {
    std::pop_heap (m_priorityQueue.begin (), m_priorityQueue.end (), m_comp);
    std::pair<Vertex, Distance> pair = m_priorityQueue.back ();
    m_priorityQueue.pop_back ();
    
    // Because we do not update distances, the priority queue may contain
    //   spurious entries for a vertex.
    // We can tell that an entry is spurious its vertex has already had its
    //   distance finalized (implying that a shorter path was found).
    // In that case, we simply discard it and move on.
    while (m_tentativeDistances.count (pair.first) == 0U) {
      assert (m_finalizedDistances.at (pair.first) <= pair.second);
      std::pop_heap (m_priorityQueue.begin (), m_priorityQueue.end (), m_comp);
      pair = m_priorityQueue.back ();
      m_priorityQueue.pop_back ();
    }
    
    m_tentativeDistances.erase (pair.first);
    m_finalizedDistances.insert (pair);
    return pair.first;
  }

  /// \brief Updates the tentative distances of all neighbors of a current
  ///   vertex.
  /// \param[in] graph The graph to work in.
  /// \param[in] current The current vertex.
  /// \post For every successor vertex of current, if either no path to it had
  ///   yet been found or the path to it through the current vertex is shorter
  ///   than the shortest path found so far, its tentative distance has been
  ///   lowered to the distance through the current vertex.
  void processNeighbors (const WeightedGraph& graph, Vertex current) {
    Distance soFar = m_finalizedDistances[current];
    for (const std::pair<Vertex, Weight>& outgoingEdge : graph.getOutgoingEdges (current)) {
      // If the neighbor's distance has already been finalized, skip it.
      if (m_finalizedDistances.count (outgoingEdge.first) == 0U) {
	const Distance combined = soFar + outgoingEdge.second;
	// If never reached or going through this node is faster, update.
	if (m_tentativeDistances.count (outgoingEdge.first) == 0U ||
	    m_tentativeDistances[outgoingEdge.first] > combined) {
	  m_tentativeDistances[outgoingEdge.first] = combined;
	  // Trying to find the entry in the priority queue to update it would
	  //   be a linear-time search, which is too slow.
	  // Instead, just add another copy with the reduced distance.
	  // When removing from the priority queue we will ignore entries for
	  //   vertices whose distances have already been finalized.
	  m_priorityQueue.emplace_back (outgoingEdge.first, combined);
	  std::push_heap (m_priorityQueue.begin (), m_priorityQueue.end (), m_comp);
	}
      }
    }
  }

  /// The start vertex.
  Vertex m_start;
  /// A map from destination vertices to the shortest distance yet discovered
  ///   to them from m_start.
  std::unordered_map<Vertex, Distance> m_tentativeDistances;
  /// A map from destination vertices to the shortest distance to them from
  ///   m_start.  If a vertex appears in this map, then its shortest distance
  ///   has been determined.
  std::unordered_map<Vertex, Distance> m_finalizedDistances;
  /// A vector that should be interpreted as a priority queue of vertices on
  ///   the frontier -- those for which some path but not necessarily the
  ///   shortest has been found.  To save time, entries in this are *not*
  ///   updated.  Thus, when an entry is removed from the priority queue it
  ///   should be treated as spurious unless that vertex still has a tenative
  ///   distance.
  std::vector<std::pair<Vertex, Distance>> m_priorityQueue;
  /// A way to compare Vertex-Distance pairs such that shortest distances are
  ///   highest priority.
  DistanceComparator m_comp;
};



/// \brief Runs the program.
/// \return Always 0.
/// Repeatedly reads problems from input and solves them until finding the
///   sentinel value of a line containing 4 0s.
int main () {
  unsigned int numVertices, numEdges, numQueries, startVertex, currentQuery;
  std::cin >> numVertices >> numEdges >> numQueries >> startVertex;
  while (numVertices != 0U || numEdges != 0U || numQueries != 0U || startVertex != 0U) {
    WeightedGraph graph {numVertices, numEdges, std::cin};
    ShortestDistances distances {graph, startVertex};
    for (unsigned int count = 0U; count < numQueries; ++count) {
      std::cin >> currentQuery;
      if (distances.isReachable (currentQuery)) {
	std::cout << distances.getDistance (currentQuery) << "\n";
      }
      else {
	std::cout << "Impossible\n";
      }
    }

    std::cin >> numVertices >> numEdges >> numQueries >> startVertex;
  }
  return 0;
}
