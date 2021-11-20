/// \file ShortestPath1.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/shortestpath1
/// \note I had this working at some point, but I ran into Time Limit Exceeded,
///   and then after fixing that I had Memory Limit Exceeded, and then in my
///   attempts to fix it I broke it to the point where I don't even get the
///   correct answer all the time any more.
/// So I have abandoned this code base entirely and am now working on
///   \see ShortestPath1V2.cpp.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <limits>

class WeightedGraph {
public:
  //virtual ~WeightedGraph () {}
  virtual unsigned int getWeight (unsigned int source, unsigned int dest) const = 0;
  virtual bool connected (unsigned int source, unsigned int dest) const = 0;

  virtual inline const std::unordered_map<unsigned int, unsigned int>& getOutgoingEdges (unsigned int vertex) const = 0;
private:
};





class MatrixWeightedGraph : public WeightedGraph {

public:

  MatrixWeightedGraph (unsigned int numNodes, unsigned int numEdges, std::istream& is)
    : numVert {numNodes}, weights (numNodes * numNodes) {
    std::fill (weights.begin (), weights.end (), NO_EDGE);
    for (unsigned int count = 0U; count < numEdges; ++count) {
      unsigned int source, destination, weight;
      is >> source >> destination >> weight;
      setWeight (source, destination, static_cast<int> (weight));
    }
  }

  //virtual ~MatrixWeightedGraph () {}

  virtual inline unsigned int getWeight (unsigned int source, unsigned int dest) const {
    int weight = weights[source * numVert + dest];
    if (weight == NO_EDGE) {
      throw std::invalid_argument("No edge");
    }
    else {
      return static_cast<unsigned int> (weight);
    }
  }

  virtual inline bool connected (unsigned int source, unsigned int dest) const {
    int weight = weights[source * numVert + dest];
    if (weight == NO_EDGE) {
      return false;
    }
    else {
      return true;
    }
  }

  virtual inline const std::unordered_map<unsigned int, unsigned int>& getOutgoingEdges (unsigned int vertex) const {
    throw std::runtime_error ("My pretty OO design died, because I need this method to make using the other implementation as fast as possible, but there's no reasonable way to write it here.");
  }

private:
  const int NO_EDGE {-1};

  inline void setWeight(unsigned int source, unsigned int dest, unsigned int weight) {
    weights[source * numVert + dest] = static_cast<int> (weight);
  }
  
  unsigned int numVert;
  std::vector<int> weights;
};





class ListWeightedGraph : public WeightedGraph {
public:

  ListWeightedGraph (unsigned int numNodes, unsigned int numEdges, std::istream& is)
    : weights {}
  {
    for (unsigned int count = 0U; count < numNodes; ++count) {
      weights.emplace_back ();
    }
    for (unsigned int count = 0U; count < numEdges; ++count) {
      unsigned int source, destination, weight;
      is >> source >> destination >> weight;
      weights[source][destination] = static_cast<int> (weight);
    }
  }
  //virtual ~ListWeightedGraph () {}

  virtual inline unsigned int getWeight (unsigned int source, unsigned int dest) const {
    if (weights[source].count (dest) == 1U) {
      return weights[source].at (dest);
    }
    else {
      throw std::invalid_argument ("No such edge.");
    }
  }

  virtual inline bool connected (unsigned int source, unsigned int dest) const {
    if (weights[source].count (dest) == 1U) {
      return true;
    }
    else {
      return false;
    }
  }

  virtual inline const std::unordered_map<unsigned int, unsigned int>& getOutgoingEdges (unsigned int vertex) const {
    return weights[vertex];
  }

private:
  std::vector<std::unordered_map<unsigned int, unsigned int>> weights;
};


class DistanceComparator {
public:
  inline bool operator() (const std::pair<unsigned int, unsigned int>& x,
			  const std::pair<unsigned int, unsigned int>& y) const {
    return x.second > y.second;
  }
};

DistanceComparator comp;


class DistanceTracker {
public:

  void setDistance (unsigned int vertex, unsigned int distance) {
    if (map.count (vertex) == 0 || map[vertex] != distance) {
      map[vertex] = distance;
      pq.emplace_back (vertex, distance);
      std::push_heap (pq.begin (), pq.end (), comp);
    }
  }

  inline bool contains (unsigned int vertex) const {
    return map.count (vertex) == 1;
  }

  inline unsigned int getDistance (unsigned int vertex) const {
    return map.at (vertex);
  }

  std::pair<unsigned int, unsigned int> getLowestDistance () {
    std::pop_heap (pq.begin (), pq.end (), comp);
    std::pair<unsigned int, unsigned int> lowest = pq.back ();
    pq.pop_back ();
    while (map.count (lowest.first) == 0 || map.at (lowest.first) != lowest.second) {
      std::pop_heap (pq.begin (), pq.end (), comp);
      lowest = pq.back ();
      pq.pop_back ();
    }
    map.erase (lowest.first);
    return lowest;
  }

