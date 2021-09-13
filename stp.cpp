#include "caf/all.hpp"
#include <iostream>

using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(my_project, caf::first_custom_type_id)

  CAF_ADD_ATOM(my_project, advertise_atom)
  CAF_ADD_ATOM(my_project, broadcast_atom)

CAF_END_TYPE_ID_BLOCK(my_project)

using switch_type = typed_actor<
  //result<void>(add_atom, uint32_t, uint32_t), result<void>(put_atom, uint32_t),
  result<uint32_t>(get_atom), result<void>(link_atom, strong_actor_ptr),
  result<void>(advertise_atom, uint32_t), result<void>(broadcast_atom)>;

class nswitch : public switch_type::base {
public:
  explicit nswitch(actor_config& cfg, uint32_t bridge_id)
    : switch_type::base(cfg), m_bridgeid(bridge_id), m_rootid(bridge_id) {
  }

  switch_type::pointer self;

protected:
  behavior_type make_behavior() override {
    return {
      // [=](add_atom, uint32_t val, uint32_t val2) {},
      // [=](put_atom, uint32_t val) { m_bridgeid = val; },
      [=](get_atom) { return m_bridgeid; },
      [=](link_atom, strong_actor_ptr sw) {
        table.insert(actor_cast<switch_type>(sw));
        // request(actor_cast<switch_type>(sw), std::chrono::seconds(10),
        //         get_atom_v)
        //   .then(
        //     [&](uint32_t add) {
        //       aout(this) << "switch 1 mac address " << add << std::endl;
        //     },
        //     [&](const error&) {});

        //delayed_send(this, std::chrono::milliseconds(1), broadcast_atom_v);
      },
      [=](advertise_atom, uint32_t rootid) {
        aout(this) << "advertised to  " << m_bridgeid << " " << rootid
                   << std::endl;

        if (m_rootid > rootid)
          m_rootid = rootid;
      },
      [=](broadcast_atom) {
        for (auto& sw : table) {
          request(sw, std::chrono::seconds(10), advertise_atom_v, m_rootid);

          // request(sw, std::chrono::seconds(10), get_atom_v)
          //   .then(
          //     [&](uint32_t add) {
          //       aout(this) << "switch 1 mac address " << add << std::endl;
          //     },
          //     [&](const error&) {});
        }
        delayed_send(this, std::chrono::seconds(10), broadcast_atom_v);
      }};
  }

private:
  uint32_t m_bridgeid;
  uint32_t m_rootid;
  std::set<switch_type> table;
};

void caf_main(actor_system& sys) {
  auto sw1 = sys.spawn<nswitch>(1u);
  auto sw2 = sys.spawn<nswitch>(2u);
  auto sw3 = sys.spawn<nswitch>(3u);
  auto sw4 = sys.spawn<nswitch>(4u);
  auto sw5 = sys.spawn<nswitch>(5u);
  auto sw6 = sys.spawn<nswitch>(6u);
  scoped_actor self{sys};

  // self->request(sw1, infinite, advertise_atom_v, 2u);
  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw2));
  self->request(sw2, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1));

  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw3));
  self->request(sw3, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1));

  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4));
  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1));

  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw5));
  self->request(sw5, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4));

  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw6));
  self->request(sw6, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4));
  //

  self->request(sw2, infinite, broadcast_atom_v);
  self->request(sw3, infinite, broadcast_atom_v);
  self->request(sw4, infinite, broadcast_atom_v);
  self->request(sw5, infinite, broadcast_atom_v);
  self->request(sw6, infinite, broadcast_atom_v);
  self->request(sw1, infinite, broadcast_atom_v);

  sys.registry();
  while (true)
    ;
}

CAF_MAIN(id_block::my_project)
