#ifndef MPSKIT_MODELS_ISING_LR_1D
#define MPSKIT_MODELS_ISING_LR_1D

#include "../json.hpp"
#include "../types.hpp"
#include "transverse_ising_1d.hpp"

class IsingLR1D : public TransverseIsing1D
{
  protected:
    Real m_alpha;
    Real m_cutoff;

  public:
    IsingLR1D(const json &js);

    const Real &getAlpha() const;
    const Real &getCutoff() const;
};

#endif /* MPSKIT_MODELS_ISING_LR_1D */
