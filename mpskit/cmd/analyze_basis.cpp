#include "analyze_basis.hpp"

#include <cmath>
#include <iostream>

#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include <itensor/mps/mps.h>
#include <itensor/mps/sites/spinhalf.h>
#include <itensor/util/readwrite.h>

#include "../json.hpp"
#include "../models/model.hpp"
#include "../models/registry.hpp"

int cmdAnalyzeBasis(const std::string &input_path, const std::string &psi_path, const std::string &output_path)
{
    auto input = loadJSON(input_path);
    auto model = createModel(input["model"]);
    auto sites = model->getSites();

    // read MPS
    auto psi = itensor::readFromFile<itensor::MPS>(psi_path);

    // auto sites = itensor::SiteSet(itensor::siteInds(psi));
    const auto num_sites = static_cast<std::size_t>(sites.length());
    const auto local_dim = static_cast<std::size_t>(sites(1).size());
    const std::size_t num_states = 1ul << num_sites;

    if (local_dim != 2ul)
    {
        std::cerr << "Currently only implement for local_dim == 2, got local_dim == " << local_dim << '\n';
        return 1;
    }

    if (num_sites > 24ul)
    {
        std::cerr << "Refusing to project onto more than 2^24 states.\n";
        return 1;
    }

    try
    {
        HighFive::File file(output_path, HighFive::File::Overwrite | HighFive::File::Create);
        HighFive::DataSet dset_states = file.createDataSet<int>(
            "states", HighFive::DataSpace({static_cast<std::size_t>(num_states), static_cast<std::size_t>(num_sites)}));
        HighFive::DataSet dset_overlaps =
            file.createDataSet<double>("overlap", HighFive::DataSpace({static_cast<std::size_t>(num_states)}));

        itensor::InitState init_state(sites, "Dn");
        std::vector<std::size_t> configuration(num_sites, 0ul);

        for (std::size_t index = 0; index < num_states; ++index)
        {
            for (std::size_t i = 0; i < num_sites; ++i)
            {
                bool bit = (index >> i) & 1ul;
                configuration[i] = bit ? 1ul : 0ul;
                init_state.set(static_cast<int>(i + 1), bit ? "Up" : "Dn");
            }

            const auto overlap = itensor::innerC(psi, itensor::MPS(init_state));
            const auto overlap_abs = std::abs(overlap);

            dset_states.select({index, 0}, {1, num_sites}).write(configuration);
            dset_overlaps.select(std::vector<std::size_t>{index}).write(overlap_abs * overlap_abs);
        }
    }
    catch (HighFive::Exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}