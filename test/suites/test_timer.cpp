/**
 * \file test_timer.h
 * \brief Test software timer library
 * \date 2024-07
 * \author Nicolas Boutin
 */

#include "gmock/gmock.h"

extern "C" {
#include "soft_timer/timer.h"
}

using namespace testing;

class Timer_test : public ::testing::Test {
protected:
  void SetUp() {}
  // attributes
  Timer_t timer;
};

TEST_F(Timer_test, init_001)
{
  Timer_init(nullptr, nullptr, nullptr);
}

TEST_F(Timer_test, init_002)
{
  Timer_init(&timer, nullptr, nullptr);
  EXPECT_EQ(timer.timeout_ms, 0);
  EXPECT_EQ(timer.elapsed_time_ms, 0);
  EXPECT_EQ(timer.state, TIMER_STATE_STOPPED);
  EXPECT_EQ(timer.callback, nullptr);
  EXPECT_EQ(timer.user_data, nullptr);
}

TEST_F(Timer_test, stop_001)
{
  Timer_init(&timer, nullptr, nullptr);
  Timer_stop(nullptr);
}

TEST_F(Timer_test, stop_002)
{
  Timer_init(&timer, nullptr, nullptr);

  Timer_set_timeout(&timer, 1000);
  Timer_stop(&timer);
  EXPECT_EQ(timer.elapsed_time_ms, 0);
  EXPECT_EQ(timer.state, TIMER_STATE_STOPPED);

  Timer_set_interval(&timer, 1000);
  Timer_stop(&timer);
  EXPECT_EQ(timer.elapsed_time_ms, 0);
  EXPECT_EQ(timer.state, TIMER_STATE_STOPPED);
}

TEST_F(Timer_test, run_001)
{
  Timer_init(&timer, nullptr, nullptr);
  Timer_run(nullptr, 0);
}
