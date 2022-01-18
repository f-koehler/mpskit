#ifndef MPSKIT_CMD_TEBD
#define MPSKIT_CMD_TEBD

#include "../types.hpp"

int cmdTEBD(const std::string &input_path, const std::string &psi_path, int order, const Real &tfinal, const Real &dt,
            int substeps);

#endif /* MPSKIT_CMD_TEBD */
