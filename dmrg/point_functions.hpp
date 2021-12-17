#ifndef DMRG_POINT_FUNCTIONS
#define DMRG_POINT_FUNCTIONS

#include "types.hpp"
#include <itensor/itensor.h>
#include <itensor/mps/mps.h>

struct OnePoint
{
    Real prefactor;
    int position;
    std::string opname;
};

struct TwoPoint
{
    Real prefactor;
    int position1;
    std::string opname1;
    int position2;
    std::string opname2;
};

template <typename SiteSet>
Complex compute_one_point(itensor::MPS &psi, const SiteSet &sites, const OnePoint &spec)
{
    psi.position(spec.position);
    auto ket = psi(spec.position);
    auto bra = itensor::dag(itensor::prime(ket, "Site"));
    auto op = spec.prefactor * itensor::op(sites, spec.opname, spec.position);
    return itensor::eltC(bra * op * ket);
}

template <typename SiteSet>
Complex compute_two_point(itensor::MPS &psi, const SiteSet &sites, const TwoPoint &spec)
{
    if (spec.position1 == spec.position2)
    {
        psi.position(spec.position1);
        auto ket = psi(spec.position1);
        auto bra = itensor::dag(itensor::prime(ket, "Site"));
        auto op = spec.prefactor * itensor::multSiteOps(itensor::op(sites, spec.opname1, spec.position1), itensor::op(sites, spec.opname2, spec.position1));
        return itensor::eltC(bra * op * ket);
    }

    psi.position(spec.position1);
    auto bondket = psi(spec.position1) * psi(spec.position2);
    auto bondbra = itensor::dag(itensor::prime(bondket, "Site"));
    auto op = spec.prefactor * itensor::op(sites, spec.opname1, spec.position1) * itensor::op(sites, spec.opname2, spec.position2);
    return itensor::eltC(bondbra * op * bondket);
}

#endif /* DMRG_POINT_FUNCTIONS */
