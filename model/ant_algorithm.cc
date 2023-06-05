#include "ant_algorithm.h"

/**
 * @brief Ant class constructor. Initializes an Ant object with the given graph
 * and starting vertex.
 * @param graph A reference to a Graph object containing the graph that the ant
 * will traverse.
 * @param start The index of the starting vertex for the ant's path.
 */
Ant::Ant(const Graph &graph, size_t start)
    : graph_{graph}, tabu_{Tabu(graph.GetGraph().size())} {
  path_.push_back(start);
  tabu_[start] = true;
}

/**
 * @brief Simulates one pass through all vertices in the graph using pheromones
 * to choose the next path.
 *
 * @param pheromones A reference to the Pheromones object containing pheromone
 * values.
 */
void Ant::MakeBypass(const Pheromones &pheromones) {
  for (size_t i = 0; i < graph_.GetGraph().size(); ++i) {
    int next = ChooseNextPath(pheromones, path_.back());
    if (next != -1) {
      visited_.emplace_back(path_.back(), next);
      path_.push_back(next);
      tabu_[next] = true;
    }
  }
  path_.push_back(path_.front());
}

/**
 * @brief Chooses the next path for an ant to travel on based on the provided
 * pheromone levels and the current path.
 *
 * @param pheromones The pheromone levels for each path in the graph.
 * @param path The index of the current vertex in the graph.
 *
 * @return the index of the next path to be taken, or -1 if no path is
 * available.
 */
int Ant::ChooseNextPath(const Pheromones &pheromones, size_t path) {
  const Chances chances = GetChances(pheromones, path);
  const double rnd = RandomChoice(0.0, 1.0);
  for (size_t i = 1; i < chances.size(); ++i) {
    if (!tabu_[i - 1] and rnd < chances[i]) return i - 1;
  }
  return -1;
}

/**
 * @brief Calculates the probability of each unvisited neighbor vertex for an
 * ant to move to from the current vertex.
 *
 * @param pheromones The pheromone levels for each path in the graph.
 * @param path The index of the current vertex in the graph.
 *
 * @return A vector representing the cumulative sum of probabilities of moving
 * to each unvisited neighbor vertex.
 */
Ant::Chances Ant::GetChances(const Pheromones &pheromones, size_t path) {
  const auto &graph = graph_.GetGraph();
  Chances chances(graph.size(), std::numeric_limits<double>::infinity());
  double total = 0.0;

  for (size_t i = 0; i < graph.size(); ++i) {
    if (graph[path][i] and !tabu_[i]) {
      chances[i] = std::pow(pheromones[path][i], consts_.kAlpha) *
                   std::pow(1.0 / graph[path][i], consts_.kBeta);
      total += chances[i];
    }
  }

  std::transform(chances.begin(), chances.end(), chances.begin(),
                 [total](double chance) {
                   return !std::isinf(chance) ? chance / total : 0;
                 });

  Chances cumulate(chances.size() + 1);
  std::partial_sum(chances.begin(), chances.end(), cumulate.begin() + 1);

  return cumulate;
}

/**
 * @brief Returns a random double value within the given range [min, max].
 *
 * @param min The lower bound of the range.
 * @param max The upper bound of the range.
 * @return A random double value
 */
double Ant::RandomChoice(const double min, const double max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(min, max);
  return dist(gen);
}

/**
 * @brief Computes the total distance traveled by an ant along its current path.
 *
 * @return A double value representing the total distance traveled by the ant.
 */
double Ant::GetDistance() const {
  double distance = 0.0;
  for (size_t i = 1; i < path_.size(); ++i) {
    distance += graph_.GetWeight(path_[i - 1], path_[i]);
  }
  return distance;
}

/**
 * @brief Returns the path followed by an ant.
 *
 * @return The path member.
 */
Ant::Path Ant::GetPath() const { return path_; }

/**
 * @brief Checks if a given vertex has already been visited by the ant.
 *
 * @param i The row index of the cell to check.
 * @param j The column index of the cell to check.
 * @return True if the vertex has already been visited, false otherwise.
 */
bool Ant::IsVisited(size_t i, size_t j) const {
  return std::any_of(visited_.cbegin(), visited_.cend(),
                     [i, j](std::pair<size_t, size_t> v) {
                       return v.first == i and v.second == j;
                     });
}

/**
 * @brief Constructs an AntAlgorithm object with the given graph.
 *
 * @param graph A reference to the graph on which the ant algorithm is to be
 * performed.
 */
AntAlgorithm::AntAlgorithm(const Graph &graph)
    : graph_{graph}, parallel_{false} {}

/**
 * @brief Runs the ant colony optimization algorithm on loaded graph.
 *
 */
