#ifndef MPSKIT_POINT_FUNCTIONS
#define MPSKIT_POINT_FUNCTIONS

#include <itensor/itensor.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <string>

#include "types.hpp"

namespace itensor
{
class MPS;
} // namespace itensor

struct OnePointFunction
{
  protected:
    int m_index;
    Real m_prefactor;
    Complex m_value;
    itensor::ITensor m_op;

  public:
    OnePointFunction(const itensor::SiteSet &sites, int index, const std::string &op, Real prefactor = 1.0);
    const Complex &getValue() const;
    int getIndex() const;
    const Complex &operator()(itensor::MPS &mps);
};

struct TwoPointFunction
{
  protected:
    int m_i;
    int m_j;
    Real m_prefactor;
    Complex m_value;
    bool m_ordered, m_same_index;
    itensor::ITensor m_op_i, m_op_j;

  public:
    TwoPointFunction(const itensor::SiteSet &sites, int index1, int index2, const std::string &op1,
                     const std::string &op2, Real prefactor = 1.0);
    const Complex &getValue() const;
    int getIndex1() const;
    int getIndex2() const;
    const Complex &operator()(itensor::MPS &mps);
};

#endif /* MPSKIT_POINT_FUNCTIONS */
