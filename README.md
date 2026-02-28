# SchedulerSim - CPU Scheduler Simulator

A C++ simulation framework for testing and comparing different CPU scheduling algorithms.

## Project Overview

SchedulerSim is an educational tool that simulates various CPU scheduling algorithms commonly used in operating systems. It creates process objects, schedules them using different algorithms, and tracks performance metrics like response time and turnaround time.

## Architecture Overview

```
SchedulerSim/
├── main.cpp                 # Entry point - UI for scheduler selection
├── processes.h/cpp          # Process class definition and implementation
├── timer.h/cpp              # Performance metrics tracking
├── Makefile                 # Build configuration
└── schedulers/              # Scheduler implementations
    ├── schedulers.h         # Scheduler function declarations
    ├── FIFO.cpp            # First In First Out implementation
    ├── SJF.cpp             # Shortest Job First implementation
    ├── RR.cpp              # Round Robin implementation
    ├── SWQ.cpp             # Shortest Wait Queue implementation
    └── IO.cpp              # IO request handling
```

## Component Hierarchy

### 1. **Main Entry Point**
- **File**: `main.cpp`
- **Purpose**: User interface for selecting and running different schedulers
- **Process Flow**:
  - Creates a list of sample processes with predefined types
  - Displays menu for scheduler selection
  - Invokes appropriate scheduler function based on user choice
  - Supported Process Types: `OS_FUNCTION`, `TEXT_EDITOR`, `MEDIA_PLAYER`

### 2. **Process Class**
- **Files**: `processes.h`, `processes.cpp`
- **Responsibility**: Represents individual tasks/processes in the system
- **Key Attributes**:
  - `ID`: Unique process identifier
  - `procType`: Type of process (OS_FUNCTION, TEXT_EDITOR, MEDIA_PLAYER)
  - `runTime`: CPU time needed for execution
  - `ioCount`: Number of I/O requests
  - `currentStatus`: Process state (READY, RUNNING, BLOCKED, DONE)
- **Default Parameters** (by process type):
  - `OS_FUNCTION`: 5 seconds runtime, 0 I/O calls
  - `TEXT_EDITOR`: 2 seconds runtime, 2 I/O calls
  - `MEDIA_PLAYER`: 5 seconds runtime, 1 I/O call
- **Key Methods**:
  - `run()`: Executes the process for one time unit
  - `ioCall()`: Handles I/O requests
  - `getID()`: Returns process ID
  - `timeLeft()`: Returns remaining execution time

### 3. **Timer Class**
- **Files**: `timer.h`, `timer.cpp`
- **Purpose**: Tracks and calculates performance metrics
- **Metrics Tracked**:
  - Individual response times
  - Individual turnaround times
  - Average response time
  - Average turnaround time
- **Key Methods**:
  - `start()`: Mark process start time
  - `respond()`: Calculate response time
  - `isResponded()`: Check if process has been responded to

### 4. **Scheduler Implementations**
- **Location**: `schedulers/` directory
- **Base File**: `schedulers.h` (function declarations)

#### 4.1 FIFO Scheduler
- **File**: `schedulers/FIFO.cpp`
- **Algorithm**: First In First Out (FCFS)
- **Behavior**: Processes are executed in the order they arrive
- **Use Case**: Simple, fair scheduling

#### 4.2 SJF Scheduler
- **File**: `schedulers/SJF.cpp`
- **Algorithm**: Shortest Job First
- **Behavior**: Prioritizes processes with shortest execution time
- **Use Case**: Minimizing average waiting time

#### 4.3 Round Robin Scheduler
- **File**: `schedulers/RR.cpp`
- **Algorithm**: Time-slice based round robin scheduling
- **Configuration**: `TIME_SLICE = 1 second`
- **Behavior**: Each process gets a fixed time quantum before yielding
- **Use Case**: Fair scheduling with preemption

#### 4.4 Shortest Wait Queue Scheduler
- **File**: `schedulers/SWQ.cpp`
- **Algorithm**: Hybrid approach considering process queue length
- **Behavior**: Prioritizes jobs based on wait time and queue position
- **Use Case**: Reducing average waiting time in complex scenarios

#### 4.5 I/O Handler
- **File**: `schedulers/IO.cpp`
- **Function**: `ioCall()`
- **Purpose**: Manages I/O requests and process blocking
- **Behavior**: Handles transitions from RUNNING to BLOCKED states
- **Configuration**: `IO_TIME = 2 seconds`

## Data Flow

```
┌─────────────────┐
│   main.cpp      │ ◄─── User selects scheduler
└────────┬────────┘
         │
         ├─► Creates process list
         │
         └─► Calls Scheduler Function
                    │
         ┌──────────┼──────────┐
         ▼          ▼          ▼
      FIFO.cpp  SJF.cpp   RR.cpp (etc.)
         │          │          │
         └──────────┼──────────┘
                    │
              Processes execute:
              ┌─────────────────┐
              │ process.run()   │
              │ process.ioCall()│
              │ timer tracking  │
              └─────────────────┘
                    │
              ┌─────v──────┐
              │ Metrics    │
              │ Output     │
              └────────────┘
```

## Key Constants and Configuration

| Constant | Value | Purpose |
|----------|-------|---------|
| `IO_TIME` | 2 seconds | Duration of I/O operations |
| `TIME_SLICE` | 1 second | Time quantum for Round Robin |

## Process States

```
READY  ──► RUNNING ──┬──► BLOCKED ──► READY
           │         │
           │         └──► DONE (exit)
           │
           └─────────────► DONE (if no I/O)
```

## Compilation

The project includes a `Makefile` that supports:
- **Linux build** (default): `make`
- **Windows build**: `make win64`
- **Clean build**: `make clean`

### Build Artifacts
- Object files: `*.o`, `schedulers/*.o`
- Executable: `SchedulerSim` (Linux) or `SchedulerSim.exe` (Windows)

## Usage

1. Compile the project: `make`
2. Run the executable: `./SchedulerSim`
3. Select a scheduler (1-4) from the menu
4. View scheduling results and performance metrics

## Technical Language

- **Language**: C++
- **Standard Library Usage**: `<list>`, `<map>`, `<iostream>`, `<chrono>`, `<thread>`
- **Design Pattern**: Modular function-based architecture
- **Memory Management**: Dynamic allocation with `new`/`delete`

## Future Enhancements

- GUI implementation (noted as "wxTest.cpp" in project)
- Additional scheduling algorithms
- Real-time performance visualization
- Process priority levels
- Multi-core scheduling simulation
