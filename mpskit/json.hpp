#ifndef MPSKIT_JSON
#define MPSKIT_JSON

#include <nlohmann/json.hpp>

using json = nlohmann::json;

template <typename T> T json_get_default(const json &j, const std::string &key, const T &default_)
{
    if (j.contains(key))
    {
        return j[key].get<T>();
    }
    return default_;
}

auto load_json(const std::string &path) -> json;

#endif /* MPSKIT_JSON */
