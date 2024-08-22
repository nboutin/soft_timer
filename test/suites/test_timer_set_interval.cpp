/**
 * \file test_timer_set_interval.h
 * \brief Test software timer library
 * \date 2024-07
 * \author Nicolas Boutin
 */

#include "gmock/gmock.h"

extern "C" {
#include "soft_timer/timer.h"
}

using namespace testing;

class Timer_set_interval_test : public ::testing::Test {
protected:
  void SetUp()
  {
    cbk_call_counter = 0;
    Timer_init(&timer, timer_user_callback, nullptr);
  }
  Timer_t timer;
  static uint32_t cbk_call_counter;

  static void timer_user_callback(void* user_data)
  {
    cbk_call_counter++;
  }
};
uint32_t Timer_set_interval_test::cbk_call_counter = 0;

/**
 * \brief check timer initialization
 */
TEST_F(Timer_set_interval_test, set_interval)
{
  Timer_set_interval(&timer, 1000);

  EXPECT_EQ(timer.timeout_ms, 1000);
  EXPECT_EQ(timer.elapsed_time_ms, 0);
  EXPECT_EQ(timer.state, TIMER_STATE_AUTO_RELOAD);

  Timer_set_interval(nullptr, 1000);
}

TEST_F(Timer_set_interval_test, exact_period)
{
  Timer_set_interval(&timer, 1000);

  Timer_run(&timer, 500);
  Timer_run(&timer, 500);

  EXPECT_EQ(cbk_call_counter, 1);
}

TEST_F(Timer_set_interval_test, overshoot_period)
{
  Timer_set_interval(&timer, 1000);

  Timer_run(&timer, 300);
  Timer_run(&timer, 300);
  Timer_run(&timer, 300);
  Timer_run(&timer, 300);

  EXPECT_EQ(cbk_call_counter, 1);
}

TEST_F(Timer_set_interval_test, periodically)
{
  Timer_set_interval(&timer, 1000);

  Timer_run(&timer, 500);
  Timer_run(&timer, 500);
  EXPECT_EQ(cbk_call_counter, 1);

  Timer_run(&timer, 250);
  Timer_run(&timer, 250);
  Timer_run(&timer, 250);
  Timer_run(&timer, 250);
  EXPECT_EQ(cbk_call_counter, 2);
}
