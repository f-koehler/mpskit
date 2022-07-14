#include "list_observables.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "../json.hpp"
#include "../models/model_1d.hpp"
#include "../models/registry.hpp"

int cmdListObservables(const std::string &input)
{
    auto config = loadJSON(input);
    auto model = createModel(config["model"]);
    for (const auto &[name, _] : model->getObservables())
    {
        std::cout << name << '\n';
    }
    return 0;
}