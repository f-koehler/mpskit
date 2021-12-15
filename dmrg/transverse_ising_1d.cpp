#include "transverse_ising_1d.hpp"
#include <fmt/core.h>

using namespace std::string_literals;

TransverseIsing1D::TransverseIsing1D(int L, double J, double hx, double hy, double hz, bool periodic) : L(L), J(J), hx(hx), hy(hy), hz(hz), periodic(periodic), sites(L, {"ConserveQNs=", false})
{
}

TransverseIsing1D::TransverseIsing1D(const json &j) : L(j["L"].get<int>()), J(j["J"].get<double>()), hx(j["hx"].get<double>()), hy(j["hy"].get<double>()), hz(j["hz"].get<double>()), periodic(j["periodic"].get<bool>()), sites(L, {"ConserveQNs=", false})
{
}

itensor::MPS TransverseIsing1D::get_initial_state() const
{
    // auto state = itensor::InitState(L);
    // for (auto i : itensor::range1(L))
    // {
    //     if (i % 2)
    //         state.set(i, "Up");
    //     else
    //         state.set(i, "Down");
    // }
    // return itensor::MPS(state);
    return itensor::randomMPS(sites);
}

itensor::MPO TransverseIsing1D::get_hamiltonian() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L - 1))
    {
        ampo += -4 * J, "Sz", i, "Sz", i + 1;
    }
    if (periodic)
    {
        ampo += -4 * J, "Sz", 1, "Sz", L;
    }
    for (auto i : itensor::range1(L))
    {
        ampo += -2 * hx, "Sx", i;
        ampo += -2 * hy, "Sy", i;
        ampo += -2 * hz, "Sz", i;
    }

    return itensor::toMPO(ampo);
}

itensor::MPO TransverseIsing1D::get_total_sigma_x() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += 2 * hx, "Sx", i;
    }

    return itensor::toMPO(ampo);
}

itensor::MPO TransverseIsing1D::get_total_sigma_y() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += 2 * hx, "Sy", i;
    }

    return itensor::toMPO(ampo);
}

itensor::MPO TransverseIsing1D::get_total_sigma_z() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += 2 * hx, "Sz", i;
    }

    return itensor::toMPO(ampo);
}

std::vector<Observable> TransverseIsing1D::get_observables() const
{
    std::vector<Observable> observables = {
        Observable{"H", get_hamiltonian()},
        Observable{"X", get_total_sigma_x()},
        Observable{"Y", get_total_sigma_y()},
        Observable{"Z", get_total_sigma_z()}};
    return observables;
}

std::map<std::string, xt::xarray<double>> TransverseIsing1D::compute_one_point(itensor::MPS &psi) const
{
    xt::xarray<double> sz_i = xt::zeros<double>({L});
    auto func = OnePoint{2.0, 1, "Sz"};
    for (auto i : itensor::range1(L))
    {
        func.position = i;
        sz_i(i - 1) = ::compute_one_point(psi, sites, func);
    }
    return {{"sz_i"s, sz_i}};
}
std::map<std::string, xt::xarray<double>> TransverseIsing1D::compute_two_point(itensor::MPS &psi) const
{
    xt::xarray<double> sz_i_sz_j = xt::zeros<double>({L, L});
    auto func = TwoPoint{4.0, 1, "Sz", 1, "Sz"};
    for (auto i : itensor::range1(L))
    {
        func.position1 = i;
        for (auto j : itensor::range1(L))
        {
            func.position2 = j;
            sz_i_sz_j(i - 1, j - 1) = ::compute_two_point(psi, sites, func);
        }
    }
    return {{"sz_i_sz_j"s, sz_i_sz_j}};
}