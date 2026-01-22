#include "sensor_handler.h"
#include "sample_queue.h"
#include "sensor_sim.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(sensor_handler, LOG_LEVEL_INF);

atomic_t g_dropped_pck_count;
atomic_t g_dropped_sample_count;
atomic_t g_failed_sensor_buf_read;

/**
 * Sensor signals queue
 * Filled by ISR registering an IRQ event
 */
K_MSGQ_DEFINE(pck_q, sizeof(sensor_id_t), 8, 4);

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

    sensor_id_t sid;
    sample_msg_t msg;
    sample_value_t raw_value;

    while (1)
    {
        /* wait for sample packets q to be filled from ISR */
        k_msgq_get(&pck_q, &sid, K_FOREVER);

        if (sensor_driver_get_raw(sid, &raw_value) == 0)
        {
            /* forge */
            msg.sensor =        sid;
            msg.value =         raw_value;
            msg.seq =           seq_counter++;
            msg.t_handled_ms = k_uptime_get();

            /* drop if queue full */
            if (k_msgq_put(&g_sample_q, &msg, K_NO_WAIT) != 0)
            {
                atomic_inc(&g_dropped_sample_count);
            }
        }
        else
        {
            /* failed to read memory potential time lost */
            atomic_inc(&g_failed_sensor_buf_read);
        }
    }
}

/**
 * Define higher priority thread, handling sensor data
 */
K_THREAD_DEFINE(sensor_handler_tid, 2048, sensor_handler_task, NULL, NULL, NULL, 1, 0, 0);

/**
 * Simulated ISR
 */
void handler_isr(sensor_id_t sensor)
{
    if (k_msgq_put(&pck_q, &sensor, K_NO_WAIT) != 0)
    {
        atomic_inc(&g_dropped_pck_count);
    }
}

void sensor_handler_init(void)
{
    seq_counter = 0;
    atomic_set(&g_dropped_pck_count, 0);
    atomic_set(&g_dropped_sample_count, 0);
    atomic_set(&g_failed_sensor_buf_read, 0);
}
