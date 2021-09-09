#include "caf/all.hpp"
#include <iostream>
using namespace caf;

using switch_type
  = typed_actor<result<void>(add_atom, uint32_t, uint32_t),
                result<void>(put_atom, uint32_t), result<uint32_t>(get_atom),
                result<void>(link_atom, strong_actor_ptr)>;

using sw=switch_type::extend<result<void>(link_atom,switch_type)>;

class nswitch : public switch_type::base {
public:
  nswitch(actor_config& cfg, uint32_t bridgeid)
    : switch_type::base(cfg), m_bridgeid(bridgeid), m_rootid(bridgeid) {
  }

protected:
  behavior_type make_behavior() override {
    return {
      [=](add_atom, uint32_t val, uint32_t val2) {},
      [=](put_atom, uint32_t val) { m_bridgeid = val; },
      [=](get_atom) { return m_bridgeid; },
      [=](link_atom, strong_actor_ptr sw) {
        table.insert(actor_cast<switch_type>(sw));
        request(actor_cast<switch_type> (sw), std::chrono::seconds(10), get_atom_v)
          .then(
            [&](uint32_t add) {
              aout(this) << "switch 1 mac address " << add << std::endl;
            },
            [&](const error&) {});
      },
    };
  }

private:
  uint32_t m_bridgeid;
  uint32_t m_rootid;
  std::set<switch_type> table;
};

void caf_main(actor_system& sys) {
  auto sw1 = sys.spawn<nswitch>(23u);
  auto sw2 = sys.spawn<nswitch>(24u, actor_cast<strong_actor_ptr>(sw1));
  scoped_actor self{sys};

  self->request(sw1, std::chrono::seconds(10), link_atom_v,
                actor_cast<strong_actor_ptr>(sw2));
}

CAF_MAIN()
