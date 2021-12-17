#ifndef DMRG_BOSE_HUBBARD_1D
#define DMRG_BOSE_HUBBARD_1D

#include <itensor/mps/sites/boson.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <map>
#include <string>
#include <vector>

#include "model.hpp"
#include "json.hpp"
#include "types.hpp"

class BoseHubbard1D : public Model
{
protected:
    int L;
    int N;
    Real J;
    Real U;
    bool periodic;
    itensor::Boson sites;

public:
    BoseHubbard1D(int L, int N, Real J, Real U, bool periodic);
    BoseHubbard1D(const json &j);

    itensor::MPS get_initial_state() const;
    itensor::MPO get_hamiltonian() const;
    itensor::MPO get_particle_number_operator() const;
    std::vector<Observable> get_observables() const;

    std::map<std::string, ComplexArray> compute_one_point(itensor::MPS &psi) const;
    std::map<std::string, ComplexArray> compute_two_point(itensor::MPS &psi) const;
};

#endif /* DMRG_BOSE_HUBBARD_1D */
