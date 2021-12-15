#include "bose_hubbard_1d.hpp"
#include <fmt/core.h>

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
