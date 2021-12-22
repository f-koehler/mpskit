#include "terms.hpp"

#include <itensor/mps/autompo.h>

namespace itensor
{
auto operator+=(AutoMPO &lhs, const OneSiteTerm &term) -> AutoMPO &
{
    lhs += term.prefactor, term.op, term.index;
    return lhs;
}
auto operator+=(AutoMPO &lhs, const TwoSiteTerm &term) -> AutoMPO &
{
    lhs += term.prefactor, term.op1, term.index1, term.op2, term.index2;
    return lhs;
}
} // namespace itensor