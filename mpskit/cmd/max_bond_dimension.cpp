#include "max_bond_dimension.hpp"

#include <iostream>
#include <itensor/mps/mps.h>
#include <itensor/util/readwrite.h>

int cmdMaxBondDimension(const std::string &psi_path)
{
    auto psi = itensor::readFromFile<itensor::MPS>(psi_path);
    const auto max_bond_dimension = itensor::maxLinkDim(psi);
    std::cout << max_bond_dimension << '\n';
    return 0;
}