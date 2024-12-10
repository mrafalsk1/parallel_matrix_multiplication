# Parallel Matrix Multiplication

This repository implements a parallel matrix multiplication algorithm using a hybrid approach that combines **MPI (Message Passing Interface)** for distributed memory systems and **POSIX threads (pthreads)** for shared memory systems. It is designed to perform scalable and efficient matrix multiplication on modern HPC clusters.

---

## Features

- **Hybrid Parallelism**: Combines MPI and pthreads to utilize both inter-process and intra-process parallelism.
- **Dynamic Communication**: Uses MPI for scattering, broadcasting, and gathering matrix data between processes.
- **Thread-Level Computation**: Implements thread-based parallelism for intra-process matrix column computations.
- **Scalability**: Can handle large matrix sizes distributed across multiple processes and cores.

---

## Prerequisites

### Tools and Libraries
1. **MPI Library**: Install `mpich` or `openmpi`.
2. **PThreads**: Ensure your compiler supports pthreads (most modern compilers like GCC do).
3. **C Compiler**: GCC or any equivalent.

### System Requirements
- A multi-core machine or a distributed system with multiple nodes.
- Sufficient memory for storing matrices.

---

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/mrafalsk1/parallel_matrix_multiplication.git
   cd parallel_matrix_multiplication
   ```
2. Compile the program using an MPI-enabled compiler:
   ```bash
   mpicc -o main main.c -lpthread
   ```

---

## Usage

### Running the Program
To execute the program on `N` processes:
```bash
mpirun -np N ./parallel_matrix_multiplication
```

### Output
The program performs the following:
1. Initializes two random matrices (`A` and `B`).
2. Displays the input matrices (on rank 0).
3. Computes the product matrix (`C`) using hybrid parallelism.
4. Displays the result matrix (`C`) (on rank 0).

---

## Code Overview

### Key Functions
- **`initialize_matrix(int *matrix, int rows, int columns)`**: Populates a matrix with random integers.
- **`print_matrix(int *matrix, int rows, int columns)`**: Prints the matrix in a human-readable format.
- **`multiply_matrix(void *arg)`**: Computes a matrix column product using threads.

### Parallelization Workflow
1. **MPI**:
   - Scatters rows of matrix `A` across all processes.
   - Broadcasts matrix `B` to all processes.
   - Gathers computed submatrices from all processes to rank 0.

2. **Pthreads**:
   - Within each process, threads handle computation for different columns of the result matrix.

---

## Example Output
```
MATRIX A
1 2 3 4
5 6 7 8
9 0 1 2
3 4 5 6

MATRIX B
7 8 9 1
2 3 4 5
6 7 8 9
0 1 2 3

RESULT MATRIX C
45 54 63 45
125 158 191 123
69 90 111 66
75 94 113 66
```

---

## Known Issues
- Assumes that the number of processes evenly divides the matrix dimensions.
