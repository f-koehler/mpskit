#ifndef DMRG_MODELS_SPIN_HALF_1D
#define DMRG_MODELS_SPIN_HALF_1D

#include "model_1d.hpp"

class SpinHalf1D : public Model1D
{
  protected:
    bool m_conserve_Sz;
    bool m_conserve_parity;

  public:
    explicit SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity);

    itensor::MPS get_initial_state() const override;

    bool doesConserveSz() const;
    bool doesConserveParity() const;
};

#endif /* DMRG_MODELS_SPIN_HALF_1D */
