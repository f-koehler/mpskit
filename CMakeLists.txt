cmake_minimum_required(VERSION 3.16)

project(mpskit LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(NOT result)
    message(WARNING "IPO is not supported: ${output}")
endif()

find_package(HDF5 REQUIRED)
find_package(Boost REQUIRED)
find_package(LAPACK REQUIRED)
find_package(Threads REQUIRED)

message(STATUS "BLAS/LAPACK vendor: ${BLA_VENDOR}")

macro(mpskit_compile_options target)
    target_compile_options(itensor PRIVATE $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)
    target_compile_definitions(${target} PRIVATE H5_USE_XTENSOR)
    target_compile_definitions(${target} PRIVATE FMT_HEADER_ONLY)
    target_compile_definitions(${target} PRIVATE XTENSOR_USE_XSIMD)
    if (${BLA_VENDOR} MATCHES "Intel" OR "mkl")
        target_compile_definitions(${target} PRIVATE -DPLATFORM_mkl)
    elseif(${BLA_VENDOR} MATCHES ACML)
        target_compile_definitions(${target} PRIVATE -DPLATFORM_mkl)
    elseif(${BLA_VENDOR} MATCHES Apple)
        target_compile_definitions(${target} PRIVATE -DPLATFORM_macos)
    else()
        target_compile_definitions(${target} PRIVATE -DPLATFORM_lapack)
    endif()
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-O>
    )
    if(IPO_SUPPORTED)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endmacro()

macro(mpskit_enable_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wconversion -Wsign-conversion -pedantic -Werror -pedantic-errors>
    )
endmacro()

macro(mpskit_include_directories target)
    target_include_directories(${target} SYSTEM BEFORE PRIVATE
        dependencies/CLI11/include
        dependencies/fmt/include
        dependencies/highfive/include
        dependencies/json/include
        dependencies/xtensor/include
        dependencies/xtl/include
        dependencies/xsimd/include
        dependencies/itensor
        ${CMAKE_CURRENT_BINARY_DIR}
    )
endmacro()

macro(mpskit_link_libraries target)
    target_link_libraries(${target} PRIVATE LAPACK::LAPACK)
    target_link_libraries(${target} PRIVATE Threads::Threads)
    target_link_libraries(${target} PRIVATE HDF5::HDF5)
    target_link_libraries(${target} PRIVATE Boost::boost)
endmacro()

configure_file(itensor_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/itensor/config.h COPYONLY)
add_library(itensor STATIC
    dependencies/itensor/itensor/decomp.cc
    dependencies/itensor/itensor/global.cc
    dependencies/itensor/itensor/hermitian.cc
    dependencies/itensor/itensor/index.cc
    dependencies/itensor/itensor/indexset.cc
    dependencies/itensor/itensor/itensor.cc
    dependencies/itensor/itensor/qn.cc
    dependencies/itensor/itensor/spectrum.cc
    dependencies/itensor/itensor/svd.cc
    dependencies/itensor/itensor/tagset.cc
    dependencies/itensor/itensor/itdata/combiner.cc
    dependencies/itensor/itensor/itdata/dense.cc
    dependencies/itensor/itensor/itdata/diag.cc
    # dependencies/itensor/itensor/itdata/itlazy.cc
    dependencies/itensor/itensor/itdata/qcombiner.cc
    dependencies/itensor/itensor/itdata/qdense.cc
    dependencies/itensor/itensor/itdata/qdiag.cc
    dependencies/itensor/itensor/itdata/scalar.cc
    dependencies/itensor/itensor/mps/autompo.cc
    dependencies/itensor/itensor/mps/mpo.cc
    dependencies/itensor/itensor/mps/mpoalgs.cc
    dependencies/itensor/itensor/mps/mps.cc
    dependencies/itensor/itensor/mps/mpsalgs.cc
    dependencies/itensor/itensor/tensor/algs.cc
    dependencies/itensor/itensor/tensor/contract.cc
    dependencies/itensor/itensor/tensor/gemm.cc
    dependencies/itensor/itensor/tensor/lapack_wrap.cc
    dependencies/itensor/itensor/tensor/mat.cc
    dependencies/itensor/itensor/tensor/vec.cc
    dependencies/itensor/itensor/util/args.cc
    dependencies/itensor/itensor/util/cputime.cc
    dependencies/itensor/itensor/util/input.cc
)
mpskit_compile_options(itensor)
mpskit_include_directories(itensor)
mpskit_link_libraries(itensor)

add_library(mpskit_common STATIC
    mpskit/cmd/dmrg.cpp
    mpskit/cmd/entanglement_entropy.cpp
    mpskit/cmd/list_observables.cpp
    mpskit/cmd/list_one_point.cpp
    mpskit/cmd/list_two_point.cpp
    mpskit/json.cpp
    mpskit/models/bose_hubbard_1d.cpp
    mpskit/models/bosonic_1d.cpp
    mpskit/models/heisenberg_1d.cpp
    mpskit/models/ising_lr_1d.cpp
    mpskit/models/model.cpp
    mpskit/models/model_1d.cpp
    mpskit/models/model_2d.cpp
    mpskit/models/registry.cpp
    mpskit/models/spin_half_1d.cpp
    mpskit/models/spin_half_square.cpp
    mpskit/models/transverse_ising_1d.cpp
    mpskit/models/transverse_ising_square.cpp
    mpskit/models/xy_spin_glass_1d.cpp
    mpskit/observable.cpp
    mpskit/observer.cpp
    mpskit/point_functions.cpp
    mpskit/terms.cpp
    mpskit/util.cpp
)
mpskit_compile_options(mpskit_common)
mpskit_enable_warnings(mpskit_common)
mpskit_include_directories(mpskit_common)
mpskit_link_libraries(mpskit_common)
target_link_libraries(mpskit_common PRIVATE itensor)


add_executable(
    mpskit
    mpskit.cpp
)
mpskit_compile_options(mpskit)
mpskit_enable_warnings(mpskit)
mpskit_include_directories(mpskit)
mpskit_link_libraries(mpskit)
target_link_libraries(mpskit PRIVATE itensor)
target_link_libraries(mpskit PRIVATE mpskit_common)