#ifndef BANKNOTE_AUTH_SAMPLE_QUEUE_H
#define BANKNOTE_AUTH_SAMPLE_QUEUE_H

#include <zephyr/kernel.h>

/**
 * Queue size
 */
#define SAMPLE_Q_CAPACITY 16

/**
 * Message queue with fixed capacity
 */
extern struct k_msgq g_sample_q;

#endif
