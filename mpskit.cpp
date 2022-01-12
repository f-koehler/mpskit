#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>
#include <string>

#include "mpskit/cmd/dmrg.hpp"
#include "mpskit/cmd/list_observables.hpp"
#include "mpskit/cmd/list_one_point.hpp"
#include "mpskit/cmd/list_two_point.hpp"

using namespace std::string_literals;

auto main(int argc, char **argv) -> int
{
    CLI::App app{"Perform computations using matrix product states (MPS)"};
    app.require_subcommand();

    std::string input_path;
    std::string output_path;
    std::string psi_path;

    auto app_dmrg = app.add_subcommand("dmrg", "Compute ground states using DMRG.");
    app_dmrg->add_option("input,-i,--input", input_path, "Input file specifying model and DMRG parameters.")
        ->required();
    app_dmrg->add_option("-o,--output", output_path, "Output file containing DMRG output.")->required();
    app_dmrg->add_option("-p,--psi", psi_path, "File to write the final matrix product state to.")->required();

    auto app_list_observables = app.add_subcommand("list-observables", "List available observables for model.");
    app_list_observables->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    auto app_list_one_point = app.add_subcommand("list-one-point", "List available one-point functions for model.");
    app_list_one_point->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    auto app_list_two_point = app.add_subcommand("list-two-point", "List available two-point functions for model.");
    app_list_two_point->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    CLI11_PARSE(app, argc, argv);

    if (app_dmrg->parsed())
    {
        return cmdDMRG(input_path, output_path, psi_path);
    }

    if (app_list_observables->parsed())
    {
        return cmdListObservables(input_path);
    }

    if (app_list_one_point->parsed())
    {
        return cmdListOnePoint(input_path);
    }

    if (app_list_two_point->parsed())
    {
        return cmdListTwoPoint(input_path);
    }

    return 0;
}
