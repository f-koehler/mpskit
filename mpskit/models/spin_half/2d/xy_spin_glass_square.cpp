#include "xy_spin_glass_square.hpp"

XYSpinGlassSquare::XYSpinGlassSquare(const json& js)
: SpinHalfSquare(js["Lx"].get<int>(), js["Ly"].get<int>(<))
{}