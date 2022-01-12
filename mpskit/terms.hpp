#ifndef MPSKIT_TERMS
#define MPSKIT_TERMS

#include <itensor/mps/autompo.h>
#include <string>

#include "types.hpp"

namespace itensor
{
class AutoMPO;
} // namespace itensor

class OneSiteTerm
{
  protected:
    Real m_prefactor;
    std::string m_op;
    int m_index;

  public:
    explicit OneSiteTerm(const Real &prefactor, const std::string &op, int index);

    const Real &getPrefactor() const;
    const std::string &getOperator() const;
    const int &getIndex() const;
};

class TwoSiteTerm
{
  protected:
    Real m_prefactor;
    std::string m_op1;
    int m_index1;
    std::string m_op2;
    int m_index2;

  public:
    explicit TwoSiteTerm(const Real &prefactor, const std::string &op1, int index1, const std::string &op2, int index2);

    void orderTerm();

    const Real &getPrefactor() const;
    const std::string &getOperator1() const;
    const int &getIndex1() const;
    const std::string &getOperator2() const;
    const int &getIndex2() const;
};

namespace itensor
{
AutoMPO &operator+=(AutoMPO &lhs, const OneSiteTerm &term);
AutoMPO &operator+=(AutoMPO &lhs, const TwoSiteTerm &term);
} // namespace itensor

#endif /* MPSKIT_TERMS */
