#include "list_one_point.hpp"
#include "../json.hpp"
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