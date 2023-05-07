#include "graph.h"

/**
 * @brief Loading a graph from a file in the adjacency matrix format
 *
 * @param filename
 */
void Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) throw std::logic_error("Open file error");
  int vertex_cnt;
  file >> vertex_cnt;
  if (file.fail() or file.peek() != '\n' or vertex_cnt <= 0) {
    throw std::logic_error("Graph size error");
  }
  CreateGraph(vertex_cnt);
  for (int i = 0; i < vertex_cnt; ++i) {
    for (int j = 0; j < vertex_cnt; ++j) {
      if (file.peek() == EOF) {
        throw std::logic_error("Count of edge less than shape of graph");
      }
      std::string str;
      std::stringstream sstr;
      int value;
      file >> str;
      if (!std::all_of(str.begin(), str.end(), [](char ch) {
            return std::isdigit(static_cast<unsigned char>(ch));
          })) {
        throw std::logic_error("Non-digit edge weights in graph");
      }
      sstr << str;
      sstr >> value;
      graph_[i][j] = value;
    }
  }
  if (file.peek() != EOF) {
    throw std::logic_error("Count of edge greater than shape of graph");
  }
  file.close();
}

/**
 * @brief Creates graph member
 *
 * @param size
 */
void Graph::CreateGraph(const int& size) {
  this->graph_ = Matrix(size, std::vector<int>(size));
}

/**
 * @brief Returns graph member
 *
 * @return const Graph::Matrix&
 */
const Graph::Matrix& Graph::GetGraph() const { return this->graph_; }

/**
 * @brief Returns weigth at (vertex1, vertex2) coordinate
 *
 * @param vertex1
 * @param vertex2
 * @return int
 */
int Graph::GetWeight(int idx1, int idx2) const {
  if ((idx1 < 0 or idx1 >= static_cast<int>(graph_.size())) or
      (idx2 < 0 or idx2 >= static_cast<int>(graph_.size())))
    throw std::out_of_range("Graph index out of range");
  return graph_[idx1][idx2];
}
