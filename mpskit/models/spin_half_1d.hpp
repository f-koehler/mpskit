#ifndef MPSKIT_MODELS_SPIN_HALF_1D
#define MPSKIT_MODELS_SPIN_HALF_1D

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/sites/spinhalf.h>
#include <map>
#include <string>
#include <vector>

#include "model_1d.hpp"

struct Observable;
struct OnePointFunction;
struct TwoPointFunction;

class SpinHalf1D : public Model1D
{
  protected:
    bool m_conserve_Sz;
    bool m_conserve_parity;

  public:
    explicit SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity);

    itensor::MPS getInitialState(const std::string &initial_state = "default") const override;
    itensor::MPO getTotalSxOperator() const;
    itensor::MPO getTotalSyOperator() const;
    itensor::MPO getTotalSzOperator() const;
    std::map<std::string, Observable> getObservables() const override;
    std::map<std::string, std::vector<OnePointFunction>> getOnePointFunctions() const override;
    std::map<std::string, std::vector<TwoPointFunction>> getTwoPointFunctions() const override;

    bool doesConserveSz() const;
    bool doesConserveParity() const;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_1D */
