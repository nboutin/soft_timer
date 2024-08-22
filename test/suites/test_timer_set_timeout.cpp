/**
 * \file test_timer_set_timeout.h
 * \brief Test software timer library
 * \date 2024-07
 * \author Nicolas Boutin
 */

#include "gmock/gmock.h"

extern "C" {
#include "soft_timer/timer.h"
}

using namespace testing;

class Timer_set_timeout_test : public ::testing::Test {
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
uint32_t Timer_set_timeout_test::cbk_call_counter = 0;

TEST_F(Timer_set_timeout_test, set_timeout)
{
  Timer_set_timeout(&timer, 1000);

  EXPECT_EQ(timer.timeout_ms, 1000);
  EXPECT_EQ(timer.elapsed_time_ms, 0);
  EXPECT_EQ(timer.state, TIMER_STATE_RUNNING);

   Timer_set_timeout(nullptr, 1000);
}

TEST_F(Timer_set_timeout_test, exact_timeout)
{
  Timer_set_timeout(&timer, 1000);

  Timer_run(&timer, 500);
  Timer_run(&timer, 500);
  EXPECT_EQ(cbk_call_counter, 1);
}

/**
 * \brief callback is called after timeout overshoot
 */
TEST_F(Timer_set_timeout_test, timeout_overshoot)
{
  Timer_set_timeout(&timer, 1000);

  Timer_run(&timer, 300);
  Timer_run(&timer, 300);
  Timer_run(&timer, 300);
  Timer_run(&timer, 300);
  EXPECT_EQ(cbk_call_counter, 1);
}

TEST_F(Timer_set_timeout_test, only_once)
{
  Timer_set_timeout(&timer, 1);

  Timer_run(&timer, 0xFFFFFFFF);
  EXPECT_EQ(cbk_call_counter, 1);
  Timer_run(&timer, 0xFFFFFFFF);
  EXPECT_EQ(cbk_call_counter, 1);
}

TEST_F(Timer_set_timeout_test, new_timeout_after_expiration)
{
  Timer_set_timeout(&timer, 1000);

  Timer_run(&timer, 500);
  Timer_run(&timer, 500);
  EXPECT_EQ(cbk_call_counter, 1);

  Timer_set_timeout(&timer, 200);
  Timer_run(&timer, 150);
  Timer_run(&timer, 150);
  EXPECT_EQ(cbk_call_counter, 2);
}
