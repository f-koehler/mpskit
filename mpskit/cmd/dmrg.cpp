#include "dmrg.hpp"

#include <array>
#include <chrono>
#include <fmt/core.h>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>
#include <highfive/bits/H5DataSet_misc.hpp>
#include <highfive/bits/H5File_misc.hpp>
#include <highfive/bits/H5Node_traits_misc.hpp>
#include <highfive/bits/H5Object_misc.hpp>
#include <highfive/bits/H5PropertyList_misc.hpp>
#include <highfive/bits/H5Slice_traits_misc.hpp>
#include <highfive/h5easy_bits/H5Easy_public.hpp>
#include <itensor/mps/dmrg.h>
#include <itensor/util/print_macro.h>
#include <itensor/util/readwrite.h>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <tuple>
#include <utility>

#include "../json.hpp"
#include "../models/model_1d.hpp"
#include "../models/registry.hpp"
#include "../observer.hpp"
#include "../types.hpp"
#include "../util.hpp"

int cmdDMRG(const std::string &input_path, const std::string &output_path, const std::string &psi_path)
{
    auto input = loadJSON(input_path);
    auto model = createModel(input["model"]);

    std::cout << '\n' << *model << "\n\n";

    const auto sweeps = getSweepsFromJSON(input["dmrg"]["sweeps"]);
    const auto initial_state = jsonGetDefault<std::string>(input["model"], "initial_state", "default");

    Print(sweeps);

    const auto hamiltonian = model->getHamiltonian();
    auto psi0 = model->getInitialState(initial_state);
    auto observer = Observer(psi0);

    const auto start_monotonic = std::chrono::steady_clock::now();
    const auto start_hires = std::chrono::high_resolution_clock::now();
    auto [energy, psi] = itensor::dmrg(hamiltonian, psi0, sweeps, observer);
    const auto stop_hires = std::chrono::high_resolution_clock::now();
    const auto stop_monotonic = std::chrono::steady_clock::now();

    auto observables = model->getObservables();
    for (auto &[_, observable] : observables)
    {
        observable(psi);
    }

    auto one_point_functions = model->getOnePointFunctions();
    for (auto &[_, one_point_function] : one_point_functions)
    {
        for (auto &instance : one_point_function)
        {
            instance(psi);
        }
    }

    auto two_point_functions = model->getTwoPointFunctions();
    for (auto &[_, two_point_function] : two_point_functions)
    {
        for (auto &instance : two_point_function)
        {
            instance(psi);
        }
    }

    auto duration_monotonic =
        static_cast<Real>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(stop_monotonic - start_monotonic).count()) /
        nanoseconds_to_seconds;
    auto duration_hires =
        static_cast<Real>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_hires - start_hires).count()) /
        nanoseconds_to_seconds;

    H5Easy::File file(output_path, H5Easy::File::Overwrite);
    H5Easy::dump(file, "/runtimes/monotonic", duration_monotonic);
    H5Easy::dump(file, "/runtimes/hires", duration_hires);

    H5Easy::dump(file, "/mps_info/max_bond_dim", itensor::maxLinkDim(psi));
    H5Easy::dump(file, "/mps_info/average_bond_dim", itensor::averageLinkDim(psi));

    H5Easy::dump(file, "/convergence/energies", observer.getEnergies());
    H5Easy::dump(file, "/convergence/max_bond_dim", observer.getMaxBondDimensions());
    H5Easy::dump(file, "/convergence/average_bond_dim", observer.getAverageBondDimensions());

    for (const auto &[name, observable] : observables)
    {
        H5Easy::dump(file, fmt::format("/observables/{}/real", name), observable.value.real());
        H5Easy::dump(file, fmt::format("/observables/{}/imag", name), observable.value.imag());
        H5Easy::dump(file, fmt::format("/observables/{}/squared_real", name), observable.squared.real());
        H5Easy::dump(file, fmt::format("/observables/{}/squared_imag", name), observable.squared.imag());
        H5Easy::dump(file, fmt::format("/observables/{}/variance_real", name), observable.variance.real());
        H5Easy::dump(file, fmt::format("/observables/{}/variance_imag", name), observable.variance.imag());
    }

    for (const auto &[name, point_function] : one_point_functions)
    {
        xt::xtensor<int, 1> indices = xt::zeros<int>({point_function.size()});
        xt::xtensor<double, 1> real = xt::zeros<double>({point_function.size()});
        xt::xtensor<double, 1> imag = xt::zeros<double>({point_function.size()});
        for (std::size_t i = 0; i < point_function.size(); ++i)
        {
            const auto &instance = point_function[i];
            indices(i) = instance.getIndex();
            real(i) = instance.getValue().real();
            imag(i) = instance.getValue().imag();
        }
        H5Easy::dump(file, fmt::format("/one_point/{}/indices", name), indices);
        H5Easy::dump(file, fmt::format("/one_point/{}/real", name), real);
        H5Easy::dump(file, fmt::format("/one_point/{}/imag", name), imag);
    }

    for (const auto &[name, point_function] : two_point_functions)
    {
        xt::xtensor<int, 2> indices = xt::zeros<int>({point_function.size(), 2ul});
        xt::xtensor<double, 1> real = xt::zeros<double>({point_function.size()});
        xt::xtensor<double, 1> imag = xt::zeros<double>({point_function.size()});
        for (std::size_t i = 0; i < point_function.size(); ++i)
        {
            const auto &instance = point_function[i];
            indices(i, 0) = instance.getIndex1();
            indices(i, 1) = instance.getIndex2();
            real(i) = instance.getValue().real();
            imag(i) = instance.getValue().imag();
        }
        H5Easy::dump(file, fmt::format("/two_point/{}/indices", name), indices);
        H5Easy::dump(file, fmt::format("/two_point/{}/real", name), real);
        H5Easy::dump(file, fmt::format("/two_point/{}/imag", name), imag);
    }

    itensor::writeToFile(psi_path, psi);

    return 0;
}