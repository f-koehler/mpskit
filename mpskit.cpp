#include <CLI/CLI.hpp>
#include <array>
#include <chrono>
#include <fmt/core.h>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5File.hpp>
#include <highfive/bits/H5DataSet_misc.hpp>
#include <highfive/bits/H5File_misc.hpp>
#include <highfive/bits/H5Node_traits_misc.hpp>
#include <highfive/bits/H5Object_misc.hpp>
#include <highfive/bits/H5PropertyList_misc.hpp>
#include <highfive/bits/H5Slice_traits_misc.hpp>
#include <highfive/h5easy_bits/H5Easy_public.hpp>
#include <initializer_list>
#include <iostream>
#include <itensor/mps/dmrg.h>
#include <itensor/util/readwrite.h>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <xtensor/xtensor.hpp>

#include "mpskit/json.hpp"
#include "mpskit/models/registry.hpp"
#include "mpskit/observer.hpp"
#include "mpskit/types.hpp"
#include "mpskit/util.hpp"

using namespace std::string_literals;

constexpr Real nanoseconds_to_seconds = 1e9;

int cmd_dmrg(const std::string &input_path, const std::string &output_path, const std::string &psi_path)
{
    auto input = load_json(input_path);
    auto model = create_model_1d(input["model"]);

    const auto sweeps = get_sweeps_from_json(input["dmrg"]["sweeps"]);

    const auto hamiltonian = model->get_hamiltonian();
    auto psi0 = model->get_initial_state();
    auto observer = Observer(psi0, model);

    const auto start_monotonic = std::chrono::steady_clock::now();
    const auto start_hires = std::chrono::high_resolution_clock::now();
    auto [energy, psi] = dmrg(hamiltonian, psi0, sweeps, observer);
    const auto stop_hires = std::chrono::high_resolution_clock::now();
    const auto stop_monotonic = std::chrono::steady_clock::now();

    auto observables = model->get_observables();
    // for (auto &[_, observable] : observables)
    // {
    //     observable(psi);
    // }

    // auto one_point_functions = model->get_one_point_functions();
    // for (auto &[_, one_point_function] : one_point_functions)
    // {
    //     for (auto &instance : one_point_function)
    //     {
    //         instance(psi);
    //     }
    // }

    // auto two_point_functions = model->get_two_point_functions();
    // for (auto &[_, two_point_function] : two_point_functions)
    // {
    //     for (auto &instance : two_point_function)
    //     {
    //         instance(psi);
    //     }
    // }

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
    H5Easy::dump(file, "/convergence/energy", observer.getEnergies());

    for (const auto &[name, values] : observer.getObservableValues())
    {
        H5Easy::dump(file, fmt::format("/convergence/observables/{}/value", name), values);
    }

    for (const auto &[name, values] : observer.getObservableSquaredValues())
    {
        H5Easy::dump(file, fmt::format("/convergence/observables/{}/squared", name), values);
    }

    for (const auto &[name, values] : observer.getObservableVariances())
    {
        H5Easy::dump(file, fmt::format("/convergence/observables/{}/variance", name), values);
    }
    // for (const auto &[name, observable] : observables)
    // {
    //     H5Easy::dump(file, fmt::format("/observables/{}/real", name), observable.value.real());
    //     H5Easy::dump(file, fmt::format("/observables/{}/imag", name), observable.value.imag());
    //     H5Easy::dump(file, fmt::format("/observables/{}/squared_real", name), observable.squared.real());
    //     H5Easy::dump(file, fmt::format("/observables/{}/squared_imag", name), observable.squared.imag());
    //     H5Easy::dump(file, fmt::format("/observables/{}/variance_real", name), observable.variance.real());
    //     H5Easy::dump(file, fmt::format("/observables/{}/variance_imag", name), observable.variance.imag());
    // }

    // for (const auto &[name, point_function] : one_point_functions)
    // {
    //     xt::xtensor<int, 1> indices = xt::zeros<int>({point_function.size()});
    //     xt::xtensor<double, 1> real = xt::zeros<double>({point_function.size()});
    //     xt::xtensor<double, 1> imag = xt::zeros<double>({point_function.size()});
    //     for (std::size_t i = 0; i < point_function.size(); ++i)
    //     {
    //         const auto &instance = point_function[i];
    //         indices(i) = instance.getIndex();
    //         real(i) = instance.getValue().real();
    //         imag(i) = instance.getValue().imag();
    //     }
    //     H5Easy::dump(file, fmt::format("/one_point/{}/indices", name), indices);
    //     H5Easy::dump(file, fmt::format("/one_point/{}/real", name), real);
    //     H5Easy::dump(file, fmt::format("/one_point/{}/imag", name), imag);
    // }

    // for (const auto &[name, point_function] : two_point_functions)
    // {
    //     xt::xtensor<int, 2> indices = xt::zeros<int>({point_function.size(), 2ul});
    //     xt::xtensor<double, 1> real = xt::zeros<double>({point_function.size()});
    //     xt::xtensor<double, 1> imag = xt::zeros<double>({point_function.size()});
    //     for (std::size_t i = 0; i < point_function.size(); ++i)
    //     {
    //         const auto &instance = point_function[i];
    //         indices(i, 0) = instance.getIndex1();
    //         indices(i, 1) = instance.getIndex2();
    //         real(i) = instance.getValue().real();
    //         imag(i) = instance.getValue().imag();
    //     }
    //     H5Easy::dump(file, fmt::format("/two_point/{}/indices", name), indices);
    //     H5Easy::dump(file, fmt::format("/two_point/{}/real", name), real);
    //     H5Easy::dump(file, fmt::format("/two_point/{}/imag", name), imag);
    // }

    itensor::writeToFile(psi_path, psi);

    return 0;
}

