#ifndef DMRG_MODELS_SPIN_HALF_1D
#define DMRG_MODELS_SPIN_HALF_1D

#include "model_1d.hpp"
#include <itensor/mps/sites/spinhalf.h>

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

    bool doesConserveSz() const;
    bool doesConserveParity() const;
};

#endif /* DMRG_MODELS_SPIN_HALF_1D */
