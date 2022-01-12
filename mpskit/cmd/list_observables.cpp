#include "list_observables.hpp"
#include "../json.hpp"
#include "../models/registry.hpp"

#include <iostream>
#include <string>

int cmdListObservables(const std::string &input)
{
    auto config = loadJSON(input);
    auto model = createModel1D(config["model"]);
    for (const auto &[name, _] : model->getObservables())
    {
        std::cout << name << '\n';
    }
    return 0;
}