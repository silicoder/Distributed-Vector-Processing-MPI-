# Distributed Vector Processing using MPI

A parallel computing project that demonstrates **distributed vector processing using MPI** in C.

The project divides a large vector among multiple MPI processes. Each process performs a local computation on its assigned portion, and the partial results are combined using MPI reduction.

## 1. Project Overview

### Problem Statement

Process a large vector by distributing its elements among multiple processes and compute the total sum efficiently using Message Passing Interface (MPI).

### Objectives

- Divide a large vector among multiple processes.
- Use MPI to distribute vector data.
- Perform local computation independently on each process.
- Combine local results using MPI reduction.
- Compare sequential and distributed execution.
- Observe how execution time changes with the number of processes.

## 2. Technology Used

| Component | Technology |
|---|---|
| Programming Language | C |
| Parallel Programming | MPI |
| MPI Implementation | Open MPI 4.1.6 |
| Compiler | GCC 13.3.0 |
| Operating System | Ubuntu 24.04 |
| Virtualization | VMware Workstation |
| Execution | `mpirun` |

> **Important:** MPI is the programming standard/API used by the project. **Open MPI is the implementation that provides the MPI tools** such as `mpicc` and `mpirun`. Therefore, using Open MPI does **not** mean the project is a different project; this is an MPI project implemented with Open MPI.

## 3. System Architecture

```text
                 Large Vector
                      |
                      v
               MPI Process 0
                      |
             MPI_Scatter / Distribution
          _________/    |     \_________
         /              |               \
        v               v                v
   Process 0        Process 1        Process 2 ... Process N
   Local data       Local data       Local data
   Local sum        Local sum        Local sum
         \              |               /
          \_____________|______________/
                        |
                        v
                  MPI_Reduce
                        |
                        v
                  Final Result
```

## 4. MPI Program Flow

The main MPI program follows this sequence:

1. `MPI_Init()` starts the MPI environment.
2. `MPI_Comm_rank()` identifies the current process.
3. `MPI_Comm_size()` obtains the total number of processes.
4. The vector is divided among processes.
5. `MPI_Scatter()` distributes portions of the vector.
6. Every process computes its local result.
7. `MPI_Reduce()` combines the local results.
8. Process 0 displays the final result.
9. `MPI_Finalize()` terminates MPI.

## 5. Sequential Version

The sequential program processes all **1,000,000 elements** in a single process.

The supplied terminal measurements were:

| Run | Execution Time (s) |
|---:|---:|
| 1 | 0.002510 |
| 2 | 0.002068 |
| 3 | 0.001601 |
| 4 | 0.001694 |
| **Average** | **0.001968** |

The sequential result shown in the terminal is:

```text
Vector Size : 1000000
Total Sum   : 500000500000
```

## 6. MPI Execution

### 2 Processes

For 2 MPI processes:

```text
Vector Size        : 1000000
Number of Processes: 2
Elements per Process: 500000
Total Vector Sum   : 500000500000
Parallel Time      : 0.002783 seconds
```

Each process handles approximately **500,000 elements**.

### 4 Processes

For 4 MPI processes:

```text
Vector Size        : 1000000
Number of Processes: 4
Elements per Process: 250000
Total Vector Sum   : 500000500000
Parallel Time      : 0.003888 seconds
```

Each process handles approximately **250,000 elements**.

## 7. Performance Comparison

The following graph is based directly on the execution-time values visible in the supplied terminal screenshots.

![Execution Time Comparison](graphs/execution_time_comparison.png)

### Observed Values

| Configuration | Processes | Time (s) |
|---|---:|---:|
| Sequential | 1 | 0.001968 average |
| MPI | 2 | 0.002783 |
| MPI | 4 | 0.003888 |

The result does **not** show a speedup on this particular VMware/Ubuntu test. This is reasonable for a relatively small computation because MPI introduces process-management, communication, synchronization and reduction overhead. On a single laptop/VM, those overheads can dominate the actual vector calculation.

