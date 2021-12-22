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
find_package(Threads REQUIRED)

message(STATUS "BLAS/LAPACK vendor: ${BLA_VENDOR}")

macro(dmrg_compile_options target)
    target_compile_options(itensor PRIVATE $<$<CXX_COMPILER_ID:GNU>:-fconcepts>)
    target_compile_definitions(${target} PRIVATE H5_USE_XTENSOR)
    target_compile_definitions(${target} PRIVATE FMT_HEADER_ONLY)
    if (${BLA_VENDOR} MATCHES "Intel" OR "mkl")
        target_compile_definitions(${target} PRIVATE -DPLATFORM_mkl)
    elseif(${BLA_VENDOR} MATCHES ACML)
        target_compile_definitions(${target} PRIVATE -DPLATFORM_mkl)
    elseif(${BLA_VENDOR} MATCHES Apple)
        target_compile_definitions(${target} PRIVATE -DPLATFORM_macos)
    else()
        target_compile_definitions(${target} PRIVATE -DPLATFORM_lapack)
    endif()
    if(IPO_SUPPORTED)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endmacro()

macro(dmrg_enable_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wconversion -Wsign-conversion -pedantic -Werror -pedantic-errors>
    )
endmacro()

macro(dmrg_include_directories target)
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
endmacro()

macro(dmrg_link_libraries target)
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
dmrg_compile_options(itensor)
dmrg_include_directories(itensor)
dmrg_link_libraries(itensor)

add_library(dmrg_common STATIC
    dmrg/models/bose_hubbard_1d.cpp
    dmrg/models/bosonic_1d.cpp
    dmrg/models/heisenberg_1d.cpp
    dmrg/models/model.cpp
    dmrg/models/model_1d.cpp
    dmrg/models/spin_half_1d.cpp
    dmrg/models/transverse_ising_1d.cpp
    dmrg/observer.cpp
    dmrg/util.cpp
    dmrg/terms.cpp
)
dmrg_compile_options(dmrg_common)
dmrg_enable_warnings(dmrg_common)
dmrg_include_directories(dmrg_common)
dmrg_link_libraries(dmrg_common)
target_link_libraries(dmrg_common PRIVATE itensor)


add_executable(
    dmrg
    dmrg.cpp
)
dmrg_compile_options(dmrg)
dmrg_enable_warnings(dmrg)
dmrg_include_directories(dmrg)
dmrg_link_libraries(dmrg)
target_link_libraries(dmrg PRIVATE itensor)
target_link_libraries(dmrg PRIVATE dmrg_common)
