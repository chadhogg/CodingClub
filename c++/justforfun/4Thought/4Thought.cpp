/// \file 4Thought.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/4thought
/// Just enuemrates all of the possible expressions, since there are only 64 of
///   them.

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

static void simplify (std::vector<std::string>& vec, unsigned int index) {
  int lh = std::stoi (vec.at (index - 1));
  int rh = std::stoi (vec.at (index + 1));
  int result;
  if (vec.at (index) == "*") {
    result = lh * rh;
  }
  else if (vec.at (index) == "/") {
    result = lh / rh;
  }
  else if (vec.at (index) == "+") {
    result = lh + rh;
  }
  else if (vec.at (index) == "-") {
    result = lh - rh;
  }
  else {
    throw std::invalid_argument("Should have been an operator.");
  }
  vec[index - 1] = std::to_string (result);
  vec.erase (vec.begin () + index + 1);
  vec.erase (vec.begin () + index);
}

static int evaluate (const std::vector<std::string> expression) {
  std::vector<std::string> copy {expression};
  unsigned int index = 1U;
  while (index < copy.size () - 1) {
    if (copy.at (index) == "*" || copy.at (index) == "/") {
      simplify (copy, index);
    }
    else {
      index += 2;
    }
  }
  index = 1U;
  while (copy.size () > 1) {
    simplify (copy, index);
  }
  return std::stoi (copy.front ());
}

static std::string concatenate (const std::vector<std::string>& expr, int result) {
  std::ostringstream os;
  for (std::string str : expr) {
    os << str << " ";
  }
  os << "= " << result;
  return os.str ();
}

static void enumerate (std::unordered_map<int, std::string>& map) {
  for (unsigned int x = 0U; x < 4 * 4 * 4; ++x) {
    std::vector<std::string> expr {};
    expr.push_back ("4");
    switch (x % 4) {
    case 0: expr.push_back ("+"); break;
    case 1: expr.push_back ("-"); break;
    case 2: expr.push_back ("*"); break;
    case 3: expr.push_back ("/"); break;
    }
    expr.push_back ("4");
    switch ((x / 4) % 4) {
    case 0: expr.push_back ("+"); break;
    case 1: expr.push_back ("-"); break;
    case 2: expr.push_back ("*"); break;
    case 3: expr.push_back ("/"); break;
    }
    expr.push_back ("4");
    switch ((x / 16) % 4) {
    case 0: expr.push_back ("+"); break;
    case 1: expr.push_back ("-"); break;
    case 2: expr.push_back ("*"); break;
    case 3: expr.push_back ("/"); break;
    }
    expr.push_back ("4");
    int result = evaluate (expr);
    map[result] = concatenate (expr, result);
  }
}

int main () {
  std::unordered_map<int, std::string> map {};
  enumerate (map);
  int count, next;
  std::cin >> count;
  for(unsigned int i = 0U; i < count; ++i) {
    std::cin >> next;
    if (map.count (next) == 1U) {
      std::cout << map.at (next) << "\n";
    }
    else {
      std::cout << "no solution\n";
    }
  }
  std::cout.flush ();
}
