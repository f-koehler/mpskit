#ifndef DMRG_TYPES
#define DMRG_TYPES

#include <complex>
#include <xtensor/xarray.hpp>

using Real = double;
using Complex = std::complex<Real>;

using RealArray = xt::xarray<Real>;
using ComplexArray = xt::xarray<Complex>;

#endif /* DMRG_TYPES */
