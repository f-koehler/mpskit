#ifndef DMRG_MODELS_BOSONIC_1D
#define DMRG_MODELS_BOSONIC_1D

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <map>
#include <string>

#include "model_1d.hpp"

struct Observable;

class Bosonic1D : public Model1D
{
  protected:
    bool m_conserve_N;
    int m_max_N;

  public:
    explicit Bosonic1D(int L, bool periodic, bool conserve_N, int max_N);

    itensor::MPS get_initial_state() const override;
    itensor::MPO get_particle_number_operator() const;
    std::map<std::string, Observable> get_observables() const override;
    std::map<std::string, std::vector<OnePointFunction>> get_one_point_functions() const override;
    std::map<std::string, std::vector<TwoPointFunction>> get_two_point_functions() const override;

    bool doesConserveN() const;
    int getMaxN() const;
};

#endif /* DMRG_MODELS_BOSONIC_1D */
