#include "util.hpp"

#include <cstddef>
#include <map>
#include <nlohmann/json.hpp>

#include "types.hpp"

auto getSweepsFromJSON(const json &j) -> itensor::Sweeps
{
    int num_sweeps = static_cast<int>(j.size());
    auto sweeps = itensor::Sweeps(num_sweeps);
    for (int i = 0; i < num_sweeps; ++i)
    {
        const auto &entry = j[static_cast<std::size_t>(i)];
        if (entry.contains("maxdim"))
        {
            sweeps.setmaxdim(i + 1, entry["maxdim"].get<int>());
        }
        if (entry.contains("mindim"))
        {
            sweeps.setmindim(i + 1, entry["mindim"].get<int>());
        }
        if (entry.contains("cutoff"))
        {
            sweeps.setcutoff(i + 1, entry["cutoff"].get<Real>());
        }
        if (entry.contains("niter"))
        {
            sweeps.setniter(i + 1, entry["niter"].get<int>());
        }
        if (entry.contains("noise"))
        {
            sweeps.setnoise(i + 1, entry["noise"].get<Real>());
        }
    }
    return sweeps;
}