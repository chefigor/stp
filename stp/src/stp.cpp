#include "stp.hpp"

uint64_t convert_mac(std::string mac) {
  mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());
  return std::stoul(mac.c_str(), NULL, 16);
}

Switch::Switch(uint32_t bridgeId)
    : bridge_id(bridgeId),
      root_id(bridgeId),
      root_path(bridgeId),
      root_cost(0) {}
Switch::Switch(const std::string &mac) : Switch(convert_mac(mac)) {}
Switch::Switch(std::string &&mac) : Switch(convert_mac(std::move(mac))) {}

uint64_t Switch::getBridgeId() const { return bridge_id; }
uint64_t Switch::getRootId() const { return root_id; }
uint64_t Switch::getRootPath() const { return root_path; }
uint32_t Switch::getRootCost() const { return root_cost; }

void link(const std::shared_ptr<Switch> &a, const std::shared_ptr<Switch> &b,
          uint32_t cost) {
  a->neighbors.emplace_back(b, cost);
  b->neighbors.emplace_back(a, cost);
}

void Switch::start() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint> dist(1, 15);
  uint hello_timer = dist(mt);
  while (!terminate.load()) {
    for (auto &a : neighbors) {
      auto neighbor = a.sw.lock();
      if (!neighbor) continue;
      std::scoped_lock<std::mutex, std::mutex> lock(neighbor->mutex,
                                                    mutex);
      ++messages;
      if (neighbor->root_id < root_id) {
        root_id = neighbor->root_id;
        root_path = neighbor->bridge_id;
        root_cost = neighbor->root_cost + a.cost;
      } else if (neighbor->root_id == root_id &&
                 root_cost > neighbor->root_cost + a.cost) {
        root_path = neighbor->bridge_id;
        root_cost = neighbor->root_cost + a.cost;
      }
      spdlog::info(
          "Switch#{0:d} received rootID {1:d} from Switch#{2:d} :: value of "
          "rootID={3:d} , Root path = Switch#{4:d}",
          bridge_id, neighbor->root_id, neighbor->bridge_id,
          root_id, root_path);
    }

    std::this_thread::sleep_for(std::chrono::seconds(hello_timer));
  }
}

void Switch::startSwitch() {
  terminate.store(false);
  th = std::move(std::thread(&Switch::start, this));
}
void Switch::stopSwitch() {
  terminate.store(true);
  th.join();
}

std::vector<std::weak_ptr<Switch>> Switch::getNeighbors() const {
  std::vector<std::weak_ptr<Switch>> s;
  for (auto sw : neighbors) {
    s.push_back(sw.sw);
  }
  return s;
}
