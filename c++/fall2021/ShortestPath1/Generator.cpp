/// \file Generator.cpp
/// \author Chad Hogg
/// \brief A program to generate difficult problems.

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>

int main (int argc, char* argv[]) {
  int maxvert = 10000;
  int maxedge = 30000;
  int maxquer = 100;
  if (argc == 4) {
    maxvert = atoi (argv[1]);
    maxedge = atoi (argv[2]);
    maxquer = atoi (argv[3]);
  }
  srand (time (NULL));
  int vertices = rand () % maxvert;
  int edgeNum = rand () % std::min (maxedge, vertices * (vertices - 1));
  int queries = rand () % maxquer;
  int start = rand () % vertices;

  std::cout << vertices << " " << edgeNum << " " << queries << " " << start << "\n";

  unsigned int printed = 0U;
  std::unordered_map<unsigned int, std::unordered_set<unsigned int>> edges;
  for (unsigned int vertex = 0U; vertex < vertices; ++vertex) {
    edges.emplace (std::make_pair(vertex, std::unordered_set<unsigned int>()));
  }
  while (printed < edgeNum) {
    int a = rand () % vertices;
    int b = rand () % vertices;
    int c = rand () % 1000;
    if (a != b && edges.at (a).count (b) == 0) {
      edges.at (a).insert (b);
      std::cout << a << " " << b << " " << c << "\n";
      ++printed;
    }
  }

  for (unsigned int q = 0U; q < queries; ++q) {
    std::cout << rand () % vertices << "\n";
  }

  std::cout << "0 0 0 0\n";

  std::cout.flush ();
  return 0;
}
