#include <chrono>
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
class custom_random {
private:
  std::random_device rd;
  std::mt19937 gen;

public:
  custom_random() : gen(rd()) {}
  int get_int(int min, int max) {
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
  }
  double get_real(double min, double max) {
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
  }
};
class gacha {
private:
  std::ifstream f;
  nlohmann::json j;
  custom_random r;
  int gacha_count = 0;

public:
  gacha()
      : f("src/data.json"), j(nlohmann::json::parse(f)), r(custom_random()) {
    if (!f.is_open())
      throw std::runtime_error("Failed to import data");
  }
  void pull(int count, std::chrono::milliseconds wait_time) {
    int R_count = 0;
    for (int i = 0; i < count; i++) {
      std::this_thread::sleep_for(wait_time);
      gacha_count++;
      if (gacha_count >= j["hard_pity"].get<int>()) {
        fmt::print(fg(fmt::color::gold), "SSR!\n");
        gacha_count = 0;
        continue;
      }
      R_count = r.get_int(1, 100);
      if (R_count <= j["R_probability"].get<int>()) {
        fmt::print(fg(fmt::color::dark_red), "R\n");
      } else if (R_count <= j["SR_probability"].get<int>()) {
        fmt::print(fg(fmt::color::blue_violet), "SR\n");
      } else {
        fmt::print(fg(fmt::color::gold), "SSR\n");
      }
    }
  }
};
int main() {
  try {
    std::ifstream f("src/strings.json");
    if (!f.is_open()) {
      fmt::println("Failed to import data");
      return 255;
    }
    nlohmann::json j = nlohmann::json::parse(f);
    bool running = true;
    int input;
    gacha g = gacha();
    fmt::println("{}", j["welcome"].get<std::string>());
    while (running) {
      fmt::print("{}", j["prompt"].get<std::string>());
      if (!(std::cin >> input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        fmt::println("Invaild input");
        continue;
      }
      switch (input) {
      default:
        fmt::println("Invaild input");
        break;
      case 1:
        g.pull(1, std::chrono::milliseconds(10));
        break;
      case 2:
        g.pull(10, std::chrono::milliseconds(100));
        break;
      case 3:
        running = false;
        break;
      }
    }
  } catch (const std::runtime_error &e) {
    fmt::println(e.what());
  }
}
