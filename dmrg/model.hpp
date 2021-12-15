#ifndef DMRG_MODEL
#define DMRG_MODEL

#include <functional>
#include <vector>

#include <itensor/mps/autompo.h>

#include "point_functions.hpp"

struct Observable;

class Model
{
public:
    virtual itensor::MPO get_hamiltonian() const = 0;
    virtual itensor::MPS get_initial_state() const = 0;

    virtual std::vector<Observable> get_observables() const = 0;
};

struct Observable
{
    std::string name;
    itensor::MPO mpo;
    bool compute_variance = true;
};

#endif /* DMRG_MODEL */
