#include "bose_hubbard_1d.hpp"

#include <complex>
#include <itensor/mps/autompo.h>
#include <itensor/util/iterate.h>
#include <nlohmann/json.hpp>
#include <xtensor/xbroadcast.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xcontainer.hpp>

#include "point_functions.hpp"

using namespace std::string_literals;

BoseHubbard1D::BoseHubbard1D(int L, int N, Real J, Real U, bool periodic)
    : L(L), N(N), J(J), U(U), periodic(periodic), sites(L, {"MaxOcc=", N})
{
}

BoseHubbard1D::BoseHubbard1D(const json &j)
    : L(j["L"].get<int>()), N(j["N"].get<int>()), J(j["J"].get<Real>()), U(j["U"].get<Real>()),
      periodic(j["periodic"].get<bool>()), sites(L, {"MaxOcc=", N})
{
}

auto BoseHubbard1D::get_initial_state() const -> itensor::MPS
{
    auto state = itensor::InitState(sites);
    state.set(1, std::to_string(N));
    return itensor::randomMPS(state);
}

auto BoseHubbard1D::get_hamiltonian() const -> itensor::MPO
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

auto BoseHubbard1D::get_particle_number_operator() const -> itensor::MPO
{
    auto ampo = itensor::AutoMPO(sites);
    for (auto i : itensor::range1(L))
    {
        ampo += 1.0, "N", i;
    }
    return itensor::toMPO(ampo);
}

auto BoseHubbard1D::get_observables() const -> std::vector<Observable>
{
    std::vector<Observable> observables = {Observable{"H", get_hamiltonian()},
                                           Observable{"N", get_particle_number_operator()}};

    return observables;
}

auto BoseHubbard1D::compute_one_point(itensor::MPS &psi) const -> std::map<std::string, ComplexArray>
{
    ComplexArray n_i = xt::zeros<Complex>({L});
    auto func = OnePoint{1.0, 1, "N"};
    for (auto i : itensor::range1(L))
    {
        func.position = i;
        n_i(i - 1) = ::compute_one_point(psi, sites, func);
    }
    return {{"n_i"s, n_i}};
}

auto BoseHubbard1D::compute_two_point(itensor::MPS &psi) const -> std::map<std::string, ComplexArray>
{
    ComplexArray n_i_n_j = xt::zeros<Complex>({L, L});
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