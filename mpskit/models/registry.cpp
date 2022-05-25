#include "registry.hpp"
#include "../json.hpp"
#include "bose_hubbard_1d.hpp"
#include "heisenberg_1d.hpp"
#include "ising_lr_1d.hpp"
#include "ising_lr_square.hpp"
#include "model_1d.hpp"
#include "transverse_ising_1d.hpp"
#include "transverse_ising_square.hpp"
#include "xy_spin_glass_1d.hpp"

#include <fmt/core.h>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

auto createModel1D(const json &js) -> std::shared_ptr<Model>
{
    const auto name = js["name"].get<std::string>();
    Model *model = nullptr;
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
    else if (name == "XYSpinGlass1D")
    {
        model = new XYSpinGlass1D(js);
    }
    else if (name == "TransverseIsingSquare")
    {
        model = new TransverseIsingSquare(js);
    }
    else if (name == "IsingLR1D")
    {
        model = new IsingLR1D(js);
    }
    else if (name == "IsingLRSquare")
    {
        model = new IsingLR1D(js);
    }
    else
    {
        throw std::runtime_error(fmt::format("Unknown model: {}", name));
    }
    return std::shared_ptr<Model>(model);
}