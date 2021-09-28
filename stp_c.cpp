#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

struct uint48{
  uint64_t x:48;
};
class Switch {
 public:
  Switch(uint32_t bridgeId) : root_id(bridgeId), bridge_id(bridgeId) {}
  friend void link(Switch& a, Switch& b) {
    a.neighbors.push_back(&b);
    b.neighbors.push_back(&a);
  }
  void start() {
    while (true) {
      for (auto a : neighbors) {
        std::scoped_lock<std::mutex,std::mutex> lock(a->mutex, this->mutex);
        if (a->root_id < this->root_id) this->root_id = a->root_id;

        std::stringstream ss;
        ss << "Switch#" << this->bridge_id << " received rootID " << a->root_id
           << " from Switch#" << a->bridge_id
           << " :: value of rootID = " << this->root_id << std::endl;

        std::cout << ss.str();
      }
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
  }

 private:
  uint32_t root_id;
  uint32_t bridge_id;
  std::vector<Switch*> neighbors;
  std::mutex mutex;
};

int main() {
  Switch a(1);
  Switch b(2);
  Switch c(3);
  Switch d(4);
  Switch e(5);
  link(a, b);
  link(a, c);
  link(b, d);
  link(b, e);
  link(e, d);
  /*
                |-------SW4
    SW3--SW1--SW2        |
                |-------SW5
  */
  std::thread s1(&Switch::start, &a);
  std::thread s2(&Switch::start, &b);
  std::thread s3(&Switch::start, &c);
  std::thread s4(&Switch::start, &d);
  std::thread s5(&Switch::start, &e);

  s1.join();
  s2.join();
  s3.join();
  s4.join();
  s5.join();

  return 0;
}