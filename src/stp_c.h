#ifndef STP_C_H_
#define STP_C_H_

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
const uint32_t LINK4 = 250;
const uint32_t LINK10 = 100;
const uint32_t LINK16 = 62;
const uint32_t LINK100 = 19;
const uint32_t LINK1000 = 4;
const uint32_t LINK10000 = 2;


class Switch;
struct Link {
  Switch* sw;
  uint32_t cost;
  Link(Switch* sw, uint32_t cost) : sw(sw), cost(cost) {}
};
class Switch {
 public:
  Switch(uint32_t bridgeId);
  friend void link(Switch& a, Switch& b, uint32_t cost);
  void start();
  void startSwitch();
  void stopSwitch();
 public:
  uint32_t messages = 0;
  uint32_t root_cost;
  uint64_t root_path;
  uint64_t root_id;
  uint64_t bridge_id;
  std::vector<Link> neighbors;
  std::thread* th;
  bool terminate;
 private:
 private:
  std::mutex mutex;
};
int test();
uint64_t convert_mac(std::string);

#endif