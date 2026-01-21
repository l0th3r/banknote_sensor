#include "sensor_sim.h"
#include "sensor_handler.h"
#include "types.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(sensor_sim, LOG_LEVEL_INF);

/* temp data */
#define VIS_SENSOR_SPAN_MS  10
#define UV_SENSOR_SPAN_MS   25

static void vis_timer_isr(struct k_timer *t);
static void uv_timer_isr(struct k_timer *t);

/* define kernel timers */
K_TIMER_DEFINE(vis_timer, vis_timer_isr, NULL);
K_TIMER_DEFINE(uv_timer, uv_timer_isr, NULL);

/* ISR context */
static void vis_timer_isr(struct k_timer *t)
{
    ARG_UNUSED(t);

    /* TODO: placeholder sampling */
    sensor_handler_notify(SENSOR_VIS, 1);
}

static void uv_timer_isr(struct k_timer *t)
{
    ARG_UNUSED(t);

    /* TODO: placeholder sampling */
    sensor_handler_notify(SENSOR_UV, 1);
}

void sensors_sim_init(void)
{
    /* TODO: init sensor simulation */
}

void sensors_sim_start(void)
{
    LOG_INF("Starting sensor simulation : VIS=%dms UV=%dms", VIS_SENSOR_SPAN_MS, UV_SENSOR_SPAN_MS);

    k_timer_start(&vis_timer, K_MSEC(VIS_SENSOR_SPAN_MS), K_MSEC(VIS_SENSOR_SPAN_MS));
    k_timer_start(&uv_timer, K_MSEC(UV_SENSOR_SPAN_MS), K_MSEC(UV_SENSOR_SPAN_MS));
}

void sensors_sim_stop(void)
{
    k_timer_stop(&vis_timer);
    k_timer_stop(&uv_timer);
    LOG_INF("Stopped sensor simulation.");
}
