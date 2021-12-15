#include "bose_hubbard_1d.hpp"
#include <fmt/core.h>

using namespace std::string_literals;

BoseHubbard1D::BoseHubbard1D(int L, int N, double J, double U, bool periodic) : L(L), N(N), J(J), U(U), periodic(periodic), sites(L, {"MaxOcc=", N})
{
}

BoseHubbard1D::BoseHubbard1D(const json &j) : L(j["L"].get<int>()), N(j["N"].get<int>()), J(j["J"].get<double>()), U(j["U"].get<double>()), periodic(j["periodic"].get<bool>()), sites(L, {"MaxOcc=", N})
{
}

itensor::MPS BoseHubbard1D::get_initial_state() const
{
    auto state = itensor::InitState(sites);
    state.set(1, std::to_string(N));
    return itensor::randomMPS(state);
}

itensor::MPO BoseHubbard1D::get_hamiltonian() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L - 1))
    {
        ampo += -J, "Adag", i, "A", i + 1;
        ampo += -J, "Adag", i + 1, "A", i;
    }
    if (periodic)
    {
        ampo += -J, "Adag", 1, "A", L;
        ampo += -J, "Adag", L, "A", 1;
    }
    for (auto i : itensor::range1(L))
    {
        ampo += (U / 2.0), "N", i, "N", i;
        ampo += -(U / 2.0), "N", i;
    }

    return itensor::toMPO(ampo);
}

itensor::MPO BoseHubbard1D::get_particle_number_operator() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += 1.0, "N", i;
    }
    return itensor::toMPO(ampo);
}

std::vector<Observable> BoseHubbard1D::get_observables() const
{
    std::vector<Observable> observables = {
        Observable{"H", get_hamiltonian()},
        Observable{"N", get_particle_number_operator()}};

    return observables;
}

std::map<std::string, xt::xarray<double>> BoseHubbard1D::compute_one_point(itensor::MPS &psi) const
{
    xt::xarray<double> n_i = xt::zeros<double>({L});
    auto func = OnePoint{1.0, 1, "N"};
    for (auto i : itensor::range1(L))
    {
        func.position = i;
        n_i(i - 1) = ::compute_one_point(psi, sites, func);
    }
    return {{"n_i"s, n_i}};
}

std::map<std::string, xt::xarray<double>> BoseHubbard1D::compute_two_point(itensor::MPS &psi) const
{
    xt::xarray<double> n_i_n_j = xt::zeros<double>({L, L});
    auto func = TwoPoint{1.0, 1, "N", 1, "N"};
    for (auto i : itensor::range1(L))
    {
        func.position1 = i;
        for (auto j : itensor::range1(L))
        {
            func.position2 = j;
            n_i_n_j(i - 1, j - 1) = ::compute_two_point(psi, sites, func);
        }
    }
    return {{"n_i_n_j"s, n_i_n_j}};
}