#pragma once

#include <string>
#include <list>
#include <vector>

std::string clique_header();

std::string compute_cliques(const std::vector<std::list<int>>& adjacency_list);
