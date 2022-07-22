#include "overlap.hpp"

#include <highfive/H5Easy.hpp>
#include <itensor/mps/mps.h>
#include <itensor/util/readwrite.h>

int cmdOverlap(const std::string &psi_path, const std::string &psi2_path, const std::string &output_path)
{
    auto psi1 = itensor::readFromFile<itensor::MPS>(psi_path);
    auto psi2 = itensor::readFromFile<itensor::MPS>(psi2_path);
    const auto overlap = itensor::innerC(psi1, psi2);
    const auto overlap_abs = std::abs(overlap);

    std::cout << "overlap = " << overlap_abs << '\n';

    H5Easy::File file(output_path, H5Easy::File::Overwrite | H5Easy::File::Create);
    H5Easy::dump(file, "overlap", overlap_abs);

    return 0;
}