#ifndef SRC_ANT_HEURISTICS_H
#define SRC_ANT_HEURISTICS_H

/**
 * @brief Hyperparameters for Ant colony optimization algorithm
 *
 */
struct Heuristics {
  const double kAlpha = 1.0;
  const double kBeta = 1.0;
  const double kQ = 4.0;
  const double kTau = 0.2;
  const double kRo = 0.5;
  const size_t kColonies = 100;
  const size_t kBypassCount = 10;
};

#endif  // SRC_ANT_HEURISTICS_H
