#include "bins.hpp"

#include <random>

bool Bins::Bin::add_element(double x) {
  if (get_capacity() + x <= MAX_CAPACITY) {
    elements.push_back(x);
    capacity += x;
    return true;
  }
  return false;
}

void Bins::add_element_next_fit(double x) {
  if (bins.size() > 0 && bins[bins.size() - 1].get_free_space() >= x) {
    bins[bins.size() - 1].add_element(x);
  } else {
    bins.push_back(Bin());
    bins[bins.size() - 1].add_element(x);
  }
}

void Bins::add_element_random_fit(double x) {
  std::vector<size_t> available_bins;
  for (size_t i = 0; i < bins.size(); i++) {
    if (bins[i].get_free_space() >= x) {
      available_bins.push_back(i);
    }
  }
  if (available_bins.size() == 0) {
    bins.push_back(Bin());
    bins[bins.size() - 1].add_element(x);
  } else {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<size_t> u(0, available_bins.size() - 1);
    bins[available_bins[u(rng)]].add_element(x);
  }
}

void Bins::add_element_first_fit(double x) {
  size_t i = 0;
  while (i < bins.size() && bins[i].get_free_space() < x) {
    i++;
  }
  if (i == bins.size()) {
    bins.push_back(Bin());
    bins[i].add_element(x);
  } else {
    bins[i].add_element(x);
  }
}

void Bins::add_element_best_fit(double x) {
  std::vector<size_t> available_bins;
  for (size_t i = 0; i < bins.size(); i++) {
    if (bins[i].get_free_space() >= x) {
      available_bins.push_back(i);
    }
  }
  if (available_bins.size() == 0) {
    bins.push_back(Bin());
    bins[bins.size() - 1].add_element(x);
  } else {
    size_t best_index = 0;
    double best_free_space = bins[available_bins[0]].get_free_space();
    for (size_t i = 0; i < available_bins.size(); i++) {
      if (bins[available_bins[i]].get_free_space() < best_free_space) {
        best_index = i;
        best_free_space = bins[available_bins[i]].get_free_space();
      }
    }
    bins[available_bins[best_index]].add_element(x);
  }
}

void Bins::add_element_worst_fit(double x) {
  std::vector<size_t> available_bins;
  for (size_t i = 0; i < bins.size(); i++) {
    if (bins[i].get_free_space() >= x) {
      available_bins.push_back(i);
    }
  }
  if (available_bins.size() == 0) {
    bins.push_back(Bin());
    bins[bins.size() - 1].add_element(x);
  } else {
    size_t worst_index = 0;
    double worst_free_space = bins[available_bins[0]].get_free_space();
    for (size_t i = 0; i < available_bins.size(); i++) {
      if (bins[available_bins[i]].get_free_space() > worst_free_space) {
        worst_index = i;
        worst_free_space = bins[available_bins[i]].get_free_space();
      }
    }
    bins[available_bins[worst_index]].add_element(x);
  }
}

void Bins::add_element(double x) {
  switch (strategy) {
    case NEXT_FIT:
      add_element_next_fit(x);
      break;
    case RANDOM_FIT:
      add_element_random_fit(x);
      break;
    case FIRST_FIT:
      add_element_first_fit(x);
      break;
    case BEST_FIT:
      add_element_best_fit(x);
      break;
    case WORST_FIT:
      add_element_worst_fit(x);
  }
}
