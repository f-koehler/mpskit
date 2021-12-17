#ifndef DMRG_TRANSVERSE_ISING_1D
#define DMRG_TRANSVERSE_ISING_1D

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/sites/spinhalf.h>
#include <map>
#include <string>
#include <vector>

#include "../json.hpp"
#include "../model.hpp"
#include "../types.hpp"

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
    explicit TransverseIsing1D(int L, Real J, Real hx, Real hy, Real hz, bool periodic);
    explicit TransverseIsing1D(const json &j);

    itensor::MPS get_initial_state() const override;
    itensor::MPO get_hamiltonian() const override;

    itensor::MPO get_total_sigma_x() const;
    itensor::MPO get_total_sigma_y() const;
    itensor::MPO get_total_sigma_z() const;

    std::vector<Observable> get_observables() const override;
    std::map<std::string, ComplexArray> compute_one_point(itensor::MPS &psi) const override;
    std::map<std::string, ComplexArray> compute_two_point(itensor::MPS &psi) const override;
};

#endif /* DMRG_TRANSVERSE_ISING_1D */
