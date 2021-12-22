#include "terms.hpp"

namespace itensor
{
AutoMPO &operator+=(AutoMPO &lhs, const OneSiteTerm &term)
{
    lhs += term.prefactor, term.op, term.index;
    return lhs;
}
AutoMPO &operator+=(AutoMPO &lhs, const TwoSiteTerm &term)
{
    lhs += term.prefactor, term.op1, term.index1, term.op2, term.index2;
    return lhs;
}
} // namespace itensor