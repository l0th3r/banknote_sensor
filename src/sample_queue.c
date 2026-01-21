#include "sample_queue.h"
#include "types.h"

/* TODO: check if alignment matches sample_msg_t size when sample_value_t type is defined*/
K_MSGQ_DEFINE(g_sample_q, sizeof(sample_msg_t), SAMPLE_Q_CAPACITY, 4);
