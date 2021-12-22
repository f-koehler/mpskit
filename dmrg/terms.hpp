#ifndef DMRG_TERMS
#define DMRG_TERMS

#include <itensor/mps/autompo.h>
#include <string>

#include "types.hpp"

namespace itensor
{
class AutoMPO;
} // namespace itensor

struct OneSiteTerm
{
    Real prefactor;
    std::string op;
    int index;
};

struct TwoSiteTerm
{
    Real prefactor;
    std::string op1;
    int index1;
    std::string op2;
    int index2;
};

namespace itensor
{
AutoMPO &operator+=(AutoMPO &lhs, const OneSiteTerm &term);
AutoMPO &operator+=(AutoMPO &lhs, const TwoSiteTerm &term);
} // namespace itensor

#endif /* DMRG_TERMS */
