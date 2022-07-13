#ifndef MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010
#define MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "spin_half_1d.hpp"

namespace Vitagliano2010
{

enum CouplingFunction : int
{
    Uniform,
    Exponential,
    Gaussian,
    Invalid
};

NLOHMANN_JSON_SERIALIZE_ENUM(CouplingFunction, {
                                                   {Invalid, nullptr},
                                                   {Uniform, "uniform"},
                                                   {Exponential, "exponential"},
                                                   {Gaussian, "gaussian"},
                                               })

class XXModel : public SpinHalf1D
{
  private:
    Real m_J0;
    CouplingFunction m_coupling_function;
    Real m_hx;
    Real m_hy;
    Real m_hz;

  public:
    explicit XXModel(int L, const Real &J0, const CouplingFunction &coupling_function, const Real &hx = 0.0,
                     const Real &hy = 0.0, const Real &hz = 0.0);
    explicit XXModel(const json &js);

    const Real &getJ0() const;
    const CouplingFunction &getCouplingFunction() const;
    const Real &getHx() const;
    const Real &getHy() const;
    const Real &getHz() const;
};

} // namespace Vitagliano2010

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010 */
