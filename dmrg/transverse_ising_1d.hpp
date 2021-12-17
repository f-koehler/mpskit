#ifndef DMRG_TRANSVERSE_ISING_1D
#define DMRG_TRANSVERSE_ISING_1D

#include <itensor/mps/sites/spinhalf.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <map>
#include <string>
#include <vector>

#include "json.hpp"
#include "model.hpp"
#include "types.hpp"

class TransverseIsing1D : public Model
{
protected:
    int L;
    Real J;
    Real hx;
    Real hy;
    Real hz;
    bool periodic;
    itensor::SpinHalf sites;

public:
    TransverseIsing1D(int L, Real J, Real hx, Real hy, Real hz, bool periodic);
    TransverseIsing1D(const json &j);

    itensor::MPS get_initial_state() const;
    itensor::MPO get_hamiltonian() const;

    itensor::MPO get_total_sigma_x() const;
    itensor::MPO get_total_sigma_y() const;
    itensor::MPO get_total_sigma_z() const;

    std::vector<Observable> get_observables() const;
    std::map<std::string, ComplexArray> compute_one_point(itensor::MPS &psi) const;
    std::map<std::string, ComplexArray> compute_two_point(itensor::MPS &psi) const;
};

#endif /* DMRG_TRANSVERSE_ISING_1D */
