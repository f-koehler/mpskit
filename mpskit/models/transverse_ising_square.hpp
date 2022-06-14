#ifndef MPSKIT_MODELS_TRANSVERSE_ISING_SQUARE
#define MPSKIT_MODELS_TRANSVERSE_ISING_SQUARE

#include "../json.hpp"
#include "spin_half_square.hpp"

class TransverseIsingSquare : public SpinHalfSquare
{
  protected:
    Real m_J;
    Real m_hx;
    Real m_hz;

  public:
    explicit TransverseIsingSquare(int Lx, int Ly, const Real &J, const Real &hx, const Real &hz);
    explicit TransverseIsingSquare(const json &js);

    const Real &getJ() const;
    const Real &getHx() const;
    const Real &getHz() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_TRANSVERSE_ISING_SQUARE */
