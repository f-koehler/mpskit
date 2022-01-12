#ifndef MPSKIT_JSON
#define MPSKIT_JSON

#include <nlohmann/json.hpp>

using json = nlohmann::json;

template <typename T> T jsonGetDefault(const json &j, const std::string &key, const T &default_)
{
    if (j.contains(key))
    {
        return j[key].get<T>();
    }
    return default_;
}

auto loadJSON(const std::string &path) -> json;

#endif /* MPSKIT_JSON */
