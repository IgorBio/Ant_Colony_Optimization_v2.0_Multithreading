#include "console.h"

constexpr size_t kWidth = 60u;

const std::string Console::menu_{
    "\n\u001b[46;1m               \
      CHOOSE AN OPTION:                      \u001b[0m\n \
      \033[39m\033[1;29m1. Change file\n \
      2. Set amount of executions\n \
      3. Solve the traveling salesman problem using\n \
         ant colony optimization without parallelism\n \
      4. Solve the traveling salesman problem using\n \
         ant colony optimization with parallelism\n \
      5. Compare time of execution\n \
      0. Exit\033[0m\n\u001b[46;1m                    \
                                        \u001b[0m\n\n"};

Console::Console() {
  try {
    system("clear");
    FirstItem(filename_);
  } catch (std::exception &e) {
    std::cout << "\u001b[41;1m" << e.what() << "\u001b[0m\n";
    InputFilename();
  }
}

void Console::FirstItem(const std::string &filename) {
  graph_.LoadGraphFromFile(filename);
}

std::pair<TsmResult, Console::Time> Console::ThirdItem() {
  std::vector<TsmResult> results;
  AntAlgorithm algo(graph_);
  algo.SetParallel(false);
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < executions_; ++i) {
    algo.RunAlgorithm();
    results.push_back(algo.GetResult());
  }
  auto end = std::chrono::steady_clock::now();
  std::sort(results.begin(), results.end(),
            [](const TsmResult &a, const TsmResult &b) {
              return a.distance < b.distance;
            });
  return {*results.begin(), end - start};
}

std::pair<TsmResult, Console::Time> Console::FourthItem() {
  std::vector<TsmResult> results;
  AntAlgorithm algo(graph_);
  algo.SetParallel(true);
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < executions_; ++i) {
    algo.RunAlgorithm();
    results.push_back(algo.GetResult());
  }
  auto end = std::chrono::steady_clock::now();
  std::sort(results.begin(), results.end(),
            [](const TsmResult &a, const TsmResult &b) {
              return a.distance < b.distance;
            });
  return {*results.begin(), end - start};
}

std::pair<Console::Time, Console::Time> Console::FifthItem() {
  Time t1, t2;
  AntAlgorithm algo(graph_);
  algo.SetParallel(false);
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < executions_; ++i) {
    algo.RunAlgorithm();
  }
  auto end = std::chrono::steady_clock::now();
  t1 = end - start;

  algo.SetParallel(true);
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < executions_; ++i) {
    algo.RunAlgorithm();
  }
  end = std::chrono::steady_clock::now();
  t2 = end - start;
  return {t1, t2};
}

int Console::InputNumber(const std::string &str) {
  int res;
  while (1) {
    std::cin >> res;
    if (std::cin.fail() or res < 0) {
      system("clear");
      std::cout << GetColor(Color::kRed) << Align("WRONG INPUT!")
                << GetColor(Color::kEnd) << " 🚨"
                << "\n\n";
      std::cout << str << "\n\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      system("clear");
      return res;
    }
  }
}

