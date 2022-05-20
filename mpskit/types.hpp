#ifndef MPSKIT_TYPES
#define MPSKIT_TYPES

#include <complex>
#include <itensor/types.h>
#include <vector>
#include <xtensor/xarray.hpp>

using Real = itensor::Real;
using Complex = itensor::Complex;

using RealArray = xt::xarray<Real>;
using IntArray = xt::xarray<int>;
using ComplexArray = xt::xarray<Complex>;

using RealSeries = std::vector<Complex>;
using IntSeries = std::vector<int>;
using ComplexSeries = std::vector<Complex>;

constexpr Real nanoseconds_to_seconds = 1e9;

#endif /* MPSKIT_TYPES */
