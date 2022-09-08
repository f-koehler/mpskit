#ifndef MPSKIT_MODELS_SPIN_HALF_1D_XY_INHOM
#define MPSKIT_MODELS_SPIN_HALF_1D_XY_INHOM

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "spin_half_1d.hpp"

namespace XYInhom
{
enum CouplingDistribution : int
{
    Uniform,
    Exponential,
    Gaussian,
    Logarithmic,
    Invalid
};

NLOHMANN_JSON_SERIALIZE_ENUM(CouplingDistribution, {
                                                       {XYInhom::Invalid, nullptr},
                                                       {XYInhom::Uniform, "uniform"},
                                                       {XYInhom::Exponential, "exponential"},
                                                       {XYInhom::Gaussian, "gaussian"},
                                                       {XYInhom::Logarithmic, "logarithmic"},
                                                   })

class XYInhom : public SpinHalf1D
{
  private:
    Real m_J0;
    CouplingDistribution m_coupling_distribution;
    Real m_alpha;
    Real m_hx;
    Real m_hy;
    Real m_hz;

  public:
    explicit XYInhom(int L, const Real &J0, const CouplingDistribution &coupling_distribution, const Real &alpha,
                     const Real &hx = 0.0, const Real &hy = 0.0, const Real &hz = 0.0);
    explicit XYInhom(const json &js);

    const Real &getJ0() const;
    const CouplingDistribution &getCouplingDistribution() const;
    const Real &getAlpha() const;
    const Real &getHx() const;
    const Real &getHy() const;
    const Real &getHz() const;
};

} // namespace XYInhom

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_XY_INHOM */
