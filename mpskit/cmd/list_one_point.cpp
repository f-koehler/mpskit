#include "list_one_point.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "../json.hpp"
#include "../models/model_1d.hpp"
#include "../models/registry.hpp"

int cmdListOnePoint(const std::string &input)
{
    auto config = loadJSON(input);
    auto model = createModel1D(config["model"]);
    for (const auto &[name, _] : model->getOnePointFunctions())
    {
        std::cout << name << '\n';
    }
    return 0;
}