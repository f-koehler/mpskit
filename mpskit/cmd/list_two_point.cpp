#include "list_two_point.hpp"
#include "../json.hpp"
#include "../models/registry.hpp"

#include <iostream>

int cmdListTwoPoint(const std::string &input)
{
    auto config = loadJSON(input);
    auto model = createModel1D(config["model"]);
    for (const auto &[name, _] : model->getTwoPointFunctions())
    {
        std::cout << name << '\n';
    }
    return 0;
}