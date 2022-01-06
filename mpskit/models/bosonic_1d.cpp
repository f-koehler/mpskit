#include "bosonic_1d.hpp"

#include <itensor/mps/autompo.h>
#include <itensor/mps/sites/boson.h>
#include <itensor/util/iterate.h>

#include "../observable.hpp"

Bosonic1D::Bosonic1D(int L, bool periodic, bool conserve_N, int max_N)
    : Model1D(itensor::Boson(L, {"MaxOcc=", max_N, "ConserveNb=", conserve_N}), L, periodic), m_conserve_N(conserve_N),
      m_max_N(max_N)
{
}

auto Bosonic1D::get_initial_state() const -> itensor::MPS
{
    auto state = itensor::InitState(m_sites);
    state.set(1, std::to_string(m_max_N));
    return itensor::randomMPS(state);
}

auto Bosonic1D::get_particle_number_operator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 1.0, "N", i;
    }
    return itensor::toMPO(ampo);
}

auto Bosonic1D::get_observables() const -> std::map<std::string, Observable>
{
    auto result = Model1D::get_observables();
    result.insert({"N", Observable(get_particle_number_operator())});
    return result;
}

std::map<std::string, std::vector<OnePointFunction>> Bosonic1D::get_one_point_functions() const
{
    std::map<std::string, std::vector<OnePointFunction>> result;
    result.insert({"n", generate_one_point_functions("N", 1.0)});
    return result;
}

std::map<std::string, std::vector<TwoPointFunction>> Bosonic1D::get_two_point_functions() const
{
    std::map<std::string, std::vector<TwoPointFunction>> result;
    result.insert({"n_n", generate_two_point_functions("N", "N", 1.0, m_L <= 32)});
    result.insert({"bdag_b", generate_two_point_functions("Adag", "A", 1.0, m_L <= 32)});
    return result;
}

auto Bosonic1D::doesConserveN() const -> bool
{
    return m_conserve_N;
}
auto Bosonic1D::getMaxN() const -> int
{
    return m_max_N;
}