int cmd_list_observables(const std::string &input)
{
    auto config = load_json(input);
    auto model = create_model_1d(config["model"]);
    for (const auto &[name, _] : model->get_observables())
    {
        std::cout << name << '\n';
    }
    return 0;
}

int cmd_list_one_point(const std::string &input)
{
    auto config = load_json(input);
    auto model = create_model_1d(config["model"]);
    for (const auto &[name, _] : model->get_one_point_functions())
    {
        std::cout << name << '\n';
    }
    return 0;
}

int cmd_list_two_point(const std::string &input)
{
    auto config = load_json(input);
    auto model = create_model_1d(config["model"]);
    for (const auto &[name, _] : model->get_two_point_functions())
    {
        std::cout << name << '\n';
    }
    return 0;
}

auto main(int argc, char **argv) -> int
{
    CLI::App app{"Perform computations using matrix product states (MPS)"};
    app.require_subcommand();

    std::string input_path;
    std::string output_path;
    std::string psi_path;

    auto app_dmrg = app.add_subcommand("dmrg", "Compute ground states using DMRG.");
    app_dmrg->add_option("input,-i, --input", input_path, "Input file specifying model and DMRG parameters.")
        ->required();
    app_dmrg->add_option("-o, --output", input_path, "Output file containing DMRG output.")->required();
    app_dmrg->add_option("-p, --psi", psi_path, "File to write the final matrix product state to.")->required();

    auto app_list_observables = app.add_subcommand("list-observables", "List available observables for model.");
    app_list_observables->add_option("input,-i, --input", input_path, "Input file specifying model.")->required();

    auto app_list_one_point = app.add_subcommand("list-one-point", "List available one-point functions for model.");
    app_list_one_point->add_option("input,-i, --input", input_path, "Input file specifying model.")->required();

    auto app_list_two_point = app.add_subcommand("list-two-point", "List available two-point functions for model.");
    app_list_two_point->add_option("input,-i, --input", input_path, "Input file specifying model.")->required();

    CLI11_PARSE(app, argc, argv);

    if (app_dmrg->parsed())
    {
        return cmd_dmrg(input_path, output_path, psi_path);
    }

    if (app_list_observables->parsed())
    {
        return cmd_list_observables(input_path);
    }

    if (app_list_one_point->parsed())
    {
        return cmd_list_one_point(input_path);
    }

    if (app_list_two_point->parsed())
    {
        return cmd_list_two_point(input_path);
    }

    return 0;
}