Therefore, the experiment demonstrates **MPI distribution and process-level parallelism**, rather than claiming that more MPI processes are always faster.

## 8. Observed Speedup

Speedup is calculated as:

```text
Speedup = Sequential Execution Time / Parallel Execution Time
```

Using the average sequential time:

```text
Sequential average = 0.001968 seconds

2-process speedup = 0.001968 / 0.002783
                  ≈ 0.707

4-process speedup = 0.001968 / 0.003888
                  ≈ 0.506
```

![Observed Speedup](graphs/speedup.png)

A value below 1 means the measured parallel execution was slower than the sequential execution in this VM experiment.

## 9. Work Distribution

For a vector of 1,000,000 elements:

- With 2 processes: approximately 500,000 elements/process.
- With 4 processes: approximately 250,000 elements/process.

![Work Distribution](graphs/work_distribution.png)

This demonstrates the main idea of distributed vector processing: the overall workload is divided into smaller portions handled by different processes.

## 10. Correctness Verification

The same total vector sum was obtained in both sequential and MPI versions:

```text
500000500000
```

This confirms that the distributed computation produced the expected aggregate result for the tested vector.

## 11. Example Commands

### Compile MPI Program

```bash
mpicc vector_mpi.c -o vector_mpi
```

### Run with 2 Processes

```bash
mpirun --allow-run-as-root -np 2 ./vector_mpi
```

### Run with 3 Processes

```bash
mpirun --allow-run-as-root -np 3 ./vector_mpi
```

### Run with 4 Processes

```bash
mpirun --allow-run-as-root -np 4 ./vector_mpi
```

### Compile Sequential Program

```bash
gcc vector_sequential.c -o vector_sequential
```

### Run Sequential Program

```bash
./vector_sequential
```

## 12. Project Structure

The recommended final GitHub structure is:

```text
Distributed-Vector-Processing-MPI/
│
├── README.md
│
├── src/
│   ├── vector_mpi.c
│   └── vector_sequential.c
│
├── graphs/
│   ├── execution_time_comparison.png
│   ├── speedup.png
│   └── work_distribution.png
│
├── data/
│   └── performance_data.csv
│
└── screenshots/
    ├── sequential_execution.jpeg
    ├── mpi_2_process.jpeg
    ├── mpi_3_processes.jpeg
    ├── mpi_4_processes.jpeg
    ├── performance_test.jpeg
    ├── sequential_vs_distributed_processing.jpeg
    └── mpi_results.jpeg
```

## 13. Screenshots

The `screenshots/` directory contains terminal evidence of:

- Sequential vector processing.
- MPI execution.
- Multiple process configurations.
- Vector distribution.
- Local sums.
- Final reduction result.
- Performance measurements.

## 14. Conclusion

This project demonstrates how a large vector can be divided among multiple processes using MPI. Each process independently handles a portion of the vector, performs local computation, and contributes its result to a global reduction.

The experiment successfully demonstrates:

- MPI initialization.
- Process identification.
- Process-count detection.
- Work distribution.
- Local computation.
- Result aggregation.
- Sequential vs. distributed comparison.
- Performance measurement.

The measured timings also demonstrate an important practical concept: **parallel processing does not automatically guarantee faster execution**. For small workloads and a single virtual machine, MPI communication and process-management overhead can exceed the computation time.

## 15. Future Improvements

Possible extensions include:

- Test much larger vectors such as 10 million or 100 million elements.
- Repeat each experiment many times and calculate the average.
- Test 1, 2, 4 and 8 processes where the VM has enough CPU resources.
- Calculate speedup and parallel efficiency automatically.
- Compare different vector operations such as sum, minimum, maximum and average.
- Run the same program across multiple physical machines to demonstrate distributed-memory computing over a network.

---

## Author / Team

**Distributed Vector Processing using MPI**

Academic parallel-computing project implemented in C using MPI and Open MPI.
