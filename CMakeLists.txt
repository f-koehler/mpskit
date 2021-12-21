cmake_minimum_required(VERSION 3.16)

project(dmrg LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O")

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(NOT result)
    message(WARNING "IPO is not supported: ${output}")
endif()

find_package(HDF5 REQUIRED)
find_package(Boost REQUIRED)
find_package(LAPACK REQUIRED)

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
target_include_directories(itensor SYSTEM BEFORE PRIVATE dependencies/itensor ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(itensor PRIVATE $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)
if(IPO_SUPPORTED)
    set_property(TARGET itensor PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
if (${BLA_VENDOR} MATCHES "Intel" OR "mkl")
    add_definitions(-DPLATFORM_mkl)
elseif(${BLA_VENDOR} MATCHES ACML)
    add_definitions(-PLATFORM_mkl)
elseif(${BLA_VENDOR} MATCHES Apple)
    add_definitions(-DPLATFORM_macos)
else()
    add_definitions(-DPLATFORM_lapack)
endif()
target_link_libraries(LAPACK::LAPACK)

macro(configure_dmrg_binary target)
    target_include_directories(${target} SYSTEM BEFORE PRIVATE
        dependencies/CLI11/include
        dependencies/fmt/include
        dependencies/highfive/include
        dependencies/json/include
        dependencies/xtensor/include
        dependencies/xtl/include
        dependencies/itensor
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_link_libraries(${target} PRIVATE lapack blas itensor pthread)
    target_link_libraries(${target} PRIVATE HDF5::HDF5 Boost::boost)

    target_compile_definitions(${target} PRIVATE H5_USE_XTENSOR FMT_HEADER_ONLY)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wconversion -Wsign-conversion -pedantic -Werror -pedantic-errors>
    )

    if(IPO_SUPPORTED)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endmacro()

add_library(dmrg_common STATIC
    dmrg/models/bose_hubbard_1d.cpp
    dmrg/models/transverse_ising_1d.cpp
    dmrg/models/spin_half_1d.cpp
    dmrg/util.cpp
    dmrg/observer.cpp
)
configure_dmrg_binary(dmrg_common)


add_executable(
    dmrg
    dmrg.cpp
)
configure_dmrg_binary(dmrg)
target_link_libraries(dmrg PRIVATE dmrg_common)
