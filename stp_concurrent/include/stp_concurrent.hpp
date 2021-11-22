#include <atomic>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "moodycamel/concurrentqueue.hpp"
#include "spdlog/spdlog.h"
using queue = moodycamel::ConcurrentQueue<uint32_t>;

class Switch {
 public:
  Switch(uint32_t bridgeId);
  friend void link(Switch &a, Switch &b);
  void startSwitch();
  void stopSwitch();

 private:
  void startSend();
  void startRecv();
  uint32_t root_id;
  uint32_t bridge_id;
  uint hello_timer;
  std::atomic_bool terminate{false};
  std::thread recvTh;
  std::thread sendTh;
  queue recvQ;
  std::vector<std::reference_wrapper<queue>> sendQ;
};