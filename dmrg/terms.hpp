#ifndef DMRG_TERMS
#define DMRG_TERMS

#include "types.hpp"
#include <itensor/mps/autompo.h>
#include <string>

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
