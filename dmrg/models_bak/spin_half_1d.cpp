#include "spin_half_1d.hpp"

#include <cmath>

SpinHalf1D::SpinHalf1D(int L, bool periodic, bool use_paulis)
    : L(L), periodic(periodic), use_paulis(use_paulis), sites(itensor::SpinHalf(L, {"ConserveQNs=", false}))
{
}

SpinHalf1D::SpinHalf1D(const json &j)
    : L(j["L"].get<int>()), periodic(json_get_default<bool>(j, "periodic", true)),
      use_paulis(json_get_default<bool>(j, "use_paulis", true)), sites(L, {"ConserveQNs=", false})
{
}

auto SpinHalf1D::get_prefactor(int num_operators) const -> Real
{
    if (use_paulis)
    {
        return std::pow(2.0, num_operators);
    }
    return 1.0;
}