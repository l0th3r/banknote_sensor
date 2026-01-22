#include "sensor_sim.h"
#include "sensor_handler.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/ring_buffer.h>

LOG_MODULE_REGISTER(sensor_sim, LOG_LEVEL_INF);

/* computer sensor simulated hardware buffer holding 16 samples  */
#define SENSOR_BUF_SIZE (16 * sizeof(sample_value_t))

/* simulated sensors hardware buffer */
RING_BUF_DECLARE(vis_buf, SENSOR_BUF_SIZE);
RING_BUF_DECLARE(uv_buf,  SENSOR_BUF_SIZE);

/* spin lock on simulated hardware buffers */
static struct k_spinlock vis_buf_lock;
static struct k_spinlock uv_buf_lock;

/**
 * Simulate sensor storing data in hardware buffer
 * Returns 1 if data was not written
 */
static int sensor_sample_emit(sensor_id_t id, const sample_value_t* raw)
{
    struct ring_buf* rbuf;
    struct k_spinlock* slock;

    if (id == SENSOR_VIS)
    {
        rbuf = &vis_buf;
        slock = &vis_buf_lock;
    }
    else if (id == SENSOR_UV)
    {
        rbuf = &uv_buf;
        slock = &uv_buf_lock;
    }
    else
    {
        /* sensor id not valid */
        return 1;
    }

    /* lock buf and write in ring buf (simulated hardware buffer) */
    k_spinlock_key_t skey = k_spin_lock(slock);
    uint32_t bwrote = ring_buf_put(rbuf, (const uint8_t*)raw, sizeof(sample_value_t));
    k_spin_unlock(slock, skey);

    /* checl written bytes */
    if (bwrote != sizeof(sample_value_t))
    {
        return 1;
    }

    /* IRQ SIMULATION */
    handler_isr(id);
    return 0;
}

/* sensors sampling timing */
#define VIS_SAMPLING_SPAN_MS    10
#define UV_SAMPLING_SPAN_MS     25

/* define kernel handlers and timers */
static void vis_sampling_evt(struct k_timer* t);
static void uv_sampling_evt(struct k_timer* t);
K_TIMER_DEFINE(vis_timer, vis_sampling_evt, NULL);
K_TIMER_DEFINE(uv_timer, uv_sampling_evt, NULL);

/* called when sensor should generate a sample */
static void vis_sampling_evt(struct k_timer* t)
{
    ARG_UNUSED(t);

    /* TODO: GENERATE VIS SENSOR SAMPLE */
    sample_value_t generated_vis_sample = 1;
    sensor_sample_emit(SENSOR_VIS, &generated_vis_sample);
}

static void uv_sampling_evt(struct k_timer* t)
{
    ARG_UNUSED(t);

    /* TODO: Generate UV SENSOR SAMPLE */
    sample_value_t generated_uv_sample = 1;
    sensor_sample_emit(SENSOR_UV, &generated_uv_sample);
}

/* sensor sim states */
void sensors_sim_init(void)
{
    /* TODO: init sensor simulation */
    ring_buf_reset(&vis_buf);
    ring_buf_reset(&uv_buf);
}

void sensors_sim_start(void)
{
    LOG_INF("Starting sensor simulation : VIS=%dms UV=%dms", VIS_SAMPLING_SPAN_MS, UV_SAMPLING_SPAN_MS);

    k_timer_start(&vis_timer, K_MSEC(VIS_SAMPLING_SPAN_MS), K_MSEC(VIS_SAMPLING_SPAN_MS));
    k_timer_start(&uv_timer, K_MSEC(UV_SAMPLING_SPAN_MS), K_MSEC(UV_SAMPLING_SPAN_MS));
}

void sensors_sim_stop(void)
{
    k_timer_stop(&vis_timer);
    k_timer_stop(&uv_timer);
    LOG_INF("Stopped sensor simulation.");
}

int sensor_driver_get_raw(sensor_id_t id, sample_value_t* out)
{
    struct ring_buf* rbuf;
    struct k_spinlock* slock;

    if (out == NULL)
    {
        return 1;
    }

    if (id == SENSOR_VIS)
    {
        rbuf = &vis_buf;
        slock = &vis_buf_lock;
    }
    else if (id == SENSOR_UV)
    {
        rbuf = &uv_buf;
        slock = &uv_buf_lock;
    }
    else
    {
        /* sensor id not valid */
        return 1;
    }

    
    /* lock buf and read in ring buf */
    k_spinlock_key_t skey = k_spin_lock(slock);
    uint32_t bread = ring_buf_get(rbuf, (uint8_t*)out, sizeof(sample_value_t));
    k_spin_unlock(slock, skey);

    /* check read bytes */
    if (bread != sizeof(sample_value_t))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
