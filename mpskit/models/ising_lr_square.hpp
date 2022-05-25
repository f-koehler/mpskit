#ifndef MPSKIT_MODELS_ISING_LR_SQUARE
#define MPSKIT_MODELS_ISING_LR_SQUARE

#include "../json.hpp"
#include "../types.hpp"
#include "spin_half_square.hpp"

class IsingLRSquare : public SpinHalfSquare
{
  protected:
    Real m_J;
    Real m_hx;
    Real m_hz;
    Real m_alpha;
    Real m_cutoff_radius;

  public:
    explicit IsingLRSquare(const json &js);

    const Real &getJ() const;
    const Real &getHx() const;
    const Real &getHz() const;
    const Real &getAlpha() const;
    const Real &getCutoffRadius() const;
};

#endif /* MPSKIT_MODELS_ISING_LR_SQUARE */
