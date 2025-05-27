
#pragma once
#include <vector>
#include <string>

struct scoreentry {
  std::string initials;
  int score;
};

class scoremanager {
public:
  static std::vector<scoreentry> loadscores(const std::string& filename); //load scores from file
  static void savescore(const std::string& filename, const scoreentry& newentry); //save a new score to file
};
