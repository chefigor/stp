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

Switch::Switch(const std::string& mac) : Switch(convert_mac(mac)) {}
Switch::Switch(std::string&& mac) : Switch(convert_mac(std::move(mac))) {}

void link(const std::shared_ptr<Switch>& a, const std::shared_ptr<Switch> b,
          uint32_t cost) {
  a->neighbors.emplace_back(b, cost);
  b->neighbors.emplace_back(a, cost);
}

void Switch::start() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint> dist(0, 15);
  uint hello_timer = dist(mt);
  while (!terminate.load()) {
    for (auto& a : neighbors) {
      auto neighbor = a.sw.lock();
      if (!neighbor) continue;
      std::scoped_lock<std::mutex, std::mutex> lock(neighbor->mutex,
                                                    this->mutex);
      ++messages;
      if (neighbor->root_id < this->root_id) {
        this->root_id = neighbor->root_id;
        this->root_path = neighbor->bridge_id;
        this->root_cost = neighbor->root_cost + a.cost;
      } else if (neighbor->root_id == this->root_id &&
                 this->root_cost > neighbor->root_cost + a.cost) {
        this->root_path = neighbor->bridge_id;
        this->root_cost = neighbor->root_cost + a.cost;
      }

      std::stringstream ss;
      ss << "Switch#" << this->bridge_id << " received rootID "
         << neighbor->root_id << " from Switch#" << neighbor->bridge_id
         << " :: value of rootID = " << this->root_id << " , Root path = Switch"
         << this->root_path << std::endl;

      std::cout << ss.str();
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

uint64_t Switch::getBridgeId() const { return bridge_id; }
uint64_t Switch::getRootId() const { return root_id; }
uint64_t Switch::getRootPath() const { return root_path; }
uint32_t Switch::getRootCost() const { return root_cost; }
std::vector<std::weak_ptr<Switch>> Switch::getNeighbors() const {
  std::vector<std::weak_ptr<Switch>> s;
  for (auto sw : neighbors) {
    s.push_back(sw.sw);
  }
  return s;
}

int main() { test(); }

int test() {
  /*                            19           62
                 root        |-------SW4(d)------|
    SW3(c)------SW1(a)------SW2(b)     |19      SW6(f)
            4            4   |-------SW5(e)------|
                                62          62
  */
  auto a = std::make_shared<Switch>("00:00:00:00:00:01");
  auto b = std::make_shared<Switch>("00:00:00:00:00:02");
  auto c = std::make_shared<Switch>("00:00:00:00:00:03");
  auto d = std::make_shared<Switch>("00:00:00:00:00:04");
  auto e = std::make_shared<Switch>("00:00:00:00:00:05");
  auto f = std::make_shared<Switch>("00:00:00:00:00:10");

  link(a, b, LINK1000);
  link(a, c, LINK1000);
  link(b, d, LINK100);
  link(b, e, LINK16);
  link(e, d, LINK100);
  link(e, f, LINK16);
  link(d, f, LINK16);

  a->startSwitch();
  b->startSwitch();
  c->startSwitch();
  d->startSwitch();
  e->startSwitch();
  f->startSwitch();

  std::this_thread::sleep_for(std::chrono::seconds(10));

  a->stopSwitch();
  b->stopSwitch();
  c->stopSwitch();
  d->stopSwitch();
  e->stopSwitch();
  f->stopSwitch();

  // std::cout << "Root paths : " << std::endl;
  // std::cout << "Switch#" << a.bridge_id << " root path Switch#" <<
  // a.root_path
  //           << " cost: " << a.root_cost << " :: Root switch" << a.root_id
  //           << std::endl;
  // std::cout << "Switch#" << b.bridge_id << " root path Switch#" <<
  // b.root_path
  //           << " cost: " << b.root_cost << " :: Root switch" << b.root_id
  //           << std::endl;
  // std::cout << "Switch#" << c.bridge_id << " root path Switch#" <<
  // c.root_path
  //           << " cost: " << c.root_cost << " :: Root switch" << c.root_id
  //           << std::endl;
  // std::cout << "Switch#" << d.bridge_id << " root path Switch#" <<
  // d.root_path
  //           << " cost: " << d.root_cost << " :: Root switch" << d.root_id
  //           << std::endl;
  // std::cout << "Switch#" << e.bridge_id << " root path Switch#" <<
  // e.root_path
  //           << " cost: " << e.root_cost << " :: Root switch" << e.root_id
  //           << std::endl;
  // std::cout << "Switch#" << f.bridge_id << " root path Switch#" <<
  // f.root_path
  //           << " cost: " << f.root_cost << " :: Root switch" << f.root_id
  //           << std::endl;
  // std::cout << "Total messages exchanged : "
  //           << a.messages + b.messages + c.messages + d.messages + e.messages
  //           +
  //                  f.messages
  //           << std::endl;
  return 0;
}
