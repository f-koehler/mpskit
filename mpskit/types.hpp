#ifndef MPSKIT_TYPES
#define MPSKIT_TYPES

#include <complex>
#include <vector>
#include <xtensor/xarray.hpp>

using Real = double;
using Complex = std::complex<Real>;

using RealArray = xt::xarray<Real>;
using ComplexArray = xt::xarray<Complex>;

using RealSeries = std::vector<Complex>;
using ComplexSeries = std::vector<Complex>;

constexpr Real nanoseconds_to_seconds = 1e9;

#endif /* MPSKIT_TYPES */
