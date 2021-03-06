#ifndef MPSKIT_MODELS_SPIN_HALF_1D_ISING_LR_1D
#define MPSKIT_MODELS_SPIN_HALF_1D_ISING_LR_1D

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "transverse_ising_1d.hpp"

class IsingLR1D : public TransverseIsing1D
{
  protected:
    Real m_alpha;
    Real m_cutoff_radius;

  public:
    IsingLR1D(const json &js);

    const Real &getAlpha() const;
    const Real &getCutoffRadius() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_ISING_LR_1D */
