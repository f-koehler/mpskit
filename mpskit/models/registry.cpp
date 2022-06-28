#include "registry.hpp"
#include "../json.hpp"

#include "bosons/1d/bose_hubbard_1d.hpp"
#include "spin_half/1d/heisenberg_1d.hpp"
#include "spin_half/1d/ising_lr_1d.hpp"
#include "spin_half/1d/transverse_ising_1d.hpp"
#include "spin_half/1d/vitagliano_2010.hpp"
#include "spin_half/1d/xy_spin_glass_1d.hpp"
#include "spin_half/2d/ising_lr_square.hpp"
#include "spin_half/2d/transverse_ising_square.hpp"

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
        model = new IsingLRSquare(js);
    }
    else if (name == "Vitagliano2010")
    {
        model = new Vitagliano2010::XXModel(js);
    }
    else
    {
        throw std::runtime_error(fmt::format("Unknown model: {}", name));
    }
    return std::shared_ptr<Model>(model);
}