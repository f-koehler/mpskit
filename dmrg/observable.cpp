#include "observable.hpp"

#include <complex>

Observable::Observable(const itensor::MPO &op) : op(op)
{
}

void Observable::operator()(const itensor::MPS &mps)
{
    value = itensor::innerC(mps, op, mps);
    squared = itensor::innerC(op, mps, op, mps);
    variance = squared - (value * value);
}