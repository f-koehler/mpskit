#ifndef DMRG_BOSE_HUBBARD_1D
#define DMRG_BOSE_HUBBARD_1D

#include "model.hpp"
#include "json.hpp"
#include <itensor/mps/sites/boson.h>

class BoseHubbard1D : public Model
{
protected:
    int L;
    int N;
    double J;
    double U;
    bool periodic;
    itensor::Boson sites;

public:
    BoseHubbard1D(int L, int N, double J, double U, bool periodic);
    BoseHubbard1D(const json &j);

    itensor::MPS get_initial_state() const;
    itensor::MPO get_hamiltonian() const;
    itensor::MPO get_particle_number_operator() const;
    std::vector<Observable> get_observables() const;

    std::map<std::string, xt::xarray<double>> compute_one_point(itensor::MPS &psi) const;
    std::map<std::string, xt::xarray<double>> compute_two_point(itensor::MPS &psi) const;
};

#endif /* DMRG_BOSE_HUBBARD_1D */
