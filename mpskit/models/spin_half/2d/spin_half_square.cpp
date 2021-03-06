#include "spin_half_square.hpp"

#include <fmt/core.h>
#include <itensor/mps/autompo.h>
#include <itensor/mps/lattice/square.h>
#include <itensor/mps/sites/spinhalf.h>
#include <itensor/util/iterate.h>

std::vector<OnePointFunction> SpinHalfSquare::generateOnePointFunctions(const std::string &op, const Real &prefactor,
                                                                        bool full) const
{
    std::vector<OnePointFunction> functions;
    if (full)
    {
        auto N = m_Lx * m_Ly;
        functions.reserve(static_cast<std::size_t>(N));
        for (int i = 0; i < N; ++i)
        {
            functions.emplace_back(m_sites, i, op, prefactor);
        }
    }
    else
    {
        functions.emplace_back(m_sites, 0, op, prefactor);
    }
    return functions;
}

std::vector<TwoPointFunction> SpinHalfSquare::generateTwoPointFunctions(const std::string &op1, const std::string &op2,
                                                                        const Real &prefactor, bool full) const
{
    std::vector<TwoPointFunction> functions;
    auto N = m_Lx * m_Ly;
    if (full)
    {
        functions.reserve(static_cast<std::size_t>(N * N));
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                functions.emplace_back(m_sites, i, j, op1, op2, prefactor);
            }
        }
    }
    else
    {
        functions.reserve(static_cast<std::size_t>(N));
        for (int i = 0; i < N; ++i)
        {
            functions.emplace_back(m_sites, 0, i, op1, op2, prefactor);
        }
    }
    return functions;
}

SpinHalfSquare::SpinHalfSquare(int Lx, int Ly, bool conserve_Sz, bool conserve_parity)
    : Model2D(itensor::SpinHalf(Lx * Ly, {"ConserveSz=", conserve_Sz, "ConserveParity=", conserve_parity}),
              itensor::squareLattice(Lx, Ly, {"YPeriodic=", false})),
      m_conserve_Sz(conserve_Sz), m_conserve_parity(conserve_parity), m_Lx(Lx), m_Ly(Ly)
{
    m_name = "SpinHalfSquare";
}

itensor::MPS SpinHalfSquare::getInitialState(const std::string &initial_state) const
{
    if ((initial_state == "random") || (initial_state == "default"))
    {
        return itensor::randomMPS(m_sites);
    }

    const auto N = m_Lx * m_Ly;
    auto init = itensor::InitState(m_sites);
    if (initial_state == "up")
    {
        for (auto i : itensor::range1(N))
        {
            init.set(i, "Up");
        }
        return init;
    }

    if (initial_state == "down")
    {
        for (auto i : itensor::range1(N))
        {
            init.set(i, "Dn");
        }
        return init;
    }

    if (initial_state == "neel_up")
    {
        for (auto i : itensor::range1(N))
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
        for (auto i : itensor::range1(N))
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
            for (auto i : itensor::range1(N))
            {
                init.set(i, "Up");
            }
        }
        else
        {
            for (auto i : itensor::range1(N))
            {
                init.set(i, "Dn");
            }
        }
        auto psi = itensor::MPS(init);

        for (auto i : itensor::range1(N))
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

itensor::MPO SpinHalfSquare::getTotalSxOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}
itensor::MPO SpinHalfSquare::getTotalSyOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}
itensor::MPO SpinHalfSquare::getTotalSzOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}
std::map<std::string, Observable> SpinHalfSquare::getObservables() const
{
    auto result = Model2D::getObservables();
    result.insert({"Sx", Observable(getTotalSxOperator())});
    result.insert({"Sy", Observable(getTotalSyOperator())});
    result.insert({"Sz", Observable(getTotalSzOperator())});
    return result;
}

std::map<std::string, std::vector<OnePointFunction>> SpinHalfSquare::getOnePointFunctions() const
{

    std::map<std::string, std::vector<OnePointFunction>> result;
    result.insert({"sx", generateOnePointFunctions("Sx", 2.0)});
    result.insert({"sy", generateOnePointFunctions("Sy", 2.0)});
    result.insert({"sz", generateOnePointFunctions("Sz", 2.0)});
    return result;
}
std::map<std::string, std::vector<TwoPointFunction>> SpinHalfSquare::getTwoPointFunctions() const
{
    std::map<std::string, std::vector<TwoPointFunction>> result;
    result.insert({"sx_sx", generateTwoPointFunctions("Sx", "Sx", 4.0, true)});
    result.insert({"sy_sy", generateTwoPointFunctions("Sy", "Sy", 4.0, true)});
    result.insert({"sz_sz", generateTwoPointFunctions("Sz", "Sz", 4.0, true)});
    return result;
}

bool SpinHalfSquare::doesConserveSz() const
{
    return m_conserve_Sz;
}
bool SpinHalfSquare::doesConserveParity() const
{
    return m_conserve_parity;
}

int SpinHalfSquare::getLx() const
{
    return m_Lx;
}
int SpinHalfSquare::getLy() const
{
    return m_Ly;
}

void SpinHalfSquare::print(std::ostream &stream) const
{
    Model2D::print(stream);
    stream << "\n\tconserve Sz:        " << m_conserve_Sz;
    stream << "\n\tconserve Parity:    " << m_conserve_parity;
    stream << "\n\tLx:                 " << m_Lx;
    stream << "\n\tLy:                 " << m_Ly;
}