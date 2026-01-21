#include "sensor_handler.h"
#include "sample_queue.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(sensor_handler, LOG_LEVEL_INF);

/**
 * Sensor data packet
 */
typedef struct
{
    sensor_id_t sensor;
    sample_value_t value;
} sensor_pck_t;

/**
 * Sensor data packets queue to handle
 */
K_MSGQ_DEFINE(pck_q, sizeof(sensor_pck_t), 8, 4);

/**
 * Sequence counter
 */
static uint32_t seq_counter;

/**
 * Handling data packets from queue
 */
static void sensor_handler_task(void* a, void* b, void* c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    sensor_pck_t pck;
    sample_msg_t msg;

    while (1)
    {
        /* wait of ISR evnt */
        k_msgq_get(&pck_q, &pck, K_FOREVER);

        msg.sensor =        pck.sensor;
        msg.value =         pck.value;
        msg.seq =           seq_counter++;
        msg.t_produced_ms = k_uptime_get();

        /* drop if queue full */
        if (k_msgq_put(&g_sample_q, &msg, K_NO_WAIT) != 0)
        {
            LOG_WRN("Sample queue full, dropped seq %d.", msg.seq);
        }
    }
}

/**
 * Define higher priority thread, handling sensor data
 */
K_THREAD_DEFINE(sensor_handler_tid, 2048, sensor_handler_task, NULL, NULL, NULL, 1, 0, 0);

void sensor_handler_notify(sensor_id_t sensor, sample_value_t value)
{
    /* sensor packet from incoming event */
    sensor_pck_t spck =
    {
        .sensor = sensor,
        .value = value
    };

    /* IMPORTANT to not block ISR calling this functionn */
    k_msgq_put(&pck_q, &spck, K_NO_WAIT);
}

void sensor_handler_init(void)
{
    seq_counter = 0;
}
