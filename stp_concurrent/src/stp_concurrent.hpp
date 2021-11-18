#include <iostream>
#include <sstream>
#include <vector>

#include "concurrentqueue.hpp"
using queue = moodycamel::ConcurrentQueue<int>;

class Switch {
 public:
  Switch(uint32_t bridgeId);
  friend void link(Switch& a,Switch& b);
  void start();
 private:
  uint32_t root_id;
  uint32_t bridge_id;
  std::shared_ptr<queue> recvQ;
  std::vector<std::shared_ptr<queue>> sendQ;
};