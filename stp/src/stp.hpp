#include <algorithm>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
static constexpr uint32_t LINK4 = 250;
static constexpr uint32_t LINK10 = 100;
static constexpr uint32_t LINK16 = 62;
static constexpr uint32_t LINK100 = 19;
static constexpr uint32_t LINK1000 = 4;
static constexpr uint32_t LINK10000 = 2;

class Switch;
struct Link {
  Switch* sw;
  uint32_t cost;
  Link() = default;
  Link(Switch* sw, uint32_t cost) : sw(sw), cost(cost) {}
};
class Switch {
 public:
  Switch(uint32_t bridgeId);
  Switch(const std::string& mac);
  Switch(std::string&& mac);
  friend void link(Switch& a, Switch& b, uint32_t cost);
  void start();
  void startSwitch();
  void stopSwitch();

 private:
  uint32_t messages = 0;
  uint64_t bridge_id;
  uint64_t root_id;
  uint64_t root_path;
  uint32_t root_cost;
  std::vector<Link> neighbors;
  std::thread th;
  std::atomic_bool terminate{false};
  std::mutex mutex;
};

uint64_t convert_mac(std::string);
int test();