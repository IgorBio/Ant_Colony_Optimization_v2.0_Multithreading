#ifndef SRC_ANT_GRAPH_H
#define SRC_ANT_GRAPH_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Graph {
 public:
  using Matrix = std::vector<std::vector<int>>;

  void LoadGraphFromFile(const std::string& filename);

  const Matrix& GetGraph() const;
  int GetWeight(int vertex1, int vertex2) const;

 private:
  void CreateGraph(const int& size);

 private:
  Matrix graph_;
};

#endif  // SRC_ANT_GRAPH_H
