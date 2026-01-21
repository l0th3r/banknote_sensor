#ifndef BANKNOTE_AUTH_SENSOR_HANDLER_H
#define BANKNOTE_AUTH_SENSOR_HANDLER_H

#include "types.h"

#include <zephyr/sys/atomic.h>

/**
 * Number of sample packets dropped at the queue form ISR.
 */
extern atomic_t g_dropped_pck_count;

/**
 * Number of formatted samples dropped at the queue from handler.
 */
extern atomic_t g_dropped_sample_count;

/**
 * Notify main handler that sensor sample is available
 * (functionis non blocking so safe to be called from ISR)
 */
void sensor_handler_notify(sensor_id_t sensor, sample_value_t value);

/**
 * Init sensor handler
 */
void sensor_handler_init(void);

#endif
