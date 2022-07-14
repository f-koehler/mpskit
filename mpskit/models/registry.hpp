#ifndef MPSKIT_MODELS_REGISTRY
#define MPSKIT_MODELS_REGISTRY

#include <memory>

#include "../json.hpp"

class Model;

auto createModel(const json &js) -> std::shared_ptr<Model>;

#endif /* MPSKIT_MODELS_REGISTRY */
