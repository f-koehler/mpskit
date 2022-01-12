#include "registry.hpp"
#include "../json.hpp"
#include "bose_hubbard_1d.hpp"
#include "heisenberg_1d.hpp"
#include "model_1d.hpp"
#include "transverse_ising_1d.hpp"

#include <fmt/core.h>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

auto createModel1D(const json &js) -> std::shared_ptr<Model1D>
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