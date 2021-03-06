#ifndef MPSKIT_MODELS_SPIN_HALF_1D_HEISENBERG_1D
#define MPSKIT_MODELS_SPIN_HALF_1D_HEISENBERG_1D

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "spin_half_1d.hpp"

class Heisenberg1D : public SpinHalf1D
{
  protected:
    Real m_Jx;
    Real m_Jy;
    Real m_Jz;
    Real m_hx;
    Real m_hy;
    Real m_hz;

  public:
    explicit Heisenberg1D(int L, bool periodic, const Real &Jx, const Real &Jy, const Real &Jz, const Real &hx,
                          const Real &hy, const Real &hz);
    explicit Heisenberg1D(const json &js);

    const Real &getJx() const;
    const Real &getJy() const;
    const Real &getJz() const;
    const Real &getHx() const;
    const Real &getHy() const;
    const Real &getHz() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_HEISENBERG_1D */
