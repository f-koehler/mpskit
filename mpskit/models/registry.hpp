#ifndef MPSKIT_MODELS_REGISTRY
#define MPSKIT_MODELS_REGISTRY

#include "../json.hpp"
#include "bose_hubbard_1d.hpp"
#include "heisenberg_1d.hpp"
#include "transverse_ising_1d.hpp"
#include <memory>

auto create_model_1d(const json &js) -> std::shared_ptr<Model1D>;

#endif /* MPSKIT_MODELS_REGISTRY */
