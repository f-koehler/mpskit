#include "transverse_ising_1d.hpp"
#include <fmt/core.h>

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

itensor::MPO TransverseIsing1D::get_sigma_x(int site) const
{
    auto ampo = itensor::AutoMPO(sites);
    ampo += 2.0, "Sx", site;
    return itensor::toMPO(ampo);
}

itensor::MPO TransverseIsing1D::get_sigma_y(int site) const
{
    auto ampo = itensor::AutoMPO(sites);
    ampo += 2.0, "Sy", site;
    return itensor::toMPO(ampo);
}

itensor::MPO TransverseIsing1D::get_sigma_z(int site) const
{
    auto ampo = itensor::AutoMPO(sites);
    ampo += 2.0, "Sz", site;
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
    for (auto i : itensor::range1(L))
    {
        observables.emplace_back(Observable{fmt::format("x_{}", i), get_sigma_x(i)});
        observables.emplace_back(Observable{fmt::format("y_{}", i), get_sigma_y(i)});
        observables.emplace_back(Observable{fmt::format("z_{}", i), get_sigma_z(i)});
    }
    return observables;
}

std::vector<TwoPointCorrelation> TransverseIsing1D::get_two_point_correlations() const
{
    std::vector<TwoPointCorrelation> correlations;
    for (auto i : itensor::range1(L))
    {
        for (auto j : itensor::range1(L))
        {
            correlations.emplace_back(TwoPointCorrelation{fmt::format("x_{}_x_{}", i, j), get_sigma_x(i), get_sigma_x(j)});
            correlations.emplace_back(TwoPointCorrelation{fmt::format("y_{}_y_{}", i, j), get_sigma_y(i), get_sigma_y(j)});
            correlations.emplace_back(TwoPointCorrelation{fmt::format("z_{}_z_{}", i, j), get_sigma_z(i), get_sigma_z(j)});
        }
    }
    return correlations;
}