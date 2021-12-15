#ifndef DMRG_TRANSVERSE_ISING_1D
#define DMRG_TRANSVERSE_ISING_1D

#include "json.hpp"
#include "model.hpp"
#include <itensor/mps/sites/spinhalf.h>

class TransverseIsing1D : public Model
{
private:
    int L;
    double J;
    double hx;
    double hy;
    double hz;
    bool periodic;
    itensor::SpinHalf sites;

public:
    TransverseIsing1D(int L, double J, double hx, double hy, double hz, bool periodic);
    TransverseIsing1D(const json &j);

    itensor::MPS get_initial_state() const;
    itensor::MPO get_hamiltonian() const;

    itensor::MPO get_total_sigma_x() const;
    itensor::MPO get_total_sigma_y() const;
    itensor::MPO get_total_sigma_z() const;

    std::vector<Observable> get_observables() const;
};

#endif /* DMRG_TRANSVERSE_ISING_1D */
