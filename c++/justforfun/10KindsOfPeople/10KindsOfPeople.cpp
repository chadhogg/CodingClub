/// \file 10KindsOfPeople.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/10kindsofpeople

#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>

typedef uint16_t Num;

/// \brief A (row,column) pair specifying a 2-d location.
typedef std::pair<Num, Num> Position;

/// \brief A vector of equal-length strings containing only '0' or '1'.
typedef std::vector<std::string> Map;

/// \brief Compares two positions for equality.
/// \param[in] x The first position.
/// \param[in] y The second position.
/// \return True if their components are identical, or false otherwise.
inline bool operator== (const Position& x, const Position& y) {
  return x.first == y.first && x.second == y.second;
}

/// \brief A specialization of std::hash for positions.
template<>
struct std::hash<Position>
{
  /// A prime number larger than the largest row/column.
  static const Num ARBITRARY_CONSTANT {1009U};

  /// \brief Hashes a position.
  /// \param[in] p A position.
  /// \return A unique hash value for that position.
  std::size_t operator() (const Position& p) const {
    return p.first * ARBITRARY_CONSTANT + p.second;
  }
};


/// \brief Checks the value of a position in the map.
/// \param[in] pos A position.
/// \param[in] map The map.
/// \return True if that position in the map is for decimal people.
inline static bool isOne (const Position& pos, const Map& map) {
  return map[pos.first][pos.second] == '1';
}



/// \brief A collection of connected components in a graph.
class ConnectedComponents {
public:

  /// \brief Finds all of the connected components in a graph.
  /// \param[in] map A planar representation of a graph; each "vertex" will be
  ///   a location within the map.  There are implicit edges between it and the
  ///   vertices in each cardinal direction from it, if they have the same
  ///   value stored.
  /// \post A collection of connected components has been built.
  ConnectedComponents (const Map& map)
    : m_components {} {
    std::unordered_set<Position> freePositions {};

    for (Num row = 0U; row < map.size (); ++row) {
      Num length = map.at (row).length ();
      for (Num col = 0U; col < length; ++col) {
	freePositions.emplace (row, col);
      }
    }

    while (!freePositions.empty ()) {
      buildComponent (map, freePositions);
    }
  }

  /// \brief Checks whether or not two positions are in the same connected
  ///   component.
  /// \param[in] x A position.
  /// \param[in] y Another position.
  /// \return True if they are in the same connected component, or false.
  bool sameComponent (const Position& x, const Position& y) {
    for (const std::unordered_set<Position>& comp : m_components) {
      if (comp.count (x) == 1U) {
	return (comp.count (y) == 1U);
      }
    }
    return false;
  }

private:

  /// \brief Builds one more connected component.
  /// \param[in] map The map.
  /// \param[inout] freePositions The positions that are not part of any
  ///   connected component.
  /// \post An arbitrary free position and all positions reachable from it have
  ///   been put into a new connected component.
  void buildComponent (const Map& map,
		       std::unordered_set<Position>& freePositions) {
    std::unordered_set<Position> newComponent {};
    std::vector<Position> frontier {};
    const Position arbitrary {*freePositions.cbegin ()};
    newComponent.insert (arbitrary);
    frontier.push_back (arbitrary);
    freePositions.erase (arbitrary);

    while (!frontier.empty ()) {
      const Position next = frontier.back ();
      frontier.pop_back ();
      addNeighbors (next, frontier, freePositions, map, newComponent);
    }
    // TODO: I should be using the move semantics here, right?
    m_components.push_back (newComponent);
  }

  /// \brief Finds the neighbors of a position and adds them to the component
  ///   and to the frontier.
  /// \param[in] arbitrary The position whose neighbors should be added.
  /// \param[inout] frontier The positions in the component whose neighbors
  ///   have not yet been added.
  /// \param[inout] freePositions The positions that are not yet part of a
  ///   component.
  /// \param[in] map The map.
  /// \param[inout] component The component.
  /// \post Every neighbor of the arbitrary component has been removed from the
  ///   free positions and added to the frontier and component, unless it was
  ///   already there.
  void addNeighbors (const Position& arbitrary,
		     std::vector<Position>& frontier,
		     std::unordered_set<Position>& freePositions,
		     const Map& map,
		     std::unordered_set<Position>& component) {
    std::vector<Position> neighbors {};
    if (arbitrary.first > 0) {
      neighbors.emplace_back (arbitrary.first - 1, arbitrary.second);
    }
    if (arbitrary.first < map.size () - 1) {
      neighbors.emplace_back (arbitrary.first + 1, arbitrary.second);
    }
    if (arbitrary.second > 0) {
      neighbors.emplace_back (arbitrary.first, arbitrary.second - 1);
    }
    if (arbitrary.second < map.at (0).size () - 1) {
      neighbors.emplace_back (arbitrary.first, arbitrary.second + 1);
    }

    for (const Position& neighbor : neighbors) {
      if (component.count (neighbor) == 0U) {
	if (isOne (arbitrary, map) == isOne (neighbor, map)) {
	  frontier.push_back (neighbor);
	  component.insert (neighbor);
	  freePositions.erase (neighbor);
	}
      }
    }    
  }

  /// A collection of components, each of which is an unordered set of
  ///   positions.
  std::vector<std::unordered_set<Position>> m_components;
};



int main() {
  Num numRows, numCols, numQueries, r1, c1, r2, c2;
  std::cin >> numRows >> numCols;
  Map map {};
  map.reserve (numRows);
  for (Num row {0U}; row < numRows; ++row) {
    std::string line;
    std::cin >> line;
    map.push_back (line);
  }

  ConnectedComponents components (map);

  std::cin >> numQueries;
  for (Num query {0U}; query < numQueries; ++query) {
    Position x, y;
    std::cin >> x.first >> x.second >> y.first >> y.second;
    x.first--;
    x.second--;
    y.first--;
    y.second--;
    if (components.sameComponent (x, y)) {
      if (isOne (x, map)) {
	std::cout << "decimal\n";
      }
      else {
	std::cout << "binary\n";
      }
    }
    else {
      std::cout << "neither\n";
    }
  }
  
  return 0;
}
