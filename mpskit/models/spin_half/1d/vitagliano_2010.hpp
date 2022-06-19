#ifndef MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010
#define MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010

#include "../../../json.hpp"
#include "../../../types.hpp"
#include "spin_half_1d.hpp"

namespace Vitagliano2010
{

enum CouplingDistribution : int
{
    Uniform,
    ExponentialDecay,
    GaussianDecay,
    Invalid
};

NLOHMANN_JSON_SERIALIZE_ENUM(CouplingDistribution, {
                                                       {Invalid, nullptr},
                                                       {Uniform, "Uniform"},
                                                       {ExponentialDecay, "ExponentialDecay"},
                                                       {GaussianDecay, "GaussianDecay"},
                                                   })

class XXModel : public SpinHalf1D
{
  private:
    Real m_J0;
    CouplingDistribution m_coupling_distribution;

  public:
    explicit XXModel(int L, const Real &J0, const CouplingDistribution &coupling_distribution);
    explicit XXModel(const json &js);

    const Real &getJ0() const;
    const CouplingDistribution &getCouplingDist() const;
};

} // namespace Vitagliano2010

#endif /* MPSKIT_MODELS_SPIN_HALF_1D_VITAGLIANO_2010 */
