#include "entanglement_entropy.hpp"
#include "../types.hpp"
#include <highfive/H5Easy.hpp>
#include <itensor/mps/mps.h>
#include <itensor/util/readwrite.h>

int cmdEntanglementEntropy(const std::string &psi_path, std::string &output_path, int bond)
{
    // read MPS
    auto psi = itensor::readFromFile<itensor::MPS>(psi_path);

    // prepare MPS
    psi.position(bond);
    auto l = itensor::leftLinkIndex(psi, bond);
    auto s = itensor::siteIndex(psi, bond);

    // compute SVD
    auto [U, S, V] = itensor::svd(psi(bond), {l, s});
    auto u = itensor::commonIndex(U, S);

    // compute SvN
    Real SvN = 0.;
    for (auto n : itensor::range1(itensor::dim(u)))
    {
        auto Sn = itensor::elt(S, n, n);
        auto p = itensor::sqr(Sn);
        if (p > 1e-12)
        {
            SvN -= p * log(p);
        }
    }

    H5Easy::File file(output_path, H5Easy::File::Overwrite);
    H5Easy::dump(file, "/SvN", SvN);
    H5Easy::dump(file, "/bond", bond);

    return 0;
}