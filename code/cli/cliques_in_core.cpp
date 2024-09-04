

#include <fstream>
#include <list>
#include <string>

#include "CLI11.hpp"
#include "app.hpp"
#include "cliques.hpp"
#include "random_graph.hpp"

int main(int argc, char** argv) {
  App app{
      "Generate the dense core of an IRG/GIRG and compute its cliques.  It "
      "generates the graph induced on the vertices with weight above the given "
      "bound.  The weights are deterministic power-law weights with minimum "
      "weight 1."};

  std::string model = "girg";
  app.arg(model, "--model", "The model that should be used.", {"girg", "irg"});

  unsigned long n = 100;
  app.arg(n, "--nr_nodes,-n",
          "Number of nodes of the generated graph (default: 100).");

  double tau = 2.5;
  app.arg(tau, "--tau", "Power-law exponent (default: 2.5).");

  double T = 0.0;
  app.arg(T, "--temperature,-T", "The temperature (default: 0.0).");

  double core_lb = 5;
  app.arg(core_lb, "--core_lb",
          "Weight bound for the core; only vertices of higher weight will be "
          "included in the graph (default: 5).");

  double core_ub = 10;
  app.arg(core_ub, "--core_ub",
          "Weight bound for the core; only vertices of lower weight will be "
          "included in the graph (default: 10).");

  app.arg_header("model,n,m,tau,T,core_lb,core_ub,core_size,seed," +
                 clique_header());

  app.arg_seed();

  app.parse(argc, argv);

  // generate the graph
  std::vector<std::pair<node, node>> edges;

  if (model == "girg") {
    edges = GIRG_core(n, tau, T, core_lb, core_ub);
  } else {
    edges = IRG_core(n, tau, core_lb, core_ub);
  }

  // number of nodes in the component
  unsigned k = 0;
  for (const auto& e : edges) {
    k = std::max(std::max(k, e.first), e.second);
  }
  k++;

  // adj list
  std::vector<std::list<int>> adjacency_list(k);
  for (const auto& e : edges) {
    adjacency_list[e.first].push_back(e.second);
    adjacency_list[e.second].push_back(e.first);
  }

  // output parameters
  std::cout << model << ',' << n << ',' << edges.size() << ',' << tau << ','
            << T << ',' << core_lb << ',' << core_ub << ',' << k << ','
            << Random::get_seed() << ',' << compute_cliques(adjacency_list)
            << std::endl;

  return 0;
}
