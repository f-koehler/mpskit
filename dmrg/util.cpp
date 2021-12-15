#include "util.hpp"

double compute_expectation_value(const itensor::MPS &psi, const itensor::MPO &op)
{
    return itensor::inner(psi, op, psi);
}

double compute_expectation_value_2(const itensor::MPS &psi, const itensor::MPO &op)
{
    return itensor::inner(op, psi, op, psi);
}

double compute_variance(const itensor::MPS &psi, const itensor::MPO &op)
{
    double exp = itensor::inner(psi, op, psi);
    return itensor::inner(op, psi, op, psi) - (exp * exp);
}

double compute_two_point(const itensor::MPS &psi, const itensor::MPO &op1, const itensor::MPO &op2)
{
    return itensor::inner(op1, psi, op2, psi) - (itensor::inner(psi, op1, psi) * itensor::inner(psi, op2, psi));
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
            sweeps.setcutoff(i, entry["cutoff"].get<double>());
        }
        if (entry.contains("niter"))
        {
            sweeps.setniter(i, entry["niter"].get<int>());
        }
        if (entry.contains("noise"))
        {
            sweeps.setnoise(i, entry["noise"].get<double>());
        }
    }
    return sweeps;
}