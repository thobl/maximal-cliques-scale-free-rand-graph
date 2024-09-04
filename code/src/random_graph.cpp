#include "random_graph.hpp"

#include <cmath>
#include <iostream>

#include "random.hpp"

std::vector<std::pair<node, node>> dense_Gnp(unsigned n, double p) {
  std::vector<std::pair<node, node>> edges;

  for (node u = 0; u < n; ++u) {
    for (node v = u + 1; v < n; ++v) {
      if (Random::coin_flip(p)) {
        edges.push_back({u, v});
      }
    }
  }

  return edges;
}

void call_for_node_pairs(unsigned n, std::function<void(node, node)> f) {
  for (node u = 0; u < n; ++u) {
    for (node v = u + 1; v < n; ++v) {
      f(u, v);
    }
  }
}

double expected_nr_edges(unsigned k, Probability prob) {
  double E = 0;
  call_for_node_pairs(k, [&](node u, node v) { E += prob(u, v); });
  return E;
}

std::vector<std::pair<node, node>> generate(unsigned k, Probability prob) {
  std::vector<std::pair<node, node>> edges;

  call_for_node_pairs(k, [&](node u, node v) {
    if (Random::coin_flip(prob(u, v))) {
      edges.push_back({u, v});
    }
  });

  return edges;
}

std::vector<std::pair<node, node>> IRG_core(unsigned long n, double tau,
                                            double weight_lb,
                                            double weight_ub) {
  auto w = power_law_weights(n, tau, weight_lb, weight_ub);
  Probability prob = [&](node u, node v) {
    return std::min(1.0, w[u] * w[v] / n);
  };
  return generate(w.size(), prob);
}

std::vector<std::pair<node, node>> GIRG_core(unsigned long n, double tau,
                                             double T, double weight_lb,
                                             double weight_ub) {
  // the weights
  auto w = power_law_weights(n, tau, weight_lb, weight_ub);
  auto k = w.size();

  // compute desired expectation for number of edges m
  double c = 1.0;
  auto p = [&](node u, node v) { return c * w[u] * w[v] / n; };
  Probability p_IRG = [&](node u, node v) { return std::min(1.0, p(u, v)); };
  double desired_m = expected_nr_edges(k, p_IRG);
  // std::cout << "desired m: " << desired_m << std::endl;

  // find correction factor c for GIRG to get the desired expected m
  // using a binary search
  Probability p_GIRG_no_pos = [&](node u, node v) {
    if (T == 0) {  // threshold case
      return p_IRG(u, v);
    } else if (T == 1) {
      double p_ = p_IRG(u, v);
      return p_ - p(u, v) * std::log(p_);
    } else {
      double p_ = p_IRG(u, v);
      return p_ - std::pow(p(u, v), 1 / T) / (1 / T - 1) *
                      (1 - std::pow(p_, 1 - 1 / T));
    }
  };

  double c1 = 1.0;
  double c2 = 1.0;

  while (expected_nr_edges(k, p_GIRG_no_pos) < desired_m) {
    // std::cout << "desired: " << desired_m
    //           << " expected: " << expected_nr_edges(k, p_GIRG_no_pos)
    //           << " c2: " << c << std::endl;
    c2 *= 2;
    c = c2;
  }
  c = c1;
  while (expected_nr_edges(k, p_GIRG_no_pos) > desired_m) {
    // std::cout << "desired: " << desired_m
    //           << " expected: " << expected_nr_edges(k, p_GIRG_no_pos)
    //           << " c1: " << c << std::endl;
    c1 /= 2;
    c = c1;
  }
  while (c2 - c1 > 0.000001) {
    c = (c1 + c2) / 2;
    // std::cout << "c1: " << c1 << " c2: " << c2 << " c: " << c
    //           << " expected: " << expected_nr_edges(k, p_GIRG_no_pos)
    //           << " desired: " << desired_m << std::endl;
    if (expected_nr_edges(k, p_GIRG_no_pos) < desired_m) {
      c1 = c;
    } else {
      c2 = c;
    }
  }

  // c has been found -> generate graph

  // generate positions
  std::vector<double> pos;
  auto get_pos = [&pos](node v) {
    if (pos.size() < v + 1) {
      pos.resize(v + 1);
      pos[v] = Random::double_in_0_1();
    }
    return pos[v];
  };

  auto dist = [&](node u, node v) {
    double p_u = get_pos(u);
    double p_v = get_pos(v);
    double d_no_wrap = std::abs(p_u - p_v);
    return 2 * std::min(d_no_wrap, 1 - d_no_wrap);
  };

  Probability p_GIRG = [&](node u, node v) {
    if (T > 0) {
      return std::min(1.0, std::pow(p(u, v) / dist(u, v), 1 / T));
    }
    // threshold case
    double c = 0.5;
    return dist(u, v) < p(u, v) ? 1.0 : 0.0;
  };

  return generate(k, p_GIRG);
}

// deterministic power-law weights: n^x / i^x for the ith vertex for
// some positive constant x
std::vector<double> power_law_weights(unsigned long n, double tau,
                                      double lower_bound, double upper_bound) {
  std::vector<double> weights;
  double exponent = -1 / (tau - 1);
  double factor = 1 / std::pow(n, exponent);
  for (unsigned i = 0; i < n; ++i) {
    double w_i = factor * std::pow(i + 1, exponent);
    if (w_i > upper_bound) continue;
    if (w_i < lower_bound) break;
    weights.push_back(w_i);
  }
  return weights;
}
