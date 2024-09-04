

#include <fstream>

#include "CLI11.hpp"
#include "app.hpp"
#include "cliques.hpp"
#include "random_graph.hpp"

int main(int argc, char** argv) {
  App app{"Generate a G(n, p)s and compute its cliques."};

  unsigned n = 100;
  app.arg(n, "--nr_nodes,-n",
          "Number of nodes of the generated graph (default: 100).");

  double p = 0.9;
  app.arg(p, "-p", "Connection probability (default: 0.9).");

  app.arg_header("n,p,seed," + clique_header());

  app.arg_seed();

  app.parse(argc, argv);

  // generate graph
  std::vector<std::pair<node, node>> edges = dense_Gnp(n, p);

  // adj list
  std::vector<std::list<int>> adjacency_list(n);
  for (const auto& e : edges) {
    adjacency_list[e.first].push_back(e.second);
    adjacency_list[e.second].push_back(e.first);
  }

  // output parameters
  std::cout << n << "," << p << "," << Random::get_seed() << ','
            << compute_cliques(adjacency_list) << std::endl;

  return 0;
}
