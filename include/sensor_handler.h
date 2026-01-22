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
 * Number of miscatch in simulated hardware memory.
 * (When handler try to read in sensor memory but failed)
 */
extern atomic_t g_failed_sensor_buf_read;

/**
 * Simulated ISR, non blocking function to simply signal that a sensor have
 * some data available in it's simulated hardware buffer
 */
void handler_isr(sensor_id_t sensor);

/**
 * Init sensor handler
 */
void sensor_handler_init(void);

#endif
