#include <gtest/gtest.h>

#include "stp_concurrent.hpp"

TEST(test_concurrent,convergence) {
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

  a.startSwitch();
  b.startSwitch();
  c.startSwitch();
  d.startSwitch();
  e.startSwitch();
  std::this_thread::sleep_for(std::chrono::seconds(60));
  a.stopSwitch();
  b.stopSwitch();
  c.stopSwitch();
  d.stopSwitch();
  e.stopSwitch();

  ASSERT_EQ(a.getRootId(), 100);
  ASSERT_EQ(b.getRootId(), 100);
  ASSERT_EQ(c.getRootId(), 100);
  ASSERT_EQ(d.getRootId(), 100);
  ASSERT_EQ(e.getRootId(), 100);
}
