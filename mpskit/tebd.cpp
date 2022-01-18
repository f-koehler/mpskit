#include "tebd.hpp"
#include "util.hpp"
#include <fmt/core.h>

#include <itensor/util/print_macro.h>

TEBD::TEBD(std::shared_ptr<Model1D> model, Real dt, bool real_time)
    : m_dt(dt), m_sites(model->getSites()), m_L(m_sites.length()),
      m_gate_type(real_time ? itensor::BondGate::tReal : itensor::BondGate::tImag), m_order(0), m_terms(toSizeT(m_L)),
      m_terms_created(toSizeT(m_L), false)
{
    // check for periodic boundary conditions
    if (model->isPeriodic())
    {
        throw std::runtime_error("TEBD is not implemented for periodic boundary conditions");
    }

    // check locality
    for (const auto &term : model->getTwoBodyTerms())
    {
        if (term.getIndex1() == term.getIndex2())
        {
            continue;
        }
        if (term.getIndex1() + 1 == term.getIndex2())
        {
            continue;
        }
        throw std::runtime_error(
            fmt::format("model is not local, found term between sites {} {}", term.getIndex1(), term.getIndex2()));
    }

    // build terms per site
    for (const auto &term : model->getTwoBodyTerms())
    {
        const auto index = toSizeT(term.getIndex1());
        if (!m_terms_created[index])
        {

            m_terms[index] = term.toItensor(m_sites);
            m_terms_created[index] = true;
        }
        else
        {
            m_terms[index] += term.toItensor(m_sites);
        }
    }

    for (const auto &term : m_terms)
    {
        Print(term);
    }
    // for (const auto &term : model->getOneBodyTerms())
    // {
    //     const auto index = toSizeT(term.getIndex());
    //     if (index + 1 == toSizeT(m_L))
    //     {
    //         m_terms[index] += term.toItensor(m_sites) * itensor::op(m_sites, "Id", term.getIndex());
    //     }
    //     else
    //     {
    //         m_terms[index] += term.toItensor(m_sites) * itensor::op(m_sites, "Id", term.getIndex() + 2);
    //     }
    // }
}

int TEBD::getOrder() const
{
    return m_order;
}

void TEBD::addGatesEvenForward(const Real &dt)
{
    (void)dt;
    // auto last_index = m_L - 1;
    // for (int i = 1; i < last_index; i += 2)
    // {
    //     if (!m_terms_created[toSizeT(i)])
    //     {
    //         continue;
    //     }
    //     m_gates.emplace_back(m_sites, i + 1, i + 2, m_gate_type, dt, m_terms[toSizeT(i)]);
    // }
    // if (last_index % 2 == 1)
    // {
    //     m_gates.emplace_back(m_sites, last_index, last_index - 1, m_gate_type, dt, m_terms[toSizeT(last_index)]);
    // }
}
void TEBD::addGatesOddForward(const Real &dt)
{
    (void)dt;
    // auto last_index = m_L - 1;
    // for (int i = 0; i < last_index; i += 2)
    // {
    //     m_gates.emplace_back(m_sites, i + 1, i + 2, m_gate_type, dt, m_terms[toSizeT(i)]);
    // }
    // if (last_index % 2 == 0)
    // {
    //     m_gates.emplace_back(m_sites, 2, last_index + 1);
    //     m_gates.emplace_back(m_sites, 2, 1, m_terms[toSizeT(last_index)]);
    //     m_gates.emplace_back(m_sites, 2, last_index + 1);
    // }
}
void TEBD::addGatesEvenBackward(const Real &dt)
{
    (void)dt;
    // auto last_index = m_L - 1;
    // int start;
    // if (last_index % 2 == 1)
    // {
    //     m_gates.emplace_back(m_sites, 1, last_index + 1);
    //     m_gates.emplace_back(m_sites, 1, 2, m_terms[toSizeT(last_index)]);
    //     m_gates.emplace_back(m_sites, 1, last_index + 1);
    //     start = last_index - 2;
    // }
    // else
    // {
    //     start = last_index - 1;
    // }
    // for (int i = start; i >= 1; i -= 2)
    // {
    //     m_gates.emplace_back(m_sites, i + 1, i + 2, m_gate_type, dt, m_terms[toSizeT(i)]);
    // }
}
void TEBD::addGatesOddBackward(const Real &dt)
{
    (void)dt;
    // auto last_index = m_L - 1;
    // int start;
    // if (last_index % 2 == 0)
    // {
    //     m_gates.emplace_back(m_sites, 2, last_index + 1);
    //     m_gates.emplace_back(m_sites, 2, 1, m_terms[toSizeT(last_index)]);
    //     m_gates.emplace_back(m_sites, 2, last_index + 1);
    //     start = last_index - 2;
    // }
    // else
    // {
    //     start = last_index - 1;
    // }
    // for (int i = start; i >= 0; i -= 2)
    // {
    //     m_gates.emplace_back(m_sites, i + 1, i + 2, m_gate_type, dt, m_terms[toSizeT(i)]);
    // }
}

void TEBD::run(itensor::MPS &psi, const Real &tfinal)
{
    // FIXME: control cutoff
    itensor::gateTEvol(m_gates, tfinal, m_dt, psi);
}

TEBD1::TEBD1(std::shared_ptr<Model1D> model, Real dt, bool real_time) : TEBD(model, dt, real_time)
{
    m_order = 1;
    m_gates.reserve(toSizeT(m_L) + 2ul);

    addGatesEvenForward(dt);
    addGatesOddForward(dt);

    for (const auto &gate : m_gates)
    {
        Print(gate);
    }
}

TEBD2::TEBD2(std::shared_ptr<Model1D> model, Real dt, bool real_time) : TEBD(model, dt, real_time)
{
    m_order = 2;
    m_gates.reserve(toSizeT(m_L + (m_L / 2)) + 4ul);

    const auto dt2 = m_dt / 2.0;

    addGatesEvenForward(dt2);
    addGatesOddForward(dt);
    addGatesEvenBackward(dt2);
}

// TEBD3::TEBD3(std::shared_ptr<Model1D> model, Real dt, bool real_time) : TEBD(model, dt, real_time)
// {
//     m_order = 3;

//     const auto dt1 = dt / (2.0 - std::pow(2.0, 1.0 / 3.0));
//     const auto dt2 = dt - 2 * dt1;
// }

// TEBD4::TEBD4(std::shared_ptr<Model1D> model, Real dt, bool real_time) : TEBD(model, dt, real_time)
// {
//     m_order = 2;

//     const auto dt1 = 1.0 / (4.0 - std::pow(1.0 / 3.0));
//     const auto dt2 = 1.0 / (4.0 - std::pow(1.0 / 3.0));
// }