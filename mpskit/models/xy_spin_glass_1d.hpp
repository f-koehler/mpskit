#ifndef MPSKIT_MODELS_XY_SPIN_GLASS_1D
#define MPSKIT_MODELS_XY_SPIN_GLASS_1D

#include <cstdint>

#include "../json.hpp"
#include "../types.hpp"
#include "spin_half_1d.hpp"

class XYSpinGlass1D : public SpinHalf1D
{
  protected:
    Real m_alpha;

  public:
    XYSpinGlass1D(const json &js);

    const Real &getAlpha() const;
};

#endif /* MPSKIT_MODELS_XY_SPIN_GLASS_1D */
