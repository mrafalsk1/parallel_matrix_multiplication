# ADR

An architectural decision record (ADR) is a document that describes a choice made about a significasnt aspect of the software architecture.

## ADR 1: Use of Hybrid Parallelism with MPI and Pthreads

### Context

Matrix multiplicatoin is a computationnally intensive task required in HPC applications. This implementation seeks to achieve scalability and performance by combining MPI and threads.

### Decision

The program uses MPI to divide matrix A into submatrices, which are distributed among MPI processes. Matrix B is broadcast to all processes. Each process uses threads to parallelize the computation of its assigned submatrix, distributing columns of the result matrix across threads 

### Consequences

#### Pros

 - Improved Scalability
 - Modularity

#### Cons

- Complexity
- Static assummptions


## ADR 2: Static Distribution of Worload

### Context

The current implementation statically allocates threads and processes to fixed rows and columns of the matrix. This approach simplifies workload distribution and reduces the overhead of dynamic scheduling.

### Decision

Each MPI process is assigned an equal numver of rows from matrix A using MPI_Scatter, with rows distributed evenly across all processes. Each thread within a process is responsible for computing one oclumn of the resulting submatrix.

### Consequences

#### Pros

- Simplicity
- Predictable Execution

#### Cons

- Poor fault tolerance
- Limited Flexibility

## ADR 3: Thread-Level Parallelism for Column Computation

### Context

Within each process, threads are used to parallelize the computation of matrix columns. This decision was made to leverage multi-core processors commonly available in modern HPC nodes.

### Decision

Threads are created using pthread to calculate each column of the result matrix. Each thread works on a subset of the computation by iterating through the assigned columns, and all threads operate on the same copy of matrix B broadcast by MPI.

### Consequences

#### Pros

- Faster Computation
- Simplifies the memory model

#### Cons

- Overhead
- Scalability limit 

