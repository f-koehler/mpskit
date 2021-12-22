#ifndef DMRG_MODELS_BOSONIC_1D
#define DMRG_MODELS_BOSONIC_1D

#include "model_1d.hpp"

class Bosonic1D : public Model1D
{
  protected:
    bool m_conserve_N;
    int m_max_N;

  public:
    explicit Bosonic1D(int L, bool periodic, bool conserve_N, int max_N);

    itensor::MPS get_initial_state() const override;

    bool doesConserveN() const;
    int getMaxN() const;
};

#endif /* DMRG_MODELS_BOSONIC_1D */
