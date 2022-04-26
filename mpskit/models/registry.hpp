#ifndef MPSKIT_MODELS_REGISTRY
#define MPSKIT_MODELS_REGISTRY

#include <memory>

#include "../json.hpp"
#include "bose_hubbard_1d.hpp"
#include "heisenberg_1d.hpp"
#include "transverse_ising_1d.hpp"

class Model;

auto createModel1D(const json &js) -> std::shared_ptr<Model>;

#endif /* MPSKIT_MODELS_REGISTRY */
