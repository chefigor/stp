#include "stp_c.h"
bool a = true;
uint64_t convert_mac(std::string mac) {
  mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());
  return strtoul(mac.c_str(), NULL, 16);
}
Switch::Switch(uint32_t bridgeId)
    : root_id(bridgeId),
      bridge_id(bridgeId),
      root_path(bridgeId),
      root_cost(0) {}

void link(Switch& a, Switch& b, uint32_t cost) {
  a.neighbors.emplace_back(&b, cost);
  b.neighbors.emplace_back(&a, cost);
}

void Switch::start() {
  while (!terminate) {
    for (auto a : neighbors) {
      std::scoped_lock<std::mutex, std::mutex> lock(a.sw->mutex, this->mutex);
      messages++;
      if (a.sw->root_id < this->root_id) {
        this->root_id = a.sw->root_id;
        this->root_path = a.sw->bridge_id;
        this->root_cost = a.sw->root_cost + a.cost;
      } else if (a.sw->root_id == this->root_id &&
                 this->root_cost > a.sw->root_cost + a.cost) {
        this->root_id = a.sw->root_id;
        this->root_path = a.sw->bridge_id;
        this->root_cost = a.sw->root_cost + a.cost;
      }

      std::stringstream ss;
      ss << "Switch#" << this->bridge_id << " received rootID " << a.sw->root_id
         << " from Switch#" << a.sw->bridge_id
         << " :: value of rootID = " << this->root_id << " , Root path = Switch"
         << this->root_path << std::endl;

      std::cout << ss.str();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

void Switch::startSwitch() {
  terminate = false;
  th = new std::thread(&Switch::start, this);
  // return new std::thread(&Switch::start,this);
}
void Switch::stopSwitch() {
  terminate = true;
  th->join();
}
int main() { test(); }

int test() {
  /*                            19           62
                 root        |-------SW4(d)------|
    SW3(c)------SW1(a)------SW2(b)     |19      SW6(f)
            4            4   |-------SW5(e)------|
                                62          62
  */

  Switch a(convert_mac("00:00:00:00:00:01"));
  Switch b(convert_mac("00:00:00:00:00:02"));
  Switch c(convert_mac("00:00:00:00:00:03"));
  Switch d(convert_mac("00:00:00:00:00:04"));
  Switch e(convert_mac("00:00:00:00:00:05"));
  Switch f(convert_mac("00:00:00:00:00:10"));

  link(a, b, LINK1000);
  link(a, c, LINK1000);
  link(b, d, LINK100);
  link(b, e, LINK16);
  link(e, d, LINK100);
  link(e, f, LINK16);
  link(d, f, LINK16);

  a.startSwitch();
  b.startSwitch();
  c.startSwitch();
  d.startSwitch();
  e.startSwitch();
  f.startSwitch();

  std::this_thread::sleep_for(std::chrono::seconds(10));

  a.stopSwitch();
  b.stopSwitch();
  c.stopSwitch();
  d.stopSwitch();
  e.stopSwitch();
  f.stopSwitch();

  std::cout << "Root paths : " << std::endl;
  std::cout << "Switch#" << a.bridge_id << " root path Switch#" << a.root_path
            << " cost: " << a.root_cost << " :: Root switch" << a.root_id
            << std::endl;
  std::cout << "Switch#" << b.bridge_id << " root path Switch#" << b.root_path
            << " cost: " << b.root_cost << " :: Root switch" << b.root_id
            << std::endl;
  std::cout << "Switch#" << c.bridge_id << " root path Switch#" << c.root_path
            << " cost: " << c.root_cost << " :: Root switch" << c.root_id
            << std::endl;
  std::cout << "Switch#" << d.bridge_id << " root path Switch#" << d.root_path
            << " cost: " << d.root_cost << " :: Root switch" << d.root_id
            << std::endl;
  std::cout << "Switch#" << e.bridge_id << " root path Switch#" << e.root_path
            << " cost: " << e.root_cost << " :: Root switch" << e.root_id
            << std::endl;
  std::cout << "Switch#" << f.bridge_id << " root path Switch#" << f.root_path
            << " cost: " << f.root_cost << " :: Root switch" << f.root_id
            << std::endl;
  std::cout << "Total messages exchanged : "
            << a.messages + b.messages + c.messages + d.messages + e.messages +
                   f.messages
            << std::endl;
  return 0;
}