#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>
#include <itensor/mps/dmrg.h>
#include <highfive/H5Easy.hpp>
#include <map>

#include "dmrg/util.hpp"
#include "dmrg/bose_hubbard_1d.hpp"
#include "dmrg/transverse_ising_1d.hpp"
#include "dmrg/point_functions.hpp"

using namespace std::string_literals;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: dmrg input_file\n";
        return 1;
    }

    std::ifstream istrm(argv[1]);
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

    std::map<std::string, double> expvals, variances;

    const auto hamiltonian = model->get_hamiltonian();
    auto psi0 = model->get_initial_state();

    const auto start_monotonic = std::chrono::steady_clock::now();
    const auto start_hires = std::chrono::high_resolution_clock::now();
    const auto [energy, psi] = dmrg(hamiltonian, psi0, sweeps);
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

    auto duration_monotonic = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_monotonic - start_monotonic).count()) / 1e9;
    auto duration_hires = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_hires - start_hires).count()) / 1e9;

    H5Easy::File file("results.h5", H5Easy::File::Overwrite);
    H5Easy::dump(file, "/runtimes/monotonic", duration_monotonic);
    H5Easy::dump(file, "/runtimes/hires", duration_hires);
    for (const auto &[name, value] : expvals)
    {
        H5Easy::dump(file, "/expvals/"s + name, value);
    }
    for (const auto &[name, value] : variances)
    {
        H5Easy::dump(file, "/variances/"s + name, value);
    }

    return 0;
}
