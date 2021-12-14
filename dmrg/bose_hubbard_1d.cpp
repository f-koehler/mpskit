#include "bose_hubbard_1d.hpp"

BoseHubbard1D::BoseHubbard1D(int L, int N, double J, double U, bool periodic) : L(L), N(N), J(J), U(U), periodic(periodic), sites(L, {"MaxOcc=", N})
{
}

BoseHubbard1D BoseHubbard1D::from_json(const json &j)
{
    return BoseHubbard1D(j["L"].get<int>(), j["N"].get<int>(), j["J"].get<double>(), j["U"].get<double>(), j["periodic"].get<bool>());
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
    for (int i = 1; i < L; ++i)
    {
        ampo += -J, "Adag", i, "A", i + 1;
        ampo += -J, "Adag", i + 1, "A", i;
    }
    if (periodic)
    {
        ampo += -J, "Adag", 1, "A", L;
        ampo += -J, "Adag", L, "A", 1;
    }
    for (int i = 1; i <= L; ++i)
    {
        ampo += (U / 2.0), "N", i, "N", i;
        ampo += -(U / 2.0), "N", i;
    }

    return itensor::toMPO(ampo);
}

itensor::MPO BoseHubbard1D::get_particle_number_operator() const
{
    auto ampo = itensor::AutoMPO(sites);
    for (int i = 1; i <= L; ++i)
    {
        ampo += 1.0, "N", i;
    }
    return itensor::toMPO(ampo);
}

itensor::MPO BoseHubbard1D::get_site_occupation_operator(int j) const
{
    auto ampo = itensor::AutoMPO(sites);
    ampo += 1.0, "N", j;
    return itensor::toMPO(ampo);
}

std::vector<Observable> BoseHubbard1D::get_observables() const
{
    std::vector<Observable> observables = {
        Observable{"H", get_hamiltonian()},
        Observable{"N", get_particle_number_operator()}};

    for (int i = 1; i < L; ++i)
    {
        observables.emplace_back(Observable{fmt::format("n_{}", i), get_site_occupation_operator(i)});
    }

    return observables;
}

std::vector<TwoPointCorrelation> BoseHubbard1D::get_two_point_correlations() const
{
    std::vector<TwoPointCorrelation> correlations;
    for (int i = 1; i < L; ++i)
    {
        for (int j = 1; j < L; ++j)
        {
            correlations.emplace_back(TwoPointCorrelation{fmt::format("n_{}_n_{}", i, j), get_site_occupation_operator(i), get_site_occupation_operator(j)});
        }
    }
    return correlations;
}