#ifndef MPSKIT_MODELS_SPIN_HALF_1D
#define MPSKIT_MODELS_SPIN_HALF_1D

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/sites/spinhalf.h>
#include <map>
#include <string>

#include "model_1d.hpp"

struct Observable;

class SpinHalf1D : public Model1D
{
  protected:
    bool m_conserve_Sz;
    bool m_conserve_parity;

  public:
    explicit SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity);

    itensor::MPS get_initial_state() const override;
    itensor::MPO get_total_sx_operator() const;
    itensor::MPO get_total_sy_operator() const;
    itensor::MPO get_total_sz_operator() const;
    std::map<std::string, Observable> get_observables() const override;
    std::map<std::string, std::vector<OnePointFunction>> get_one_point_functions() const override;
    std::map<std::string, std::vector<TwoPointFunction>> get_two_point_functions() const override;

    bool doesConserveSz() const;
    bool doesConserveParity() const;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_1D */
