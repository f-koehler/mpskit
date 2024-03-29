# mpskit

[![Repository on GitHub](https://img.shields.io/badge/Repo-Github-brightgreen?logo=github)](https://github.com/f-koehler/mpskit)
[![CI](https://github.com/f-koehler/mpskit/actions/workflows/ci.yml/badge.svg)](https://github.com/f-koehler/mpskit/actions/workflows/ci.yml)
[![GitHub](https://img.shields.io/github/license/f-koehler/mpskit)](https://github.com/f-koehler/mpskit/blob/main/LICENSE.md)
![GitHub top language](https://img.shields.io/github/languages/top/f-koehler/mpskit)

mpskit is a toolkit to facilitate the investigation of discrete quantum systems (e.g. lattice models of spins and bosons) in one and two dimensions.
It employs the [ITensor](https://github.com/ITensor/ITensor) library for matrix product and tensor network states as well as the [density matrix renormalization group (DMRG) algorithm](https://en.wikipedia.org/wiki/Density_matrix_renormalization_group) to compute eigenstates.
The library allows to easily implement new systems and to compute relevant observables (expectation values, local operators, correlations, …).
Results are stored in an efficient and portable [HDF5](https://www.hdfgroup.org/solutions/hdf5/) file format that can be read by many programming languages/tools for further analysis.
Input parameters are passed via [JSON](https://en.wikipedia.org/wiki/JSON) files which are human-readable plain text files.

I tried to implement [time-evolving block decimation (TEBD)](https://en.wikipedia.org/wiki/Time-evolving_block_decimation) to compute the time-evolution but could not quite get it to work with the C++ version of ITensor.
However, I got it to work with the Julia version of ITensor, [ITensor.jl](https://github.com/ITensor/ITensors.jl) and you can use my project [MPSToolkit.jl](https://github.com/f-koehler/MPSToolkit.jl) for that.

## Available Models

### 1D Spin Systems

#### Transverse-Field Ising Model

- [Wikipedia](https://en.wikipedia.org/wiki/Transverse-field_Ising_model)
- [example input](examples/transverse_ising_1d.json)

$$
H=\sum\limits_{i=1}^{L-1}\left(
  J\sigma_{i}^{z}\sigma_{i+1}^{z}
  \right)
  +\sum\limits_{i=1}^{L}\left(
    h_x\sigma_{i}^{x}
    +h_z\sigma_{i}^{z}
  \right)
$$

#### Long-Range Transverse-Field Ising Model

$$
H=\sum\limits_{i=1}^{L-1}\sum\limits_{j=1}^{i}\left(
  \frac{J}{{|i-j|}^{\alpha}}\sigma_{i}^{z}\sigma_{i+1}^{z}
  \right)
  +\sum\limits_{i=1}^{L}\left(
    h_x\sigma_{i}^{x}
    +h_z\sigma_{i}^{z}
  \right)
$$

with an optional cutoff radius for the interaction.

#### Heisenberg Model

- [Wikipedia](https://en.wikipedia.org/wiki/Quantum_Heisenberg_model)

$$
H=\sum\limits_{i=1}^{L-1}\left(
  J_x\sigma_{i}^{x}\sigma_{i+1}^{x}
  +J_y\sigma_{i}^{y}\sigma_{i+1}^{y}
  +J_z\sigma_{i}^{z}\sigma_{i+1}^{z}
  \right)
  +\sum\limits_{i=1}^{L}\left(
    h_x\sigma_{i}^{x}
    +h_y\sigma_{i}^{y}
    +h_z\sigma_{i}^{z}
  \right)
$$

#### Long-Range Disordered XY Spin Glass

### 1D Bosonic Systems

#### Bose-Hubbard Model (1D)

- [example input](examples/bose_hubbard_1d.json)
- [Wikipedia](https://en.wikipedia.org/wiki/Bose%E2%80%93Hubbard_model)

$$
H=-J\sum\limits_{i=1}^{L-1}\left(
  b_{i}^{\dagger}b_{i+1}
  +b_{i+1}^{\dagger}b_{i}
\right)
+\frac{U}{2}\sum\limits_{i=1}^{L}
n_i(n_i-1)
$$

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

### 2. Compile the code

After step one just run

```bash
cmake --build build
```

to compile the code.
The `build/` folder will then contain the `mpskit` executable.

## Running the program

You can consult `mpskit --help` to see the command line options this programs has to offer.