void AntAlgorithm::RunAlgorithm() {
  TsmResult best_result{Path{}, std::numeric_limits<double>::infinity()};
  for (size_t colony = 0; colony < consts_.kColonies; ++colony) {
    RunColony();
    if (result_.distance < best_result.distance) {
      best_result = std::move(result_);
    }
  }
  result_ = std::move(best_result);
}

/**
 * @brief Simulates pass of the graph by colony of ants.
 *
 */
void AntAlgorithm::RunColony() {
  result_ = TsmResult{Path{}, std::numeric_limits<double>::infinity()};
  pheromones_ =
      Pheromones(graph_.GetGraph().size(),
                 std::vector<double>(graph_.GetGraph().size(), consts_.kTau));

  for (size_t i = 0; i < consts_.kBypassCount; ++i) {
    parallel_ ? RunAntsParallel() : RunAnts();
    UpdateResult();
    UpdatePheromones();
  }
}

/**
 * @brief Simulates pass of the graph by each ant of a colony.
 *
 */
void AntAlgorithm::RunAnts() {
  ants_ = Ants(graph_.GetGraph().size());
  for (size_t i = 0; i < ants_.size(); ++i) {
    ants_[i] = std::make_unique<Ant>(graph_, i);
    ants_[i]->MakeBypass(pheromones_);
  }
}

/**
 * @brief Simulates parallel pass of the graph by each ant of a colony.
 *
 */
void AntAlgorithm::RunAntsParallel() {
  ants_.clear();
  Threads threads;
  for (size_t i = 0; i < graph_.GetGraph().size(); ++i) {
    threads.emplace_back(&AntAlgorithm::ParallelBypass, this, i);
  }
  for (auto &thread : threads) {
    thread.join();
  }
}

/**
 * @brief Executes the Ant's MakeBypass method in parallel for a given vertex.
 *
 * @param vertex The start vertex for the ants' bypass.
 */
void AntAlgorithm::ParallelBypass(const size_t vertex) {
  auto ant = std::make_unique<Ant>(graph_, vertex);
  ant->MakeBypass(pheromones_);

  std::lock_guard<std::mutex> lock(mtx_);
  ants_.emplace_back(std::move(ant));
}

/**
 * @brief Updates pheromone levels on all paths based on ant tours.
 *
 */
void AntAlgorithm::UpdatePheromones() {
  const size_t vertex_cnt = graph_.GetGraph().size();
  for (size_t i = 0; i < vertex_cnt; ++i) {
    for (size_t j = i + 1; j < vertex_cnt; ++j) {
      for (const auto &ant : ants_) {
        if (ant->IsVisited(i, j)) {
          pheromones_[i][j] += (1.0 - consts_.kRo) * pheromones_[i][j] +
                               consts_.kQ / ant->GetDistance();
          pheromones_[j][i] = pheromones_[i][j];
        }
      }
    }
  }
}

/**
 * @brief Updates the current best TSP path found by the ant algorithm.
 *
 */
void AntAlgorithm::UpdateResult() {
  std::sort(ants_.begin(), ants_.end(), [](auto &x, auto &y) {
    return x->GetDistance() < y->GetDistance();
  });
  const auto &path = ants_.front();
  TsmResult result{path->GetPath(), path->GetDistance()};
  if (result.distance < result_.distance) {
    result_ = std::move(result);
  }
}

/**
 * @brief Returns the result of the Ant Algorithm.
 *
 * @return TsmResult - The solution of the traveling salesman problem found by
 * the ant colony algorithm.
 */
TsmResult AntAlgorithm::GetResult() const {
  if (!CheckResult()) {
    throw std::logic_error(
        "It is impossible to solve the problem with a given graph");
  }
  TsmResult result = result_;
  std::for_each(result.vertices.begin(), result.vertices.end(),
                [](int &v) { ++v; });
  return result;
}

/**
 * @brief Checks if the result obtained after running the Ant Algorithm is
 * valid.
 *
 * @return True if the result contains all the vertices of the graph, false
 * otherwise.
 */
bool AntAlgorithm::CheckResult() const {
  const auto &graph = graph_.GetGraph();
  Path vertices(graph.size());
  std::iota(vertices.begin(), vertices.end(), 0);

  return std::all_of(vertices.cbegin(), vertices.cend(), [&](const auto &v) {
    return std::find(result_.vertices.cbegin(), result_.vertices.cend(), v) !=
           result_.vertices.cend();
  });
}

/**
 * @brief Sets the parallel flag for the AntAlgorithm.
 *
 * @param parallel A boolean value indicating whether the algorithm should run
 * in parallel.
 */
void AntAlgorithm::SetParallel(bool parallel) { parallel_ = parallel; }
