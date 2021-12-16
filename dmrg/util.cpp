#include "util.hpp"
#include <fmt/core.h>

inline Real require_real_valued(Complex x, double threshold = 1e-10)
{
    if (x.imag() > threshold)
        throw std::domain_error(fmt::format("Encountered large imaginary part ({} > {})", x.imag(), threshold));
    return x.real();
}

Complex compute_expectation_value(const itensor::MPS &psi, const itensor::MPO &op)
{
    return itensor::innerC(psi, op, psi);
}

Complex compute_expectation_value_2(const itensor::MPS &psi, const itensor::MPO &op)
{
    return itensor::innerC(op, psi, op, psi);
}

Complex compute_variance(const itensor::MPS &psi, const itensor::MPO &op)
{
    Complex exp = require_real_valued(itensor::innerC(psi, op, psi));
    return itensor::innerC(op, psi, op, psi) - (exp * exp);
}

itensor::Sweeps get_sweeps_from_json(const json &j)
{
    int num_sweeps = static_cast<int>(j.size());
    auto sweeps = itensor::Sweeps(num_sweeps);
    for (int i = 0; i < num_sweeps; ++i)
    {
        const auto &entry = j[static_cast<std::size_t>(i)];
        if (entry.contains("maxdim"))
        {
            sweeps.setmaxdim(i, entry["maxdim"].get<int>());
        }
        if (entry.contains("mindim"))
        {
            sweeps.setmindim(i, entry["mindim"].get<int>());
        }
        if (entry.contains("cutoff"))
        {
            sweeps.setcutoff(i, entry["cutoff"].get<Real>());
        }
        if (entry.contains("niter"))
        {
            sweeps.setniter(i, entry["niter"].get<int>());
        }
        if (entry.contains("noise"))
        {
            sweeps.setnoise(i, entry["noise"].get<Real>());
        }
    }
    return sweeps;
}