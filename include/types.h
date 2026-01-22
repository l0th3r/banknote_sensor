#ifndef BANKNOTE_AUTH_TYPES_H
#define BANKNOTE_AUTH_TYPES_H

#include <stdint.h>

/**
 * Sensor sample value type
 * 
 * Note: matching real VIS and UV sensors sample data.
 */
typedef uint16_t sample_value_t;

/**
 * Sensor ID
 */
typedef enum
{
    SENSOR_VIS = 1,
    SENSOR_UV = 2
} sensor_id_t;

/**
 * Sensor sample data
 */
typedef struct
{
    sensor_id_t     sensor;
    uint32_t        seq;
    int64_t         t_handled_ms;
    sample_value_t  value;
} sample_msg_t;

#endif
