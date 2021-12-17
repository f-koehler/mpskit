#include "util.hpp"

#include <complex>
#include <cstddef>
#include <fmt/core.h>
#include <itensor/types.h>
#include <nlohmann/json.hpp>
#include <stdexcept>

auto compute_expectation_value(const itensor::MPS &psi, const itensor::MPO &op) -> Complex
{
    return itensor::innerC(psi, op, psi);
}

auto compute_expectation_value_2(const itensor::MPS &psi, const itensor::MPO &op) -> Complex
{
    return itensor::innerC(op, psi, op, psi);
}

auto compute_variance(const itensor::MPS &psi, const itensor::MPO &op) -> Complex
{
    Complex exp = require_real_valued(itensor::innerC(psi, op, psi));
    return itensor::innerC(op, psi, op, psi) - (exp * exp);
}

auto get_sweeps_from_json(const json &j) -> itensor::Sweeps
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