  inline bool empty () const {
    return map.empty ();
  }
  
private:
  std::vector<std::pair<unsigned int, unsigned int>> pq;
  std::unordered_map<unsigned int, unsigned int> map;
};

static bool hasUnvisitedWithDistance (const std::unordered_map<unsigned int, unsigned int>& distances, const std::unordered_set<unsigned int>& unvisited) {
  for (unsigned int uv : unvisited) {
    if (distances.count (uv) == 1) {
      return true;
    }
  }
  return false;
};



/// \note According to a profiler, 61% of my time is spent in here.
static unsigned int findSmallestDistanceUnvisited (const std::unordered_map<unsigned int, unsigned int>& distances, const std::unordered_set<unsigned int>& unvisited) {
  unsigned int bestNode = 0;
  unsigned int bestDistance = std::numeric_limits<unsigned int>::max ();
  for (unsigned int uv : unvisited) {
    if (distances.count (uv) == 1) {
      unsigned int distance = distances.at (uv);
      if (distance < bestDistance) {
	bestDistance = distance;
	bestNode = uv;
      }
    }
  }
  return bestNode;
}




static std::unordered_map<unsigned int, unsigned int> dijkstra (int n, const std::unique_ptr<WeightedGraph>& graph, unsigned int start) {
  std::unordered_map<unsigned int, unsigned int> distances {};
  std::unordered_set<unsigned int> unvisited {};
  for (unsigned int vertex = 0U; vertex < n; ++vertex) {
    unvisited.insert (vertex);
  }
  distances[start] = 0;

  while (hasUnvisitedWithDistance (distances, unvisited)) {
    unsigned int current = findSmallestDistanceUnvisited (distances, unvisited);
    unsigned int distance = distances[current];
    unvisited.erase (current);
    for (unsigned int potentialNeighbor : unvisited) {
      if (graph->connected (current, potentialNeighbor)) {
	unsigned int weight = graph->getWeight (current, potentialNeighbor);
	if (distances.count (potentialNeighbor) == 0 ||
	    distances[potentialNeighbor] > distance + weight) {
	  distances[potentialNeighbor] = distance + weight;
	}
      }
    }
  }
  return distances;
}



static std::unordered_map<unsigned int, unsigned int> dijkstra2 (int n, const std::unique_ptr<WeightedGraph>& graph, unsigned int start) {
  std::unordered_map<unsigned int, unsigned int> completedDistances {};
  DistanceTracker tentativeDistances {};
  std::unordered_set<unsigned int> unvisited {};
  for (unsigned int vertex = 0U; vertex < n; ++vertex) {
    unvisited.insert (vertex);
  }
  tentativeDistances.setDistance (start, 0U);

  
  while (!tentativeDistances.empty ()) {
    std::pair<unsigned int, unsigned int> pair = tentativeDistances.getLowestDistance ();
    unsigned int current = pair.first;
    unsigned int distance = pair.second;
    unvisited.erase (current);
    completedDistances[current] = distance;
    const std::unordered_map<unsigned int, unsigned int>& neighbors = graph->getOutgoingEdges (current);
    for (const std::pair<unsigned int, unsigned int> potential : neighbors) {
      unsigned int potentialNeighbor = potential.first;
      unsigned int weight = potential.second;
      if (unvisited.count (potentialNeighbor) == 1) {
	if (!tentativeDistances.contains (potentialNeighbor) ||
	    tentativeDistances.getDistance (potentialNeighbor) > distance + weight) {
	  tentativeDistances.setDistance (potentialNeighbor, distance + weight);
	}
      }
    }
  }
  return completedDistances;
}




int main () {
  unsigned int n, m, q, s;
  std::cin >> n >> m >> q >> s;
  while (n > 0 || m > 0 || q > 0 || s > 0) {
    std::unique_ptr<WeightedGraph> graph {new ListWeightedGraph (n, m, std::cin)};
 
    std::vector<unsigned int> queries;
    queries.reserve (q);
    for (unsigned int count = 0U; count < q; ++count) {
      unsigned int temp;
      std::cin >> temp;
      queries.push_back (temp);
    }

    std::unordered_map<unsigned int, unsigned int> distances = dijkstra2 (n, graph, s);
      
    for (unsigned int destination : queries) {
      if (distances.count (destination) == 1) {
	std::cout << distances[destination] << "\n";
      }
      else {
	std::cout << "Impossible\n";
      }
    }

    std::cin >> n >> m >> q >> s;
  }
  std::cout.flush ();
  return 0;
}
