#ifndef DMRG_BOSE_HUBBARD_1D
#define DMRG_BOSE_HUBBARD_1D

#include "model.hpp"
#include "json.hpp"
#include <fmt/core.h>
#include <string_view>

#include <itensor/mps/sites/boson.h>

class BoseHubbard1D
{
private:
    int L;
    int N;
    double J;
    double U;
    bool periodic;
    const itensor::Boson sites;

public:
    BoseHubbard1D(int L, int N, double J, double U, bool periodic);

    static BoseHubbard1D from_json(const json &j);

    itensor::MPS get_initial_state() const;
    itensor::MPO get_hamiltonian() const;
    itensor::MPO get_particle_number_operator() const;
    itensor::MPO get_site_occupation_operator(int j) const;
    std::vector<Observable> get_observables() const;
    std::vector<TwoPointCorrelation> get_two_point_correlations() const;
};

#endif /* DMRG_BOSE_HUBBARD_1D */
