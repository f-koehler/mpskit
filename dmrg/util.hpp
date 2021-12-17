#ifndef DMRG_UTIL
#define DMRG_UTIL

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/sweeps.h>

#include "json.hpp"
#include "types.hpp"

Complex compute_expectation_value(const itensor::MPS &psi, const itensor::MPO &op);
Complex compute_expectation_value_2(const itensor::MPS &psi, const itensor::MPO &op);
Complex compute_variance(const itensor::MPS &psi, const itensor::MPO &op);
itensor::Sweeps get_sweeps_from_json(const json &j);

#endif /* DMRG_UTIL */
