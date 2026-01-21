#ifndef BANKNOTE_AUTH_SENSOR_HANDLER_H
#define BANKNOTE_AUTH_SENSOR_HANDLER_H

#include "types.h"

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
