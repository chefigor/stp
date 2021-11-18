#include "stp_concurrent.hpp"

Switch::Switch(uint32_t bridgeId) : root_id(bridgeId), bridge_id(bridgeId) {
  recvQ = std::make_shared<queue>();
}
void link(Switch& a, Switch& b) {
  a.sendQ.push_back(b.recvQ);
  b.sendQ.push_back(a.recvQ);
}
void Switch::start() {
  while (true) {
    for (auto a : sendQ) {
      a->enqueue(root_id);
    }
    std::array<uint32_t,20> neighbors;
    std::size_t count;
    while ((count = recvQ->try_dequeue_bulk(neighbors.data(), 20)) != 0) {
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

int main() {
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
