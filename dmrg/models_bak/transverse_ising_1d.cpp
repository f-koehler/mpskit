#include "transverse_ising_1d.hpp"
#include "../point_functions.hpp"

#include <complex>
#include <itensor/mps/autompo.h>
#include <itensor/util/iterate.h>
#include <nlohmann/json.hpp>
#include <xtensor/xbroadcast.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xcontainer.hpp>

using namespace std::string_literals;

TransverseIsing1D::TransverseIsing1D(int L, Real J, Real hx, Real hy, Real hz, bool periodic, bool use_paulis)
    : SpinHalf1D(L, periodic, use_paulis), J(J), hx(hx), hy(hy), hz(hz)
{
}

TransverseIsing1D::TransverseIsing1D(const json &j)
    : SpinHalf1D(j), J(j["J"].get<Real>()), hx(j["hx"].get<Real>()), hy(j["hy"].get<Real>()), hz(j["hz"].get<Real>())
{
}

auto TransverseIsing1D::get_initial_state() const -> itensor::MPS
{
    return itensor::randomMPS(sites);
}

auto TransverseIsing1D::get_hamiltonian() const -> itensor::MPO
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L - 1))
    {
        ampo += -get_prefactor(2) * J, "Sz", i, "Sz", i + 1;
    }
    if (periodic)
    {
        ampo += -get_prefactor(2) * J, "Sz", 1, "Sz", L;
    }
    for (auto i : itensor::range1(L))
    {
        ampo += -get_prefactor(1) * hx, "Sx", i;
        ampo += -get_prefactor(1) * hy, "Sy", i;
        ampo += -get_prefactor(1) * hz, "Sz", i;
    }

    return itensor::toMPO(ampo);
}

auto TransverseIsing1D::get_total_sigma_x() const -> itensor::MPO
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += get_prefactor(1) * hx, "Sx", i;
    }

    return itensor::toMPO(ampo);
}

auto TransverseIsing1D::get_total_sigma_y() const -> itensor::MPO
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += get_prefactor(1) * hx, "Sy", i;
    }

    return itensor::toMPO(ampo);
}

auto TransverseIsing1D::get_total_sigma_z() const -> itensor::MPO
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += get_prefactor(1) * hx, "Sz", i;
    }

    return itensor::toMPO(ampo);
}

auto TransverseIsing1D::get_observables() const -> std::vector<Observable>
{
    std::vector<Observable> observables = {Observable{"H", get_hamiltonian()}, Observable{"X", get_total_sigma_x()},
                                           Observable{"Y", get_total_sigma_y()}, Observable{"Z", get_total_sigma_z()}};
    return observables;
}

auto TransverseIsing1D::compute_one_point(itensor::MPS &psi) const -> std::map<std::string, ComplexArray>
{
    ComplexArray sx = xt::zeros<Complex>({L});
    ComplexArray sy = xt::zeros<Complex>({L});
    ComplexArray sz = xt::zeros<Complex>({L});
    auto func_x = OnePoint{get_prefactor(1), 1, "Sx"};
    auto func_y = OnePoint{get_prefactor(1), 1, "Sy"};
    auto func_z = OnePoint{get_prefactor(1), 1, "Sz"};
    for (auto i : itensor::range1(L))
    {
        func_x.position = i;
        func_y.position = i;
        func_z.position = i;
        sx(i - 1) = ::compute_one_point(psi, sites, func_x);
        sy(i - 1) = ::compute_one_point(psi, sites, func_y);
        sz(i - 1) = ::compute_one_point(psi, sites, func_z);
    }
    return {{"sx"s, sx}, {"sy"s, sy}, {"sz"s, sz}};
}
auto TransverseIsing1D::compute_two_point(itensor::MPS &psi) const -> std::map<std::string, ComplexArray>
{
    ComplexArray sx_sx = xt::zeros<Complex>({L, L});
    ComplexArray sy_sy = xt::zeros<Complex>({L, L});
    ComplexArray sz_sz = xt::zeros<Complex>({L, L});
    auto func_xx = TwoPoint{get_prefactor(2), 1, "Sx", 1, "Sx"};
    auto func_yy = TwoPoint{get_prefactor(2), 1, "Sy", 1, "Sy"};
    auto func_zz = TwoPoint{get_prefactor(2), 1, "Sz", 1, "Sz"};
    for (auto i : itensor::range1(L))
    {
        func_xx.position1 = i;
        func_yy.position1 = i;
        func_zz.position1 = i;
        for (auto j : itensor::range1(L))
        {
            func_xx.position2 = j;
            func_yy.position2 = j;
            func_zz.position2 = j;
            sx_sx(i - 1, j - 1) = ::compute_two_point(psi, sites, func_xx);
            sy_sy(i - 1, j - 1) = ::compute_two_point(psi, sites, func_yy);
            sz_sz(i - 1, j - 1) = ::compute_two_point(psi, sites, func_zz);
        }
    }
    return {{"sz_sz"s, sz_sz}};
}