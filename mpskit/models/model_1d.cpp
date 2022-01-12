#include "model_1d.hpp"

#include <itensor/mps/autompo.h>

#include "../terms.hpp"

Model1D::Model1D(const itensor::SiteSet &sites, int L, bool periodic) : Model(sites), m_L(L), m_periodic(periodic)
{
}

auto Model1D::getHamiltonian() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (const auto &term : m_one_body_terms)
    {
        ampo += term;
    }
    for (const auto &term : m_two_body_terms)
    {
        ampo += term;
    }
    return itensor::toMPO(ampo);
}

std::vector<OnePointFunction> Model1D::generateOnePointFunctions(const std::string &op, const Real &prefactor,
                                                                 bool full) const
{
    std::vector<OnePointFunction> functions;
    if (full || !m_periodic)
    {
        functions.reserve(static_cast<std::size_t>(m_L));
        for (auto i : itensor::range1(m_L))
        {
            functions.emplace_back(m_sites, i, op, prefactor);
        }
    }
    else
    {
        functions.emplace_back(m_sites, 1, op, prefactor);
    }
    return functions;
}

std::vector<TwoPointFunction> Model1D::generateTwoPointFunctions(const std::string &op1, const std::string &op2,
                                                                 const Real &prefactor, bool full) const
{
    std::vector<TwoPointFunction> functions;
    if (!full)
    {
        if (m_periodic)
        {
            functions.reserve(static_cast<std::size_t>(m_L));
            for (auto i : itensor::range1(m_L))
            {
                functions.emplace_back(m_sites, 1, i, op1, op2, prefactor);
            }
            return functions;
        }
        if (op1 == op2)
        {
            functions.reserve(static_cast<std::size_t>((m_L * (m_L + 1)) / 2));
            for (auto i : itensor::range1(m_L))
            {
                for (auto j : itensor::range1(i))
                {
                    functions.emplace_back(m_sites, i, j, op1, op1, prefactor);
                }
            }
            return functions;
        }
    }

    functions.reserve(static_cast<std::size_t>(m_L * m_L));
    for (auto i : itensor::range1(m_L))
    {
        for (auto j : itensor::range1(m_L))
        {
            functions.emplace_back(m_sites, i, j, op1, op1, prefactor);
        }
    }
    return functions;
}

auto Model1D::getOneBodyTerms() const -> const std::vector<OneSiteTerm> &
{
    return m_one_body_terms;
}

auto Model1D::getTwoBodyTerms() const -> const std::vector<TwoSiteTerm> &
{
    return m_two_body_terms;
}

auto Model1D::isPeriodic() const -> bool
{
    return m_periodic;
}

auto Model1D::getL() const -> int
{
    return m_L;
}