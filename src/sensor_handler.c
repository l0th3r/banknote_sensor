#include "sensor_handler.h"
#include "sample_queue.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(sensor_handler, LOG_LEVEL_INF);

atomic_t g_dropped_pck_count;
atomic_t g_dropped_sample_count;

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
        /* wait for sample packets q to be filled from ISR */
        k_msgq_get(&pck_q, &pck, K_FOREVER);

        msg.sensor =        pck.sensor;
        msg.value =         pck.value;
        msg.seq =           seq_counter++;
        msg.t_produced_ms = k_uptime_get();

        /* drop if queue full */
        if (k_msgq_put(&g_sample_q, &msg, K_NO_WAIT) != 0)
        {
            atomic_inc(&g_dropped_sample_count);
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
    if (k_msgq_put(&pck_q, &spck, K_NO_WAIT) != 0)
    {
        atomic_inc(&g_dropped_pck_count);
    }
}

void sensor_handler_init(void)
{
    seq_counter = 0;
    atomic_set(&g_dropped_pck_count, 0);
    atomic_set(&g_dropped_sample_count, 0);
}
