#ifndef MPSKIT_MODELS_SPIN_HALF_2D_XY_SPIN_GLASS_SQUARE
#define MPSKIT_MODELS_SPIN_HALF_2D_XY_SPIN_GLASS_SQUARE

#include "../../../json.hpp"
#include "spin_half_square.hpp"

class XYSpinGlassSquare : public SpinHalfSquare
{
  protected:
    Real m_alpha;

  public:
    XYSpinGlassSquare(const json &js);

    const Real &getAlpha() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_2D_XY_SPIN_GLASS_SQUARE */
