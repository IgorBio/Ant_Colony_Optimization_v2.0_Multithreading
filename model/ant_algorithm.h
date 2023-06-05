#ifndef PARALLELS_ANT_MODEL_ANT_ALGORITHM_H_
#define PARALLELS_ANT_MODEL_ANT_ALGORITHM_H_

#include <algorithm>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "graph.h"
#include "heuristics.h"

struct TsmResult {
  std::vector<int> vertices;
  double distance = 0.0;
};

class Ant {
 public:
  using Path = std::vector<int>;
  using Tabu = std::vector<bool>;
  using Visited = std::vector<std::pair<size_t, size_t>>;
  using Pheromones = std::vector<std::vector<double>>;
  using Chances = std::vector<double>;

  Ant(const Graph &, size_t);

  void MakeBypass(const Pheromones &);
  double GetDistance() const;
  Path GetPath() const;
  bool IsVisited(size_t, size_t) const;

 private:
  int ChooseNextPath(const Pheromones &, size_t);
  Chances GetChances(const Pheromones &, size_t);
  double RandomChoice(const double, const double);

 private:
  const Graph &graph_;
  Tabu tabu_;
  Path path_;
  Visited visited_;
  Heuristics consts_;
};

class AntAlgorithm {
 public:
  using Path = std::vector<int>;
  using Ants = std::vector<std::unique_ptr<Ant>>;
  using Threads = std::vector<std::thread>;
  using Pheromones = std::vector<std::vector<double>>;
  using Chances = std::vector<double>;

 public:
  explicit AntAlgorithm(const Graph &);

  void RunAlgorithm();
  TsmResult GetResult() const;
  void SetParallel(const bool);

 private:
  void RunColony();
  void RunAnts();
  void RunAntsParallel();
  void UpdatePheromones();
  void UpdateResult();
  bool CheckResult() const;
  void ParallelBypass(size_t);

 private:
  const Graph &graph_;
  TsmResult result_;
  Ants ants_;
  Pheromones pheromones_;
  Heuristics consts_;
  std::mutex mtx_;
  bool parallel_;
};

#endif  // PARALLELS_ANT_MODEL_ANT_ALGORITHM_H_
