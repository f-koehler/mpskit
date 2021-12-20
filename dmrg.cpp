#include <CLI/CLI.hpp>
#include <algorithm>
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
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <xtensor/xcomplex.hpp>
#include <xtensor/xfunctor_view.hpp>

#include "dmrg/json.hpp"
#include "dmrg/model.hpp"
#include "dmrg/models/bose_hubbard_1d.hpp"
#include "dmrg/models/transverse_ising_1d.hpp"
#include "dmrg/observer.hpp"
#include "dmrg/types.hpp"
#include "dmrg/util.hpp"

using namespace std::string_literals;

constexpr Real nanoseconds_to_seconds = 1e9;

auto main(int argc, char **argv) -> int
{
    CLI::App app{"Calculate groundstates using DMRG"};
    std::string input_file;
    std::string output_file;
    std::string psi_file;
    app.add_option("-f,--file", input_file, "Input file specifying the model and it's parameters.")->required();
    app.add_option("-o,--output", output_file, "File to write the results to.")->required();
    app.add_option("--psi", psi_file,
                   "File to write the final matrix-product state to, can be used as input for TEBD.");
    CLI11_PARSE(app, argc, argv);

    std::ifstream istrm(input_file);
    if (!istrm.is_open())
    {
        std::cerr << "Failed to open input file: " << input_file << '\n';
        return 1;
    }
    json input;
    istrm >> input;

    const auto sweeps = get_sweeps_from_json(input["sweeps"]);

    std::unique_ptr<Model> model;
    const auto model_name = input["model"].get<std::string>();
    if (model_name == "BoseHubbard1D")
    {
        model = std::unique_ptr<Model>(new BoseHubbard1D(input));
    }
    else if (model_name == "TransverseIsing1D")
    {
        model = std::unique_ptr<Model>(new TransverseIsing1D(input));
    }
    else
    {
        std::cerr << "unknown model: " << model_name << '\n';
        return 1;
    }

    std::map<std::string, Complex> expvals;
    std::map<std::string, Complex> variances;

    const auto hamiltonian = model->get_hamiltonian();
    auto psi0 = model->get_initial_state();
    auto observer = Observer(psi0);

    const auto start_monotonic = std::chrono::steady_clock::now();
    const auto start_hires = std::chrono::high_resolution_clock::now();
    auto [energy, psi] = dmrg(hamiltonian, psi0, sweeps, observer);
    const auto stop_hires = std::chrono::high_resolution_clock::now();
    const auto stop_monotonic = std::chrono::steady_clock::now();

    for (const auto &observable : model->get_observables())
    {
        std::cout << "Computing expectation value: " << observable.name << "\n";
        expvals.insert({observable.name, compute_expectation_value(psi, observable.mpo)});
        if (observable.compute_variance)
        {
            std::cout << "Computing variance: " << observable.name << "\n";
            variances.insert({observable.name, compute_variance(psi, observable.mpo)});
        }
    }

    const auto one_point = model->compute_one_point(psi);
    const auto two_point = model->compute_two_point(psi);

    auto duration_monotonic =
        static_cast<Real>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(stop_monotonic - start_monotonic).count()) /
        nanoseconds_to_seconds;
    auto duration_hires =
        static_cast<Real>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_hires - start_hires).count()) /
        nanoseconds_to_seconds;

    H5Easy::File file(output_file, H5Easy::File::Overwrite);
    H5Easy::dump(file, "/runtimes/monotonic", duration_monotonic);
    H5Easy::dump(file, "/runtimes/hires", duration_hires);
    H5Easy::dump(file, "/convergence/energy", observer.getEnergies());
    for (const auto &[name, value] : expvals)
    {
        H5Easy::dump(file, fmt::format("/expvals/{}/real", name), xt::real(value));
        H5Easy::dump(file, fmt::format("/expvals/{}/imag", name), xt::imag(value));
    }
    for (const auto &[name, value] : variances)
    {
        H5Easy::dump(file, fmt::format("/variances/{}/real", name), xt::real(value));
        H5Easy::dump(file, fmt::format("/variances/{}/imag", name), xt::imag(value));
    }

    // we have to create new array from the real/imaginary view on the value arrays
    // HighFive does not support the return type of xt::real
    // maybe its worthwile to report this issue to the HighFive library
    for (const auto &[name, values] : one_point)
    {
        H5Easy::dump(file, fmt::format("/one_point/{}/real", name), static_cast<RealArray>(xt::real(values)));
        H5Easy::dump(file, fmt::format("/one_point/{}/imag", name), static_cast<RealArray>(xt::imag(values)));
    }

    for (const auto &[name, values] : two_point)
    {
        H5Easy::dump(file, fmt::format("/two_point/{}/real", name), static_cast<RealArray>(xt::real(values)));
        H5Easy::dump(file, fmt::format("/two_point/{}/imag", name), static_cast<RealArray>(xt::imag(values)));
    }

    if (app.count("--psi"))
    {
        itensor::writeToFile(psi_file, psi);
    }

    return 0;
}
