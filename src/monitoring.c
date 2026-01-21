#include "monitoring.h"
#include "processing.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(monitoring, LOG_LEVEL_INF);

/**
 * Monitoring loop
 */
static void monitoring_task(void* a, void* b, void* c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    while (1)
    {
        k_sleep(K_SECONDS(1));
        LOG_INF("processed=%ld", (long)atomic_get(&g_processed_count));
    }
}

K_THREAD_DEFINE(monitoring_tid, 1536, monitoring_task, NULL, NULL, NULL, 3, 0, 0);

void monitoring_init(void)
{
    /* TODO: Start monitoring */
}
