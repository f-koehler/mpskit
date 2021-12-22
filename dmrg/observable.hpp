#ifndef DMRG_OBSERVABLE
#define DMRG_OBSERVABLE

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <optional>

#include "types.hpp"

struct Observable
{
    itensor::MPO op;
    Complex value, squared, variance;

    explicit Observable(itensor::MPO op);
    Complex &operator()(const itensor::MPS &mps);
};

#endif /* DMRG_OBSERVABLE */
