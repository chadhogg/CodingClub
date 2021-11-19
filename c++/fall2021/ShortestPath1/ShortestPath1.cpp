/// \file ShortestPath1.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/shortestpath1
/// \note Currently, I don't have this being fast enough.

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

private:
  std::vector<std::unordered_map<unsigned int, unsigned int>> weights;
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




int main () {
  unsigned int n, m, q, s;
  std::cin >> n >> m >> q >> s;
  while (n > 0 || m > 0 || q > 0 || s > 0) {
    std::unique_ptr<WeightedGraph> graph {new MatrixWeightedGraph (n, m, std::cin)};
 
    std::vector<unsigned int> queries;
    queries.reserve (q);
    for (unsigned int count = 0U; count < q; ++count) {
      unsigned int temp;
      std::cin >> temp;
      queries.push_back (temp);
    }

    std::unordered_map<unsigned int, unsigned int> distances = dijkstra (n, graph, s);
      
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
