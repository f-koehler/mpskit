#include "list_two_point.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "../json.hpp"
#include "../models/model_1d.hpp"
#include "../models/registry.hpp"

int cmdListTwoPoint(const std::string &input)
{
    auto config = loadJSON(input);
    auto model = createModel(config["model"]);
    for (const auto &[name, _] : model->getTwoPointFunctions())
    {
        std::cout << name << '\n';
    }
    return 0;
}