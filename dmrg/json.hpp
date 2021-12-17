#ifndef DMRG_JSON
#define DMRG_JSON

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

#endif /* DMRG_JSON */
