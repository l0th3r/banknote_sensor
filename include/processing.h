#ifndef BANKNOTE_AUTH_PROCESSING_H
#define BANKNOTE_AUTH_PROCESSING_H

#include <zephyr/sys/atomic.h>

/**
 * Counter of processed msgs
 */
extern atomic_t g_processed_count;

/**
 * Init processing
 */
void processing_init(void);

#endif
