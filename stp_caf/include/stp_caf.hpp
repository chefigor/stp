#include <iostream>
#include "caf/all.hpp"
using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(my_project, caf::first_custom_type_id)

CAF_ADD_ATOM(my_project, advertise_atom)
CAF_ADD_ATOM(my_project, broadcast_atom)
CAF_ADD_ATOM(my_project, get_rootID_atom)

CAF_END_TYPE_ID_BLOCK(my_project)

using switch_type =
    typed_actor<result<uint32_t>(get_atom),
                result<void>(link_atom, strong_actor_ptr, uint32_t),
                result<void>(advertise_atom, uint32_t),
                result<void>(broadcast_atom),
                result<uint32_t>(get_rootID_atom)>;

class network_switch : public switch_type::base {
 public:
  explicit network_switch(actor_config &cfg, uint32_t bridge_id,
                          uint32_t helloTimer)
      : switch_type::base(cfg),
        m_bridgeId(bridge_id),
        m_rootId(bridge_id),
        m_helloTimer(helloTimer) {}

  switch_type::pointer self;

  void on_exit() override {
    for (auto &[port, sw] : table) destroy(sw);
  }

 protected:
  behavior_type make_behavior() override {
    return {[=](get_atom) { return m_bridgeId; },
            [=](get_rootID_atom){return m_rootId;},
            [=](link_atom, strong_actor_ptr sw, uint32_t port) {
              table.insert(std::make_pair(port, actor_cast<switch_type>(sw)));
            },
            [=](advertise_atom, uint32_t rootId) {
              aout(this) << "advertised to  " << m_bridgeId << " " << rootId
                         << std::endl;
              if (m_rootId > rootId) m_rootId = rootId;
            },
            [=](broadcast_atom) {
              aout(this) << "Switch : " << m_bridgeId << " is broadcasting "
                         << m_rootId << std::endl;
              for (auto &sw : table) {
                request(sw.second, std::chrono::seconds(10), advertise_atom_v,
                        m_rootId);
              }
              delayed_send(this, std::chrono::seconds(m_helloTimer),
                           broadcast_atom_v);
            }};
  }

 private:
  uint32_t m_bridgeId;
  uint32_t m_rootId;
  uint32_t m_helloTimer;
  std::map<uint32_t, switch_type> table;
};
