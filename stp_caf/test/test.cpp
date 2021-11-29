#define CAF_SUITE stp_caf
#include "caf/test/dsl.hpp"
#include "caf/test/unit_test_impl.hpp"
#include "stp_caf.hpp"
using namespace caf;
namespace {
struct fixture {
  caf::actor_system_config cfg;
  caf::actor_system sys;
  caf::scoped_actor self;
  fixture() : sys(cfg), self(sys) {}
};
}  // namespace
CAF_TEST_FIXTURE_SCOPE(stp_caf_tests, fixture)

CAF_TEST(convergence) {
  init_global_meta_objects<id_block::my_project>();
  auto sw1 = sys.spawn<network_switch>(7u, 5u);
  auto sw2 = sys.spawn<network_switch>(2u, 10u);
  auto sw3 = sys.spawn<network_switch>(3u, 10u);
  auto sw4 = sys.spawn<network_switch>(4u, 10u);
  auto sw5 = sys.spawn<network_switch>(1u, 5u);
  auto sw6 = sys.spawn<network_switch>(6u, 5u);

  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw2),
                1u);
  self->request(sw2, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1),
                1u);

  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw3),
                2u);
  self->request(sw3, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1),
                1u);

  self->request(sw1, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4),
                3u);
  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw1),
                1u);

  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw5),
                2u);
  self->request(sw5, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4),
                1u);

  self->request(sw4, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw6),
                3u);
  self->request(sw6, infinite, link_atom_v, actor_cast<strong_actor_ptr>(sw4),
                1u);

  self->request(sw2, infinite, broadcast_atom_v);
  self->request(sw3, infinite, broadcast_atom_v);
  self->request(sw4, infinite, broadcast_atom_v);
  self->request(sw5, infinite, broadcast_atom_v);
  self->request(sw6, infinite, broadcast_atom_v);
  self->request(sw1, infinite, broadcast_atom_v);
  std::this_thread::sleep_for(std::chrono::seconds(21));

  self->request(sw1, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });
  self->request(sw2, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });
  self->request(sw3, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });
  self->request(sw4, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });
  self->request(sw5, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });
  self->request(sw6, infinite, get_rootID_atom_v)
      .receive([=](uint32_t rootID) { CAF_REQUIRE(rootID == 1); },
               [&](caf::error& err) { CAF_FAIL(err); });

  self->send_exit(sw1, exit_reason::user_shutdown);
  self->send_exit(sw2, exit_reason::user_shutdown);
  self->send_exit(sw3, exit_reason::user_shutdown);
  self->send_exit(sw4, exit_reason::user_shutdown);
  self->send_exit(sw5, exit_reason::user_shutdown);
  self->send_exit(sw6, exit_reason::user_shutdown);
}

CAF_TEST_FIXTURE_SCOPE_END()