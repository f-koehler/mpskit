#ifndef MPSKIT_UTIL
#define MPSKIT_UTIL

#include "json.hpp"
#include "types.hpp"

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/sweeps.h>
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_signed<T>::value && std::is_integral<T>::value, std::size_t>::type toSizeT(const T &t)
{
    if (t < 0)
    {
        throw std::domain_error("cannot cast negative number to a std::size_t");
    }
    return static_cast<std::size_t>(t);
}

itensor::Sweeps getSweepsFromJSON(const json &j);

#endif /* MPSKIT_UTIL */
