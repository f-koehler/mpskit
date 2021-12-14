#ifndef DMRG_MODEL
#define DMRG_MODEL

#include <functional>
#include <vector>

#include <itensor/mps/autompo.h>

struct Observable;
struct TwoPointCorrelation;

class Model
{
    virtual itensor::MPO get_hamiltonian() const = 0;
    virtual std::vector<Observable> get_observables() const = 0;
    virtual std::vector<TwoPointCorrelation> get_two_point_correlations() const = 0;
};

struct Observable
{
    std::string name;
    itensor::MPO mpo;
    bool compute_variance = true;
};

struct TwoPointCorrelation
{
    std::string name;
    itensor::MPO mpo1, mpo2;
};

#endif /* DMRG_MODEL */
