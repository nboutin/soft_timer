/**
 * \file timer.c
 * \brief Software timer library
 * \date 2024-07
 * \author Nicolas Boutin
 */

#include <stddef.h>

#include "timer.h"

// --- Public functions

void Timer_init(Timer_t* timer, Timer_user_callback_t callback, void* user_data)
{
  if (timer != NULL)
  {
    timer->elapsed_time_ms = 0;
    timer->timeout_ms      = 0;
    timer->state           = TIMER_STATE_STOPPED;
    timer->callback        = callback;
    timer->user_data       = user_data;
  }
}

bool Timer_is_running(Timer_t* timer)
{
  return (timer != NULL) && (timer->state != TIMER_STATE_STOPPED);
}

void Timer_set_timeout(Timer_t* timer, uint32_t timeout_ms)
{
  if (timer != NULL)
  {
    timer->elapsed_time_ms = 0;
    timer->timeout_ms      = timeout_ms;
    timer->state           = TIMER_STATE_RUNNING;
  }
}

void Timer_set_interval(Timer_t* timer, uint32_t period_ms)
{
  if (timer != NULL)
  {
    timer->elapsed_time_ms = 0;
    timer->timeout_ms      = period_ms;
    timer->state           = TIMER_STATE_AUTO_RELOAD;
  }
}

void Timer_stop(Timer_t* timer)
{
  if (timer != NULL)
  {
    timer->elapsed_time_ms = 0;
    timer->state           = TIMER_STATE_STOPPED;
  }
}

void Timer_run(Timer_t* timer, uint32_t tick_ms)
{
  if (Timer_is_running(timer) == true)
  {
    timer->elapsed_time_ms += tick_ms;

    // Timeout reached
    if (timer->elapsed_time_ms >= timer->timeout_ms)
    {
      // Reload
      timer->elapsed_time_ms = 0;

      if (timer->state == TIMER_STATE_RUNNING)
      {
        timer->state = TIMER_STATE_STOPPED;
      }
      else if (timer->state == TIMER_STATE_AUTO_RELOAD)
      {
        // do nothing
      }
      else
      {
        // do nothing
      }

      if (timer->callback != NULL)
      {
        timer->callback(timer->user_data);
      }
    }
  }
}
