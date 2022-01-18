#ifndef MPSKIT_TEBD
#define MPSKIT_TEBD

#include "models/model_1d.hpp"
#include "terms.hpp"
#include "types.hpp"

#include <cmath>
#include <itensor/mps/bondgate.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/tevol.h>
#include <memory>
#include <vector>

class TEBD
{
  protected:
    std::vector<itensor::BondGate> m_gates;
    Real m_dt;
    const itensor::SiteSet m_sites;
    const int m_L;
    const itensor::BondGate::Type m_gate_type;
    int m_order;
    std::vector<itensor::ITensor> m_terms;
    std::vector<bool> m_terms_created;

    void addGatesEvenForward(const Real &dt);
    void addGatesOddForward(const Real &dt);
    void addGatesEvenBackward(const Real &dt);
    void addGatesOddBackward(const Real &dt);

  public:
    explicit TEBD(std::shared_ptr<Model1D> model, Real dt, bool real_time = true);

    int getOrder() const;

    void run(itensor::MPS &psi, const Real &tfinal);
};

class TEBD1 : public TEBD
{
  public:
    explicit TEBD1(std::shared_ptr<Model1D> model, Real dt, bool real_time = true);
};

class TEBD2 : public TEBD
{
  public:
    explicit TEBD2(std::shared_ptr<Model1D> model, Real dt, bool real_time = true);
};

// class TEBD3 : public TEBD
// {
//   public:
//     explicit TEBD3(std::shared_ptr<Model1D> model, Real dt, bool real_time = true);
// };

// class TEBD4 : public TEBD
// {
//   public:
//     explicit TEBD4(std::shared_ptr<Model1D> model, Real dt, bool real_time = true);
// };

#endif /* MPSKIT_TEBD */
