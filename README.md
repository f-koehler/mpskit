# mpskit

[![CI](https://github.com/f-koehler/dmrg/actions/workflows/ci.yml/badge.svg)](https://github.com/f-koehler/dmrg/actions/workflows/ci.yml)

mpskit is a toolkit to facilitate the investigation of discrete quantum systems (e.g. lattice models of spins and bosons) in one and two dimensions.
It employs the [ITensor](https://github.com/ITensor/ITensor) library for matrix product and tensor network states as well as the [density matrix renormalization group (DMRG) algorithm](https://en.wikipedia.org/wiki/Density_matrix_renormalization_group) to compute eigenstates.
The library allows to easily implement new systems and to compute relevant observables (expectation values, local operators, correlations, …).
Results are stored in an efficient and portable [HDF5](https://www.hdfgroup.org/solutions/hdf5/) file format that can be read by many programming languages/tools for further analysis.
Input parameters are passed via [JSON](https://en.wikipedia.org/wiki/JSON) files which are human-readable plain text files.

I tried to implement [time-evolving block decimation (TEBD)](https://en.wikipedia.org/wiki/Time-evolving_block_decimation) to compute the time-evolution but could not quite get it to work with the C++ version of ITensor.
However, I got it to work with the Julia version of ITensor, [ITensor.jl](https://github.com/ITensor/ITensors.jl) and you can use my project [MPSToolkit.jl](https://github.com/f-koehler/MPSToolkit.jl) for that.

## Available Models

- Transverse-Field Ising Model (1D)
  - [example input](examples/transverse_ising_1d.json)
  - [Wikipedia](https://en.wikipedia.org/wiki/Transverse-field_Ising_model)
- Transverse-Field Ising Model (2D square lattice)
  - [example input](examples/transverse_ising_2d.json)
  - [Wikipedia](https://en.wikipedia.org/wiki/Transverse-field_Ising_model)
- Heisenberg Model (1D)
  - [Wikipedia](https://en.wikipedia.org/wiki/Quantum_Heisenberg_model)
- Bose-Hubbard Model (1D)
  - [example input](examples/bose_hubbard_1d.json)
  - [Wikipedia](https://en.wikipedia.org/wiki/Bose%E2%80%93Hubbard_model)

A new model can be added by adding it to the `models` source folder (take inspiration from [`transverse_ising_1d.hpp`](https://github.com/f-koehler/dmrg/blob/main/mpskit/models/transverse_ising_1d.hpp), [`transverse_ising_1d.cpp`](https://github.com/f-koehler/dmrg/blob/main/mpskit/models/transverse_ising_1d.cpp), etc.).
Afterwards it has to be registered in the model registry (see [`registry.cpp`](https://github.com/f-koehler/dmrg/blob/main/mpskit/models/registry.cpp)).

## Dependencies

- `cmake`
- `gcc>=8` or `clang>=6`
- `boost`
- `hdf5`

## Compilation

Compilation of the project is an easy two-step process.

### 1. Configure the project

Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

from the project directory to create a `build/` directory.
You can specify the C++ compiler by setting the `CXX` environment variable, e.g. `export CXX=clang++`, before invoking the CMake command.

If you are developing based on this you might want to create a Debug build for testing instead (not recommended for actual simulation due to reduced performance).
In order to achieve this replace the CMake command above with

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

## 2. Compile the code

After step one just run

```bash
cmake --build build
```

to compile the code.
The `build/` folder will then contain the `mpskit` executable.

## Running the code

You can consult `mpskit --help` to see the command line options this programs has to offer.
