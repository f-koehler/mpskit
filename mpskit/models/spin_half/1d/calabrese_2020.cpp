#include "calabrese_2020.hpp"
#include <highfive/H5Easy.hpp>
#include <xtensor/xarray.hpp>

Calabrese2020::Calabrese2020(const json &js)
    : SpinHalf1D(js["L"].get<int>(), false, false, false), m_delta(jsonGetDefault<Real>(js, "delta", 1.0))
{
    m_name = "Calabrese2020";

    const auto J_vector_file = js["J_vector_file"].get<std::string>();
    xt::xarray<Real> J_vector = xt::zeros<Real>({m_L});
    H5Easy::File file(J_vector_file, H5Easy::File::ReadOnly);
    J_vector = H5Easy::load<xt::xarray<Real>>(file, "/values");

    for (int i = 0; i < m_L - 1; ++i)
    {
        const auto J = J_vector(i);
        m_two_body_terms.emplace_back(4.0 * J, "Sx", i, "Sx", i + 1);
        m_two_body_terms.emplace_back(4.0 * J, "Sy", i, "Sy", i + 1);
        m_two_body_terms.emplace_back(4.0 * J * m_delta, "Sz", i, "Sz", i + 1);
    }
}

const Real &Calabrese2020::getDelta() const
{
    return m_delta;
}

void Calabrese2020::print(std::ostream &stream) const
{
    SpinHalf1D::print(stream);
    stream << "\n\tdelta:              " << m_delta;
}