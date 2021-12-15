#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>
#include <itensor/mps/dmrg.h>

#include "dmrg/util.hpp"
#include "dmrg/bose_hubbard_1d.hpp"

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
    else
    {
        std::cerr << "unknown model: " << model_name << '\n';
        return 1;
    }

    const auto start_monotonic = std::chrono::steady_clock::now();
    const auto start_hires = std::chrono::high_resolution_clock::now();

    const auto hamiltonian = model->get_hamiltonian();
    auto psi0 = model->get_initial_state();

    const auto [energy, psi] = dmrg(hamiltonian, psi0, sweeps);

    json expvals, variances, two_point;

    for (const auto &observable : model->get_observables())
    {
        expvals[observable.name] = compute_expectation_value(psi, observable.mpo);
        if (observable.compute_variance)
        {
            variances[observable.name] = compute_variance(psi, observable.mpo);
        }
    }

    for (const auto &correlation : model->get_two_point_correlations())
    {
        two_point[correlation.name] = compute_two_point(psi, correlation.mpo1, correlation.mpo2);
    }

    const auto stop_monotonic = std::chrono::steady_clock::now();
    const auto stop_hires = std::chrono::high_resolution_clock::now();

    json data;
    data["expvals"] = expvals;
    data["variances"] = variances;
    data["two_point"] = two_point;
    data["runtime"] = {
        {"hires", static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_hires - start_hires).count()) / 1e9},
        {"monotonic", static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_monotonic - start_monotonic).count()) / 1e9}};

    std::ofstream strm("results.json");
    strm << data << '\n';

    return 0;
}
