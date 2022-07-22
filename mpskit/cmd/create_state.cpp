#include "create_state.hpp"

#include <itensor/util/readwrite.h>

#include "../json.hpp"
#include "../models/model_1d.hpp"
#include "../models/registry.hpp"

int cmdCreateState(const std::string &name, const std::string &input_path, const std::string &output_path)
{
    auto input = loadJSON(input_path);
    auto model = createModel(input["model"]);

    auto psi0 = model->getInitialState(name);

    itensor::writeToFile(output_path, psi0);

    return 0;
}