#ifndef BANKNOTE_AUTH_SENSOR_SIM_H
#define BANKNOTE_AUTH_SENSOR_SIM_H

#include "types.h"

/**
 * Init sensor simulation
 */
void sensors_sim_init(void);

/**
 * Start sensor simulation
 */
void sensors_sim_start(void);

/**
 * End sensor simulation
 */
void sensors_sim_stop(void);

/**
 * Extract sensoor sample data from simulated hardware buffer
 * @return 0 on success, 1 otherwise
 * @note typically provided by sensor driver
 * And typically cqlled after a data-ready interrupt.
 */
int sensor_driver_get_raw(sensor_id_t sensor, sample_value_t* out);

#endif
