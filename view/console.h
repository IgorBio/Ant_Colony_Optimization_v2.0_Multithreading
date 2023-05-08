#ifndef SRC_VIEW_CONSOLE_H
#define SRC_VIEW_CONSOLE_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "../model/ant_algorithm.h"
#include "../model/graph.h"

enum class Color { kRed, kGreen, kBlue, kYellow, kGrey, kCyan, kMagenta, kEnd };

class Console {
 public:
  using Matrix = std::vector<std::vector<int>>;
  using Vector = std::vector<int>;
  using Time = std::chrono::duration<double>;

 public:
  explicit Console();

 public:
  Console(const Console& other) = delete;
  Console(Console&& other) = delete;
  Console& operator=(const Console& other) = delete;
  Console& operator=(Console&& other) = delete;

 public:
  void Run();

 private:
  void FirstItem(const std::string& filename);
  std::pair<TsmResult, Time> ThirdItem();
  std::pair<TsmResult, Time> FourthItem();
  std::pair<Time, Time> FifthItem();
  bool ChooseMenuItem();
  void PrintChosenFile();
  void PrintExecutions();
  void InputFilename();
  int InputNumber(const std::string& str);
  std::string Align(const std::string& str);
  std::string RouteToString(std::pair<TsmResult, Time>& tsm);
  std::string GetColor(Color color);

 private:
  Graph graph_;
  std::string filename_{"./resources/graph_undirected.txt"};
  const static std::string menu_;
  int executions_{1};
};

#endif  // SRC_VIEW_CONSOLE_H
