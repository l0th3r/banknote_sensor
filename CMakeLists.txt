cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(banknote_sensor)

target_sources(app PRIVATE
    src/main.c
    src/sample_queue.c
    src/processing.c
    src/monitoring.c
    src/sensor_handler.c
    src/sensor_sim.c
)
target_include_directories(app PRIVATE include)