#include "scoremanager.h"
#include <fstream>
#include <algorithm>

//read scores from file and return sorted vector
std::vector<scoreentry> scoremanager::loadscores(const std::string& filename) {
  std::vector<scoreentry> scores;
  std::ifstream file(filename);
  std::string initials;
  int score;

  while (file >> initials >> score) {
    scores.push_back({initials, score});
  }

  std::sort(scores.begin(), scores.end(), [](const scoreentry& a, const scoreentry& b) {
    return b.score < a.score;
  });

  if (scores.size() > 8)
    scores.resize(8);

  return scores;
}

//save new score, keep top 8, overwrite file
void scoremanager::savescore(const std::string& filename, const scoreentry& newentry) {
  std::vector<scoreentry> scores = loadscores(filename);
  scores.push_back(newentry);

  std::sort(scores.begin(), scores.end(), [](const scoreentry& a, const scoreentry& b) {
    return b.score < a.score;
  });

  if (scores.size() > 8)
    scores.resize(8);

  std::ofstream file(filename);
  for (const auto& entry : scores) {
    file << entry.initials << " " << entry.score << "\n";
  }
}
