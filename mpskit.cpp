#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>
#include <string>

#include "mpskit/cmd/analyze_basis.hpp"
#include "mpskit/cmd/create_state.hpp"
#include "mpskit/cmd/dmrg.hpp"
#include "mpskit/cmd/entanglement_entropy.hpp"
#include "mpskit/cmd/list_observables.hpp"
#include "mpskit/cmd/list_one_point.hpp"
#include "mpskit/cmd/list_two_point.hpp"
#include "mpskit/cmd/max_bond_dimension.hpp"
#include "mpskit/cmd/overlap.hpp"
#include "mpskit/cmd/tebd.hpp"
#include "mpskit/types.hpp"

using namespace std::string_literals;

auto main(int argc, char **argv) -> int
{
    CLI::App app{"Perform computations using matrix product states (MPS)"};
    app.require_subcommand();

    std::string input_path;
    std::string output_path;
    std::string psi_path;
    std::string psi2_path;
    std::string name;
    int bond;
    // Real tfinal = 1.0;
    // Real dt = 0.05;
    // int substeps = 5;
    // int order = 2;

    auto app_dmrg = app.add_subcommand("dmrg", "Compute ground states using DMRG.");
    app_dmrg->add_option("input,-i,--input", input_path, "Input file specifying model and DMRG parameters.")
        ->required();
    app_dmrg->add_option("-o,--output", output_path, "Output file containing DMRG output.")->required();
    app_dmrg->add_option("-p,--psi", psi_path, "File to write the final matrix product state to.")->required();

    auto app_entanglement_entropy =
        app.add_subcommand("entanglement-entropy", "Compute von Neumann entanglement entropy across a certain bond");
    app_entanglement_entropy->add_option("psi,-p,--psi", psi_path, "Matrix product state to analyze.")->required();
    app_entanglement_entropy->add_option("-o,--output", output_path, "Output file for entanglement-entropy.")
        ->required();
    app_entanglement_entropy->add_option("bond,-b,--bond", bond, "Bond to compute t.")->required();

    // auto app_tebd = app.add_subcommand("tebd", "Compute time-evolution using TEBD.");
    // app_tebd->add_option("input,-i,--input", input_path, "input file spceifying the model.")->required();
    // app_tebd->add_option("-p,--psi", psi_path, "Path to the initial wave function.")->required();
    // app_tebd->add_option("--order", order, "Order of the Trotter-Suzuki formula.");
    // app_tebd->add_option("--tfinal", tfinal, "Final time for the time-evolution.");
    // app_tebd->add_option("--dt", tfinal, "Desired time step.");
    // app_tebd->add_option("-n,--substeps", substeps, "Number of TEBD substeps per dt.");

    auto app_list_observables = app.add_subcommand("list-observables", "List available observables for model.");
    app_list_observables->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    auto app_list_one_point = app.add_subcommand("list-one-point", "List available one-point functions for model.");
    app_list_one_point->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    auto app_list_two_point = app.add_subcommand("list-two-point", "List available two-point functions for model.");
    app_list_two_point->add_option("input,-i,--input", input_path, "Input file specifying model.")->required();

    auto app_analyze_basis = app.add_subcommand("analyze-basis", "Project a MPS state onto all basis states.");
    app_analyze_basis->add_option("-i,--input", input_path, "Input file specifying model.")->required();
    app_analyze_basis->add_option("-p,--psi", psi_path, "Matrix product state to analyze.")->required();
    app_analyze_basis->add_option("-o,--output", output_path, "Output file for the projections.")->required();

    auto app_create_state = app.add_subcommand("create-state", "Store a specific MPS in a file.");
    app_create_state->add_option("name,-n,--name", name, "Name of the MPS to create.")->required();
    app_create_state->add_option("-i,--input", input_path, "Input file specifying model.")->required();
    app_create_state->add_option("-o,--output", output_path, "Output file for the MPS.")->required();

    auto app_overlap = app.add_subcommand("overlap", "Compute the overlap of between two MPS states.");
    app_overlap->add_option("psi1,--psi1", psi_path, "Ket state.")->required();
    app_overlap->add_option("psi2,--psi2", psi2_path, "Bra state.")->required();
    app_overlap->add_option("-o,--output", output_path, "Output file for the overlap.")->required();

    auto app_max_bond_dimension =
        app.add_subcommand("max-bond-dimension", "Compute the maximum bond dimension of a MPS.");
    app_max_bond_dimension->add_option("psi,-p,--psi", psi_path, "Matrix product state to analyze.")->required();

    CLI11_PARSE(app, argc, argv);

    if (app_dmrg->parsed())
    {
        return cmdDMRG(input_path, output_path, psi_path);
    }

    if (app_entanglement_entropy->parsed())
    {
        return cmdEntanglementEntropy(psi_path, output_path, bond);
    }

    // if (app_tebd->parsed())
    // {
    //     return cmdTEBD(input_path, psi_path, order, tfinal, dt, substeps);
    // }

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

    if (app_analyze_basis->parsed())
    {
        return cmdAnalyzeBasis(input_path, psi_path, output_path);
    }

    if (app_create_state->parsed())
    {
        return cmdCreateState(name, input_path, output_path);
    }

    if (app_overlap->parsed())
    {
        return cmdOverlap(psi_path, psi2_path, output_path);
    }

    if (app_max_bond_dimension->parsed())
    {
        return cmdMaxBondDimension(psi_path);
    }

    return 0;
}
