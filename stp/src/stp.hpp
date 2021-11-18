#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
static constexpr uint32_t LINK4 = 250;
static constexpr uint32_t LINK10 = 100;
static constexpr uint32_t LINK16 = 62;
static constexpr uint32_t LINK100 = 19;
static constexpr uint32_t LINK1000 = 4;
static constexpr uint32_t LINK10000 = 2;

uint64_t convert_mac(std::string);

class Switch {
 public:
  Switch(uint32_t bridgeId);
  Switch(const std::string& mac);
  Switch(std::string&& mac);
  uint64_t getBridgeId() const;
  uint64_t getRootId() const;
  uint64_t getRootPath() const;
  uint32_t getRootCost() const;
  std::vector<std::weak_ptr<Switch>> getNeighbors() const;
  friend void link(const std::shared_ptr<Switch>& a,
                   const std::shared_ptr<Switch> b, uint32_t cost);
  void startSwitch();
  void stopSwitch();

 private:
  void start();
  struct Link {
    std::weak_ptr<Switch> sw;
    uint32_t cost;
    Link() = default;
    Link(const std::weak_ptr<Switch>& sw, uint32_t cost) : sw(sw), cost(cost) {}
  };
  uint32_t messages = 0;
  uint64_t bridge_id;
  uint64_t root_id;
  uint64_t root_path;
  uint32_t root_cost;
  std::vector<Link> neighbors;
  std::thread th;
  std::atomic_bool terminate{false};
  std::mutex mutex;
};
