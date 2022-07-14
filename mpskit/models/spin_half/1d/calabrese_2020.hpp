#ifndef MPSKIT_MODELS_SPIN_HALF_1D_CALABRESE_2020
#define MPSKIT_MODELS_SPIN_HALF_1D_CALABRESE_2020

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "spin_half_1d.hpp"

class Calabrese2020 : public SpinHalf1D
{
  protected:
    Real m_delta;

  public:
    Calabrese2020(const json &js);

    const Real &getDelta() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_CALABRESE_2020 */
