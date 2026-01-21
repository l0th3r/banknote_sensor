#include "processing.h"
#include "types.h"
#include "sample_queue.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(processing, LOG_LEVEL_INF);

/* processed elements counter */
atomic_t g_processed_count;

static void processing_task(void* a, void* b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    sample_msg_t msg;

    /* loop is not polling K_FOREVER sleep the current thread until data to get */
    while (1)
    {
        /* wait for incoming sample msg to process */
        (void)k_msgq_get(&g_sample_q, &msg, K_FOREVER);
        atomic_inc(&g_processed_count);
        
        /* TODO: actually process it */
    }
}

K_THREAD_DEFINE(processing_tid, 2048, processing_task, NULL, NULL, NULL, 2, 0, 0);

void processing_init(void)
{
    atomic_set(&g_processed_count, 0);
}
