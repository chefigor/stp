#include "stp_concurrent.hpp"

Switch::Switch(uint32_t bridgeId) : root_id(bridgeId), bridge_id(bridgeId) {}
void link(Switch &a, Switch &b) {
  a.sendQ.push_back(b.recvQ);
  b.sendQ.push_back(a.recvQ);
}
void Switch::startSend() {
  while (!terminate.load()) {
    for (auto a : sendQ) {
      a.get().enqueue(root_id);
    }
    std::this_thread::sleep_for(std::chrono::seconds(hello_timer));
  }
}
void Switch::startRecv() {
  while (!terminate.load()) {
    std::array<uint32_t, 20> neighbors;
    std::size_t count;
    while ((count = recvQ.try_dequeue_bulk(neighbors.data(), 20)) != 0) {
      for (std::size_t i = 0; i != count; ++i) {
        if (neighbors[i] < root_id) root_id = neighbors[i];
        spdlog::info(
            "Switch#{0:d} received rootID {1:d} :: value of rootID={2:d}",
            this->bridge_id, neighbors[i], this->root_id);
      }
    }
    std::this_thread::sleep_for(std::chrono::seconds(hello_timer));
  }
}
void Switch::startSwitch() {
  terminate.store(false);
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint> dist(1, 15);
  hello_timer = dist(mt);
  sendTh = std::move(std::thread(&Switch::startSend, this));
  recvTh = std::move(std::thread(&Switch::startRecv, this));
}
void Switch::stopSwitch() {
  terminate.store(true);
  sendTh.join();
  recvTh.join();
}
uint32_t Switch::getRootId(){return root_id;}
