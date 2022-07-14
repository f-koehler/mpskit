#include "tebd.hpp"
#include "../json.hpp"
#include "../models/registry.hpp"
#include "../tebd.hpp"

#include <fmt/core.h>

int cmdTEBD(const std::string &input_path, const std::string &psi_path, int order, const Real &tfinal, const Real &dt,
            int substeps)
{
    auto input = loadJSON(input_path);
    auto model = createModel(input["model"]);

    using TEBDPtr = std::shared_ptr<TEBD>;

    std::shared_ptr<TEBD> tebd;
    switch (order)
    {
    case 1:
        tebd = TEBDPtr(new TEBD1(model, dt / static_cast<double>(substeps), true));
        break;
    case 2:
        tebd = TEBDPtr(new TEBD2(model, dt / static_cast<double>(substeps), true));
        break;
    default:
        throw std::invalid_argument(fmt::format("TEBD of order {} is not implemented", order));
    }

    auto psi = itensor::readFromFile<itensor::MPS>(psi_path);

    Real time = 0.0;
    while (time <= tfinal)
    {
        tebd->run(psi, tfinal);
        time += dt;
    }

    return 0;
}