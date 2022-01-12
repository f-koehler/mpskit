#ifndef MPSKIT_MODELS_BOSONIC_1D
#define MPSKIT_MODELS_BOSONIC_1D

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <map>
#include <string>

#include "model_1d.hpp"

struct Observable;

class Bosonic1D : public Model1D
{
  protected:
    bool m_conserve_N;
    int m_max_N;

  public:
    explicit Bosonic1D(int L, bool periodic, bool conserve_N, int max_N);

    itensor::MPS getInitialState() const override;
    itensor::MPO getParticleNumberOperator() const;
    std::map<std::string, Observable> getObservables() const override;
    std::map<std::string, std::vector<OnePointFunction>> getOnePointFunctions() const override;
    std::map<std::string, std::vector<TwoPointFunction>> getTwoPointFunctions() const override;

    bool doesConserveN() const;
    int getMaxN() const;
};

#endif /* MPSKIT_MODELS_BOSONIC_1D */
