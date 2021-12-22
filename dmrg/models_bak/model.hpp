#ifndef DMRG_MODEL
#define DMRG_MODEL

#include <functional>
#include <itensor/mps/autompo.h>
#include <vector>
#include <xtensor/xarray.hpp>

#include "point_functions.hpp"

// struct OneBodyTerm;
// struct TwoBodyTerm;
struct Observable;

class Model
{
  protected:
    int L;
    bool periodic;

  public:
    virtual ~Model() = default;

    virtual itensor::MPO get_hamiltonian() const = 0;
    virtual itensor::MPS get_initial_state() const = 0;

    virtual std::vector<Observable> get_observables() const = 0;
    virtual std::map<std::string, ComplexArray> compute_one_point(itensor::MPS &psi) const = 0;
    virtual std::map<std::string, ComplexArray> compute_two_point(itensor::MPS &psi) const = 0;
};

// struct OneBodyTerm
// {
//     Real prefactor;
//     int index;
//     std::string op;
// };

// struct TwoBodyTerm
// {
//     Real prefactor;
//     int index1;
//     int index2;
//     std::string op1;
//     std::string op2;
// };

struct Observable
{
    std::string name;
    itensor::MPO mpo;
    bool compute_variance = true;
};

// namespace itensor
// {

// AutoMPO operator+=(AutoMPO &lhs, const OneBodyTerm &rhs)
// {
//     lhs += rhs.prefactor, rhs.op, rhs.index;
//     return lhs;
// }

// AutoMPO operator+=(AutoMPO &lhs, const TwoBodyTerm &rhs)
// {
//     lhs += rhs.prefactor, rhs.op1, rhs.index1, rhs.op2, rhs.index2;
//     return lhs;
// }

// } // namespace itensor

#endif /* DMRG_MODEL */
