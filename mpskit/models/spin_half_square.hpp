#ifndef MPSKIT_MODELS_SPIN_HALF_SQUARE
#define MPSKIT_MODELS_SPIN_HALF_SQUARE

#include "model_2d.hpp"

class SpinHalfSquare : public Model2D
{
  protected:
    bool m_conserve_Sz;
    bool m_conserve_parity;
    int m_Lx;
    int m_Ly;

    std::vector<OnePointFunction> generateOnePointFunctions(const std::string &op, const Real &prefactor,
                                                            bool full = true) const;

  public:
    explicit SpinHalfSquare(int Lx, int Ly, bool conserve_Sz, bool conserve_parity);

    itensor::MPS getInitialState() const override;
    itensor::MPO getTotalSxOperator() const;
    itensor::MPO getTotalSyOperator() const;
    itensor::MPO getTotalSzOperator() const;
    std::map<std::string, Observable> getObservables() const override;
    std::map<std::string, std::vector<OnePointFunction>> getOnePointFunctions() const override;
    std::map<std::string, std::vector<TwoPointFunction>> getTwoPointFunctions() const override;

    bool doesConserveSz() const;
    bool doesConserveParity() const;
};

#endif /* MPSKIT_MODELS_SPIN_HALF_2D */
