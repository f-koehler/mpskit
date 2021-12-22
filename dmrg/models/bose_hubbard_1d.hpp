#ifndef DMRG_MODELS_BOSE_HUBBARD_1D
#define DMRG_MODELS_BOSE_HUBBARD_1D

#include "../json.hpp"
#include "../types.hpp"
#include "bosonic_1d.hpp"

class BoseHubbard1D : public Bosonic1D
{
  protected:
    Real m_J;
    Real m_U;
    Real m_mu;

  public:
    explicit BoseHubbard1D(int L, bool periodic, bool conserve_N, int max_N, const Real &J, const Real &U,
                           const Real &mu);
    explicit BoseHubbard1D(const json &js);

    const Real &getJ() const;
    const Real &getU() const;
    const Real &getMu() const;
};

#endif /* DMRG_MODELS_BOSE_HUBBARD_1D */
