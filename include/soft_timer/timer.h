/**
 * \file timer.h
 * \brief Software timer library
 * \date 2024-07
 * \author Nicolas Boutin
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

// --- Public types

typedef enum {
  TIMER_STATE_STOPPED,
  TIMER_STATE_RUNNING,
  TIMER_STATE_AUTO_RELOAD,
} Timer_state_t;

typedef void (*Timer_user_callback_t)(void* user_data);

typedef struct {
  uint32_t elapsed_time_ms;
  uint32_t timeout_ms;
  Timer_user_callback_t callback;
  Timer_state_t state;
  void* user_data;
} Timer_t;

// --- Public functions

/**
 * \brief Initialize timer
 * \param timer Timer to initialize
 * \param callback Callback to call when timeout
 * \param user_data User data to pass to callback
 */
void Timer_init(Timer_t* timer, Timer_user_callback_t callback, void* user_data);

/**
 * \brief Run the timer
 * \param timer Timer to run
 * \param tick_ms Time in ms since last call
 */
void Timer_run(Timer_t* timer, uint32_t tick_ms);

/**
 * \brief Check if timer is running
 * \param timer Timer to check
 * \return true if timer is running
 */
bool Timer_is_running(Timer_t* timer);

/**
 * \brief Set the timeout, one shot call
 * \param timer Timer to set timeout
 * \param timeout_ms Timeout in ms
 */
void Timer_set_timeout(Timer_t* timer, uint32_t timeout_ms);

/**
 * \brief Set interval, call periodically every period_ms
 * \param timer Timer to set interval
 * \param period_ms Period in ms
 */
void Timer_set_interval(Timer_t* timer, uint32_t period_ms);

/**
 * \brief Stop the timer
 * \details Does not call user callback
 * \param timer Timer to stop
 */
void Timer_stop(Timer_t* timer);
