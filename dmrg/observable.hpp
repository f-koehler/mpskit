#ifndef DMRG_OBSERVABLE
#define DMRG_OBSERVABLE

#include "types.hpp"
#include <itensor/mps/mpo.h>
#include <optional>

struct Observable
{
    itensor::MPO op;
    Complex value, squared, variance;

    explicit Observable(const itensor::MPO &op);
    void operator()(const itensor::MPS &mps);
};

#endif /* DMRG_OBSERVABLE */
