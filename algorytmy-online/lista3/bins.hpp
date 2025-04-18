#ifndef BINS_HPP
#define BINS_HPP

#include <vector>

enum Strategy { NEXT_FIT, RANDOM_FIT, FIRST_FIT, BEST_FIT, WORST_FIT };

class Bins {
 private:
  class Bin {
   private:
    static constexpr double MAX_CAPACITY = 1.0;
    std::vector<double> elements;
    double capacity;

   public:
    Bin() : capacity(0.0) {}
    double get_capacity() const { return capacity; }
    double get_free_space() const { return MAX_CAPACITY - capacity; }
    bool add_element(double x);
  };

  std::vector<Bin> bins;
  const Strategy strategy;

  void add_element_next_fit(double x);
  void add_element_random_fit(double x);
  void add_element_first_fit(double x);
  void add_element_best_fit(double x);
  void add_element_worst_fit(double x);

 public:
  Bins(Strategy s) : strategy(s) {}
  void add_element(double x);
  size_t get_no_of_bins() const { return bins.size(); }
};

#endif  // BINS_HPP