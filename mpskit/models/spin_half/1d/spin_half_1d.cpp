#include "spin_half_1d.hpp"

#include <fmt/core.h>
#include <itensor/mps/autompo.h>
#include <itensor/mps/sites/spinhalf.h>
#include <itensor/util/iterate.h>

#include "../../../observable.hpp"
#include "../../../point_functions.hpp"

SpinHalf1D::SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity)
    : Model1D(itensor::SpinHalf(L, {"ConserveSz=", conserve_Sz, "ConserveParity=", conserve_parity}), L, periodic),
      m_conserve_Sz(conserve_Sz), m_conserve_parity(conserve_parity)
{
    m_name = "SpinHalf1D";
}

auto SpinHalf1D::getInitialState(const std::string &initial_state) const -> itensor::MPS
{
    if ((initial_state == "random") || (initial_state == "default"))
    {
        return itensor::randomMPS(m_sites);
    }

    auto init = itensor::InitState(m_sites);
    if (initial_state == "up")
    {
        for (auto i : itensor::range1(m_L))
        {
            init.set(i, "Up");
        }
        return init;
    }

    if (initial_state == "down")
    {
        for (auto i : itensor::range1(m_L))
        {
            init.set(i, "Dn");
        }
        return init;
    }

    if (initial_state == "neel_up")
    {
        for (auto i : itensor::range1(m_L))
        {
            if (i % 2 == 0)
            {
                init.set(i, "Dn");
            }
            else
            {
                init.set(i, "Up");
            }
        }
        return init;
    }

    if (initial_state == "neel_down")
    {
        for (auto i : itensor::range1(m_L))
        {
            if (i % 2 == 0)
            {
                init.set(i, "Up");
            }
            else
            {
                init.set(i, "Dn");
            }
        }
        return init;
    }

    if ((initial_state == "plus") || (initial_state == "minus"))
    {
        if (initial_state == "plus")
        {
            for (auto i : itensor::range1(m_L))
            {
                init.set(i, "Up");
            }
        }
        else
        {
            for (auto i : itensor::range1(m_L))
            {
                init.set(i, "Dn");
            }
        }
        auto psi = itensor::MPS(init);

        for (auto i : itensor::range1(m_L))
        {
            auto ind = m_sites(i);
            auto ind_prime = itensor::prime(m_sites(i));
            auto hadamard = itensor::ITensor(ind, ind_prime);
            hadamard.set(ind(1), ind_prime(1), itensor::ISqrt2);
            hadamard.set(ind(1), ind_prime(2), itensor::ISqrt2);
            hadamard.set(ind(2), ind_prime(1), itensor::ISqrt2);
            hadamard.set(ind(2), ind_prime(2), -itensor::ISqrt2);
            psi.set(i, (hadamard * psi(i)).noPrime());
        }
        return psi;
    }

    throw std::invalid_argument(fmt::format("Unknown initial state \"{}\"", initial_state));
}

auto SpinHalf1D::getTotalSxOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getTotalSyOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getTotalSzOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getObservables() const -> std::map<std::string, Observable>
{
    auto result = Model1D::getObservables();
    result.insert({"Sx", Observable(getTotalSxOperator())});
    result.insert({"Sy", Observable(getTotalSyOperator())});
    result.insert({"Sz", Observable(getTotalSzOperator())});
    return result;
}

std::map<std::string, std::vector<OnePointFunction>> SpinHalf1D::getOnePointFunctions() const
{
    std::map<std::string, std::vector<OnePointFunction>> result;
    result.insert({"sx", generateOnePointFunctions("Sx", 2.0)});
    result.insert({"sy", generateOnePointFunctions("Sy", 2.0)});
    result.insert({"sz", generateOnePointFunctions("Sz", 2.0)});
    return result;
}

std::map<std::string, std::vector<TwoPointFunction>> SpinHalf1D::getTwoPointFunctions() const
{
    std::map<std::string, std::vector<TwoPointFunction>> result;
    result.insert({"sx_sx", generateTwoPointFunctions("Sx", "Sx", 4.0, m_L <= 32)});
    result.insert({"sy_sy", generateTwoPointFunctions("Sy", "Sy", 4.0, m_L <= 32)});
    result.insert({"sz_sz", generateTwoPointFunctions("Sz", "Sz", 4.0, m_L <= 32)});
    return result;
}

auto SpinHalf1D::doesConserveSz() const -> bool
{
    return m_conserve_Sz;
}
auto SpinHalf1D::doesConserveParity() const -> bool
{
    return m_conserve_parity;
}

void SpinHalf1D::print(std::ostream &stream) const
{
    Model1D::print(stream);
    stream << "\n\tconserve Sz:        " << m_conserve_Sz;
    stream << "\n\tconserve Parity:    " << m_conserve_parity;
}