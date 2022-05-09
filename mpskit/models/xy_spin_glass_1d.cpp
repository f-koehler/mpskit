#include "xy_spin_glass_1d.hpp"
#include <highfive/H5Easy.hpp>
#include <random>
#include <xtensor/xarray.hpp>

XYSpinGlass1D::XYSpinGlass1D(const json &js)
    : SpinHalf1D(js["L"].get<int>(), js["periodic"].get<bool>(), false, false), m_alpha(js["alpha"].get<Real>())
{
    xt::xarray<Real> J_matrix = xt::zeros<Real>({2, 2});
    const auto J_matrix_file = jsonGetDefault<std::string>(js, "J_matrix_file", "");
    if (!J_matrix_file.empty())
    {
        H5Easy::File file(J_matrix_file, H5Easy::File::ReadOnly);
        J_matrix = H5Easy::load<xt::xarray<Real>>(file, "/values");
    }
    else
    {
        std::mt19937_64 prng;
        prng.seed(jsonGetDefault<std::uint_fast64_t>(js, "seed", 0));

        std::uniform_real_distribution<Real> dist(jsonGetDefault<Real>(js, "Jmin", -1.0),
                                                  jsonGetDefault<Real>(js, "Jmax", 0.0));
        for (int i = 0; i < m_L; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                J_matrix(i, j) = dist(prng);
            }
        }
    }

    for (int i = 0; i < m_L; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            const auto J = J_matrix(i, j) / std::pow(std::abs(static_cast<Real>(i - j)), m_alpha);
            m_two_body_terms.emplace_back(4.0 * J, "S+", i, "S-", j);
            m_two_body_terms.emplace_back(4.0 * J, "S-", i, "S+", j);
        }
    }
}

auto XYSpinGlass1D::getAlpha() const -> const Real &
{
    return m_alpha;
}