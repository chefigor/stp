#include <iostream>
#include <sstream>
#include <vector>

#include "concurrentqueue.h"
using queue = moodycamel::ConcurrentQueue<int>;

class Switch {
 public:
  Switch(uint32_t bridgeId) : root_id(bridgeId), bridge_id(bridgeId) {
    recvQ = std::make_shared<queue>();
  }
  friend void link(Switch& a, Switch& b) {
    a.sendQ.push_back(b.recvQ);
    b.sendQ.push_back(a.recvQ);
  }
  void start() {
    while (true) {
      for (auto a : sendQ) {
        a->enqueue(root_id);
      }

      // int neighbor;
      // while (recvQ->try_dequeue(neighbor)) {
      //   if (neighbor < root_id) root_id = neighbor;
      //   std::stringstream ss;
      //   ss << "Switch#" << bridge_id << " received rootID " << neighbor
      //      << std::endl;
      //   std::cout << ss.str();
      // }

      // int neighbors[20];
      // for (std::size_t count = recvQ->try_dequeue_bulk(neighbors, 20);
      //      count != 0; --count) {
      //   if (neighbors[count - 1] < root_id) root_id = neighbors[count - 1];
      //   std::stringstream ss;
      //   ss << "Switch#" << bridge_id << " received rootID "
      //      << neighbors[count - 1] << std::endl;
      //   std::cout << ss.str();
      // }
      int neighbors[20];
      std::size_t count;
      while ((count = recvQ->try_dequeue_bulk(neighbors, 20)) != 0) {
        for (std::size_t i = 0; i != count; ++i) {
          if (neighbors[i] < root_id) root_id = neighbors[i];
          std::stringstream ss;
          ss << "Switch#" << bridge_id << " received rootID " << neighbors[i]
             << " :: value of rootID = " << root_id << std::endl;
          std::cout << ss.str();
        }
      }
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
  }

 private:
  uint32_t root_id;
  uint32_t bridge_id;
  std::shared_ptr<queue> recvQ;
  std::vector<std::shared_ptr<queue>> sendQ;
};

int main() {
  // std::vector<Switch> switches;
  // std::thread threads[10];
  // std::srand(std::time(nullptr));
  // for (int i = 0; i < 10; ++i) switches.emplace_back(std::rand() % 100);
  // for (int i = 0; i < 9; ++i) link(switches[i], switches[i + 1]);
  // for (int i = 0; i < 10; ++i)
  //   threads[i] = std::thread(&Switch::start, &switches[i]);
  // for (int i = 0; i < 10; ++i) threads[i].join();

  Switch a(1000);
  Switch b(300);
  Switch c(400);
  Switch d(800);
  Switch e(100);
  link(a, b);
  link(a, c);
  link(b, d);
  link(b, e);
  link(e, d);
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