#include "observable.hpp"

#include <complex>
#include <utility>

Observable::Observable(itensor::MPO op) : op(std::move(op))
{
}

Complex &Observable::operator()(const itensor::MPS &mps)
{
    value = itensor::innerC(mps, op, mps);
    squared = itensor::innerC(op, mps, op, mps);
    variance = squared - (value * value);
    return value;
}