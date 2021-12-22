#ifndef DMRG_MODELS_MODEL1D
#define DMRG_MODELS_MODEL1D

#include "../types.hpp"
#include "model.hpp"
#include <itensor/mps/autompo.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <vector>

struct OneBodyTerm;
struct TwoBodyTerm;

class Model1D : public Model
{
  protected:
    int m_L;
    bool m_periodic;
    std::vector<OneBodyTerm> m_one_body_terms;
    std::vector<TwoBodyTerm> m_two_body_terms;

  public:
    explicit Model1D(const itensor::SiteSet &sites, int L, bool periodic);
    virtual ~Model1D() = default;

    itensor::MPO get_hamiltonian() const override;
    virtual const std::vector<OneBodyTerm> &get_one_body_terms() const;
    virtual const std::vector<TwoBodyTerm> &get_two_body_terms() const;

    bool isPeriodic() const;
    int getL() const;
};

struct OneBodyTerm
{
    Real prefactor;
    std::string op;
    int index;
};

struct TwoBodyTerm
{
    Real prefactor;
    std::string op1;
    int index1;
    std::string op2;
    int index2;
};

namespace itensor
{
AutoMPO &operator+=(AutoMPO &lhs, const OneBodyTerm &term);
AutoMPO &operator+=(AutoMPO &lhs, const TwoBodyTerm &term);
} // namespace itensor

#endif /* DMRG_MODELS_MODEL1D */
