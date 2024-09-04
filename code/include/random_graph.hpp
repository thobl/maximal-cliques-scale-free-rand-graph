#pragma once

#include <functional>
#include <vector>

typedef unsigned node;
typedef std::function<double(node, node)> Probability;

std::vector<std::pair<node, node>> dense_Gnp(unsigned n, double p);

// Generate an inhomogeneous random graph (IRG) restricted to vertices
// with weight between `core_lb` and `core_ub` with 1 being the
// smallest weight of the full IRG.  One can pass a general
// probability function for the connection probability that gets the
// ids of the vertices (consecutive ints between 0 and n', where n' is
// the number of vertices of the core), the weights of the vertices
// and the total number of vertices (of the full graph).
// std::vector<std::pair<node, node>> generate_general_IRG_core(
//     unsigned long n, double tau, double core_lb, double core_ub,
//     std::function<double(node, node, double, double, unsigned long)> prob);

// Generate the core of a normal IRG (also see `generate_general_IRG_core`).
std::vector<std::pair<node, node>> IRG_core(unsigned long n, double tau,
                                            double weight_lb, double weight_ub);

// Generate the core of 1d-GIRG (also see `generate_general_IRG_core`).
std::vector<std::pair<node, node>> GIRG_core(unsigned long n, double tau,
                                             double T, double weight_lb,
                                             double weight_ub);

std::vector<double> power_law_weights(unsigned long n, double tau,
                                      double lower_bound, double upper_bound);
