#ifndef BANKNOTE_AUTH_TYPES_H
#define BANKNOTE_AUTH_TYPES_H

#include <stdint.h>

/**
 * Sensor sample value type
 * 
 * TODO: Change placeholder type to actual sensor data
 */
typedef int32_t sample_value_t;

/**
 * Sensor ID
 */
typedef enum
{
    SENSOR_VIS = 0,
    SENSOR_UV = 1
} sensor_id_t;

/**
 * Sensor sample data
 */
typedef struct
{
    sensor_id_t     sensor;
    uint32_t        seq;
    int64_t         t_produced_ms;
    sample_value_t  value;
} sample_msg_t;

#endif
