# Banknote Sensor RTOS

This project is a **real-time embedded application** implementing a simplified
banknote authentication pipeline.

It is developed and executed in a **RTOS environment**, running on a
host-based target for fast iteration. The execution model, constraints, and
failure modes are those of a microcontroller-class system; only the physical
sensors are abstracted during development.

The application is structured exactly as it would be for deployment on real
hardware.

## Scope

What this project **does**:
- Implements a real-time, multi-stage pipeline
- Uses strict RTOS primitives: threads, priorities, timers, bounded queues
- Enforces embedded constraints
- Simulates sensor inputs while keeping the application logic unchanged

This project doesnt replicate accuratly the electrical environement, is it made first to display the architecture and structure needed in a similar realistic context.

### Project logic structure
```mermaid
---
title: Banknote Sensor Pipeline
---
flowchart TD
  subgraph SIM["Hardware Simulation"]
    subgraph TIM["Running on k_timer"]
      VIS_SENSOR["VIS Sensor"]
      UV_SENSOR["UV Sensor"]
    end
    VIS_SENSOR-- Filling on fixed interval -->VIS_SENSOR_BUF["VIS Sensor hardware memory"]
    UV_SENSOR-- Filling on fixed interval -->UV_SENSOR_BUF["UV Sensor hardware memory"]
  end
  VIS_SENSOR_BUF-- Simulate IRQ signal -->ISR
  UV_SENSOR_BUF-- Simulate IRQ signal -->ISR

ISR["ISR"]-- non blocking filling with just sensor id -->PCKQ["k_msgq of sensor id"]

  subgraph THR["Thread Context"]
    subgraph HND["Handler thread - priority 1"]
      HANDLER["Handler task"]-- thread wait for k_msgq to have data -->PCKQ
      HANDLER-- Retreive data from hardware memeory -->SENSOR_BUF["VIS or UV Sensor hardware memory"]
      HANDLER-- Fill with timestamp data and sensor id -->SMPLQ["Sample queue"]
    end
    subgraph PROC["Process thread  - priority 2"]
      PROCESSING["Processing Task"]-- thread wait for k_msgq to have data -->SMPLQ
      PROCESSING-- handle samples and comes to a conclusion -->DECISION["SUCCESS | INCONCLUSIVE | FAIL"]
    end
    subgraph MON["Monitoring thread - priority 3"]
      MONITORING["Monitoring task, every second gather all the atomic counters and print log"]
      MONITORING-->HANDLER
      MONITORING-->PROCESSING
    end
  end
```

## Execution Environment

- **RTOS:** Zephyr RTOS  
- **Target:** `native_sim` (host-based execution for development and CI)
- **Host OS:** Linux

The same application can later be built for real microcontroller targets without
structural changes.

## Repository Model

This repository is **application-only**.

It contains:
- application source code
- RTOS configuration
- a manifest pinning external dependencies

It does **not** contain:
- the RTOS source tree
- external modules
- build artifacts
- toolchains or SDKs

All external components are fetched and managed by the workspace tooling.

## Build Environment

### Host
- Linux
- Python ≥ 3.10
- CMake, Ninja
- RTOS SDK

### Target
- Host-based RTOS execution (`native_sim`)

## Build and Run

Initialize the workspace and fetch dependencies:

```sh
west init -m <repo-url>
cd <workspace>
west update
west zephyr-export
```

Build:

```sh
west build -b native_sim .
```

Run:

```sh
./build/zephyr/zephyr.exe
```
