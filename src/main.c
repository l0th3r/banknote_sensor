#include "processing.h"
#include "monitoring.h"
#include "sensor_handler.h"
#include "sensor_sim.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <stdlib.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

int main(int argc, char** argv)
{
    processing_init();
    LOG_INF("Processing task running.");

    monitoring_init();
    LOG_INF("Monitoring task running.");

    sensor_handler_init();
    LOG_INF("Sensor handler task running.");

    LOG_INF("Waiting for samples...");

    sensors_sim_init();
    LOG_INF("Sensor simulation ready.");
    
    sensors_sim_start();

    return 0;
}
