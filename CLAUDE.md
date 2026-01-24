# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is an MS-MPI learning repository containing MPI (Message Passing Interface) example programs in C for Windows. MS-MPI is Microsoft's implementation of the MPI standard for parallel computing.

## Build System

This repository uses **MSVC (Microsoft Visual C++ Compiler)** for compiling MPI programs.

### Standard Build Command

```cmd
cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" <source.c> /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
```

Components:
- `/I"..."`: Include path for MPI headers (mpi.h)
- `/link /LIBPATH:"..."`: Linker library path for MS-MPI
- `msmpi.lib`: MS-MPI library to link against

### Alternative: MinGW-w64

```cmd
gcc -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" <source.c> -o <output.exe> -lmsmpi
```

## Running MPI Programs

Use `mpiexec` to launch MPI programs with multiple processes:

```cmd
mpiexec -n <num_processes> <program.exe> [arguments]
```

Examples:
- `mpiexec -n 4 hello_mpi.exe` - Run with 4 processes
- `mpiexec -n 8 monte_carlo_pi.exe 1000000000` - Run with 8 processes and arguments

## Code Architecture

### MPI Program Structure

All MPI programs follow this basic pattern:

1. **Initialization**: `MPI_Init(&argc, &argv)` - Must be called before any MPI functions
2. **Get rank and size**:
   - `MPI_Comm_rank(MPI_COMM_WORLD, &rank)` - Get process ID
   - `MPI_Comm_size(MPI_COMM_WORLD, &size)` - Get total number of processes
3. **Parallel computation**: Each process performs work based on its rank
4. **Communication** (if needed): Use MPI collectives like `MPI_Reduce`, `MPI_Gather`, etc.
5. **Finalization**: `MPI_Finalize()` - Must be called before program exit

### Existing Examples

- **hello_mpi.c**: Basic MPI program demonstrating process identification
  - Shows rank, total processes, and processor name

- **monte_carlo_pi.c**: Parallel Monte Carlo π estimation
  - Distributes random sampling across processes
  - Uses `MPI_Reduce` to aggregate results from all processes
  - Rank 0 performs final calculation and output
  - Work distribution: `localN = N/size + (rank < N%size ? 1 : 0)` ensures even distribution

## Key Patterns

### Work Distribution

Divide total work among processes, handling remainder:
```c
long long localN = N/size + (rank < N%size ? 1 : 0);
```

### Random Seeds in Parallel

Each process needs different seed to avoid identical random sequences:
```c
unsigned int seed = base_seed + rank * offset;
```

### Collective Operations

Rank 0 typically handles output after reduction:
```c
MPI_Reduce(&local_data, &total_data, 1, MPI_TYPE, MPI_OP, 0, MPI_COMM_WORLD);
if (rank == 0) {
    // Process and output results
}
```
