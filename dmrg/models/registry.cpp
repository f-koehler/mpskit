#include "registry.hpp"
#include <fmt/core.h>
#include <stdexcept>

auto create_model_1d(const json &js) -> std::shared_ptr<Model1D>
{
    const auto name = js["name"].get<std::string>();
    Model1D *model = nullptr;
    if (name == "BoseHubbard1D")
    {
        model = new BoseHubbard1D(js);
    }
    else if (name == "TransverseIsing1D")
    {
        model = new TransverseIsing1D(js);
    }
    else if (name == "Heisenberg1D")
    {
        model = new Heisenberg1D(js);
    }
    else
    {
        throw std::runtime_error(fmt::format("Unknown model: {}", name));
    }
    return std::shared_ptr<Model1D>(model);
}