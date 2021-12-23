#ifndef DMRG_MODELS_MODEL1D
#define DMRG_MODELS_MODEL1D

#include <itensor/mps/autompo.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <vector>

#include "../terms.hpp"
#include "../types.hpp"
#include "model.hpp"

class Model1D : public Model
{
  protected:
    int m_L;
    bool m_periodic;
    std::vector<OneSiteTerm> m_one_body_terms;
    std::vector<TwoSiteTerm> m_two_body_terms;

  public:
    explicit Model1D(const itensor::SiteSet &sites, int L, bool periodic);
    virtual ~Model1D() = default;

    std::vector<OnePointFunction> generate_one_point_functions(const std::string &op, const Real &prefactor = 1.0,
                                                               bool full = true) const;
    std::vector<TwoPointFunction> generate_two_point_functions(const std::string &op1, const std::string &op2,
                                                               const Real &prefactor = 1.0, bool full = false) const;

    itensor::MPO get_hamiltonian() const override;
    virtual const std::vector<OneSiteTerm> &get_one_body_terms() const;
    virtual const std::vector<TwoSiteTerm> &get_two_body_terms() const;
    bool isPeriodic() const;
    int getL() const;
};

#endif /* DMRG_MODELS_MODEL1D */
