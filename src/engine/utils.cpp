#include <cstdlib>
#include <ctime>
#include <string>
#include "engine/utils.hpp"

using namespace Utils;

std::string Utils::uuid() {
  std::srand(std::time(nullptr) * std::rand());
  std::string uuid = std::to_string(std::rand());
  return uuid;
}
