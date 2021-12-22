#ifndef DMRG_MODELS_REGISTRY
#define DMRG_MODELS_REGISTRY

#include "../json.hpp"
#include "bose_hubbard_1d.hpp"
#include "heisenberg_1d.hpp"
#include "transverse_ising_1d.hpp"
#include <memory>

auto create_model_1d(const json &js) -> std::shared_ptr<Model1D>;

#endif /* DMRG_MODELS_REGISTRY */