bool Console::ChooseMenuItem() {
  int choice = InputNumber(menu_);
  bool result = true;

  switch (choice) {
    case 1:
      system("clear");
      InputFilename();
      break;
    case 2:
      system("clear");
      std::cout << GetColor(Color::kCyan)
                << Align("ENTER NUMBER OF EXECUTIONS:") << GetColor(Color::kEnd)
                << "\n\n";
      executions_ = InputNumber(GetColor(Color::kCyan) +
                                Align("ENTER NUMBER OF EXECUTIONS:") +
                                GetColor(Color::kEnd));
      break;
    case 3:
      system("clear");
      try {
        std::pair<TsmResult, Time> tsm = ThirdItem();
        std::cout << GetColor(Color::kMagenta) << RouteToString(tsm)
                  << GetColor(Color::kEnd) << "\n\n";
      } catch (const std::logic_error &e) {
        std::cout << GetColor(Color::kRed) << Align(e.what())
                  << GetColor(Color::kEnd) << " 🚨"
                  << "\n\n";
      }
      break;
    case 4:
      system("clear");
      try {
        std::pair<TsmResult, Time> tsm = FourthItem();
        std::cout << GetColor(Color::kMagenta) << RouteToString(tsm)
                  << GetColor(Color::kEnd) << "\n\n";
      } catch (const std::logic_error &e) {
        std::cout << GetColor(Color::kRed) << Align(e.what())
                  << GetColor(Color::kEnd) << " 🚨"
                  << "\n\n";
      }
      break;
    case 5:
      system("clear");
      try {
        std::pair<Time, Time> t = FifthItem();
        std::string fst = "TIME OF " + std::to_string(executions_) +
                          " EXECUTIONS WITHOUT PARALLELISM: ";
        fst += std::to_string(t.first.count()) + " SEC";
        fst = Align(fst);
        std::string snd = "TIME OF " + std::to_string(executions_) +
                          " EXECUTIONS WITH PARALLELISM: ";
        snd += std::to_string(t.second.count()) + " SEC";
        snd = Align(snd);
        std::cout << GetColor(Color::kMagenta) << fst << GetColor(Color::kEnd)
                  << "\n\n";
        std::cout << GetColor(Color::kMagenta) << snd << GetColor(Color::kEnd)
                  << "\n\n";
      } catch (const std::logic_error &e) {
        std::cout << GetColor(Color::kRed) << Align(e.what())
                  << GetColor(Color::kEnd) << " 🚨"
                  << "\n\n";
      }
      break;
    case 0:
      result = false;
      std::cout << GetColor(Color::kCyan) << Align("HOPE YOU ENJOY!👋")
                << GetColor(Color::kEnd) << "\n\n";
      break;
    default:
      system("clear");
      std::cout << GetColor(Color::kRed) << Align("WRONG INPUT!")
                << GetColor(Color::kEnd) << " 🚨"
                << "\n\n";
      break;
  }
  return result;
}

void Console::PrintChosenFile() {
  size_t idx = filename_.find_last_of('/');
  std::string file = filename_.substr(idx + 1, filename_.size() - idx);
  std::cout << GetColor(Color::kCyan) << Align("SETTINGS")
            << GetColor(Color::kEnd) << std::endl;
  std::cout << "\u001b[38;5;50m " << Align("Loaded file: " + file)
            << "\u001b[0m\n";
}

void Console::PrintExecutions() {
  std::cout << "\u001b[38;5;50m "
            << Align("Number of executions: " + std::to_string(executions_))
            << "\u001b[0m\n";
}

void Console::InputFilename() {
  while (1) {
    std::cout << GetColor(Color::kCyan) << Align("ENTER PATH TO A GRAPH FILE:")
              << GetColor(Color::kEnd) << "\n\n";
    std::string file;
    std::cin >> file;
    system("clear");
    try {
      FirstItem(file);
      filename_ = file;
      break;
    } catch (std::logic_error &e) {
      std::cout << GetColor(Color::kRed) << Align(e.what())
                << GetColor(Color::kEnd) << " 🚨"
                << "\n\n";
      continue;
    }
  }
}

void Console::Run() {
  system("clear");
  bool flag = true;
  while (flag) {
    PrintChosenFile();
    PrintExecutions();
    std::cout << menu_;
    flag = ChooseMenuItem();
  }
}

std::string Console::Align(const std::string &str) {
  std::string aligned;
  std::string addition((kWidth - str.size()) / 2, ' ');
  aligned.append(addition);
  aligned.append(str);
  aligned.append(addition);
  while (aligned.size() < kWidth) aligned.append(" ");
  return aligned;
}

std::string Console::RouteToString(std::pair<TsmResult, Time> &tsm) {
  std::string res = "ROUTE: " + std::to_string(tsm.first.vertices[0]);
  for (size_t i = 1; i < tsm.first.vertices.size(); ++i) {
    res += "->" + std::to_string(tsm.first.vertices[i]);
  }
  res = Align(res) + "\n\n";
  res +=
      Align("LENGTH: " + std::to_string(static_cast<int>(tsm.first.distance))) +
      "\n\n";
  res += Align("TIME: " + std::to_string(tsm.second.count()) + " SEC");
  return res;
}

std::string Console::GetColor(Color color) {
  switch (color) {
    case Color::kRed:
      return "\u001b[41;1m";
    case Color::kGreen:
      return "\u001b[42;1m";
    case Color::kYellow:
      return "\u001b[43;1m";
    case Color::kBlue:
      return "\u001b[44;1m";
    case Color::kMagenta:
      return "\u001b[45;1m";
    case Color::kCyan:
      return "\u001b[46;1m";
    case Color::kGrey:
      return "\u001b[47;1m";
    case Color::kEnd:
      return "\u001b[0m";
    default:
      return "";
  }
}
