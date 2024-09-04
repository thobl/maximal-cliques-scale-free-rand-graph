#include "cliques.hpp"

#include <string>

#include "DegeneracyAlgorithm.h"
#include "timer.hpp"

std::string clique_header() { return "nr_cliques,max_clique,time_cliques"; }

std::string compute_cliques(const std::vector<std::list<int>>& adjacency_list) {
  if (adjacency_list.size() == 0) {
    return "0,0,0";
  }
  // auto algorithm = HybridAlgorithm(adjacency_list);
  auto algorithm = DegeneracyAlgorithm(adjacency_list);
  int nr_cliques = 0;
  size_t max_clique = 0;
  auto collect_clique = [&nr_cliques,
                         &max_clique](std::list<int> const& clique) {
    nr_cliques++;
    max_clique = std::max(max_clique, clique.size());
  };
  algorithm.AddCallBack(collect_clique);
  std::list<std::list<int>> cliques;

  Timer::create_timer("cliques");
  Timer::start_timer("cliques");
  // The actual "cliques" parameter seems to be unused
  algorithm.Run(cliques);
  Timer::stop_timer("cliques");

  return std::to_string(nr_cliques) + ',' + std::to_string(max_clique) + ',' +
         std::to_string(Timer::time("cliques").count());
}
