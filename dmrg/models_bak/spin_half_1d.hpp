#ifndef DMRG_MODELS_SPIN_HALF_1D
#define DMRG_MODELS_SPIN_HALF_1D

#include "../json.hpp"
#include "../model.hpp"

#include <itensor/mps/sites/spinhalf.h>

class SpinHalf1D : public Model
{
  protected:
    int L;
    bool periodic;
    bool use_paulis;
    itensor::SpinHalf sites;

  public:
    explicit SpinHalf1D(int L, bool periodic = true, bool use_paulis = true);
    explicit SpinHalf1D(const json &j);

    Real get_prefactor(int num_operators) const;
};

#endif /* DMRG_MODELS_SPIN_HALF_1D */
