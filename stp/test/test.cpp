#include<gtest/gtest.h>

#include "stp.hpp"

TEST(test_stp_g, convergence) {
  std::shared_ptr<Switch> s0=std::make_shared<Switch>("00:00:00:00:00:01");
  std::shared_ptr<Switch> s1=std::make_shared<Switch>("00:00:00:00:00:02");
  std::shared_ptr<Switch> s2=std::make_shared<Switch>("00:00:00:00:00:03");
  std::shared_ptr<Switch> s3=std::make_shared<Switch>("00:00:00:00:00:04");
  std::shared_ptr<Switch> s4=std::make_shared<Switch>("00:00:00:00:00:05");
  std::shared_ptr<Switch> s5=std::make_shared<Switch>("00:00:00:00:00:10");

  link(s0,s1,LINK1000);
  link(s0,s2,LINK1000);
  link(s1,s3,LINK100);
  link(s1,s4,LINK16);
  link(s4,s3,LINK100);
  link(s4,s5,LINK16);
  link(s3,s5,LINK16);
  

  s0->startSwitch();
  s1->startSwitch();
  s2->startSwitch();
  s3->startSwitch();
  s4->startSwitch();
  s5->startSwitch();
  std::this_thread::sleep_for(std::chrono::seconds(60));
  s0->stopSwitch();
  s1->stopSwitch();
  s2->stopSwitch();
  s3->stopSwitch();
  s4->stopSwitch();
  s5->stopSwitch();

  EXPECT_EQ(s0->getRootId(), 1);
  EXPECT_EQ(s1->getRootId(), 1);
  EXPECT_EQ(s2->getRootId(), 1);
  EXPECT_EQ(s3->getRootId(), 1);
  EXPECT_EQ(s4->getRootId(), 1);
  EXPECT_EQ(s5->getRootId(), 1);

}
