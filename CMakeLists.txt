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

include(ExternalProject)
ExternalProject_Add(
    itensor_project
    GIT_REPOSITORY https://github.com/ITensor/ITensor.git
    GIT_TAG v3.1.10
    PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/options.mk <SOURCE_DIR>
    CONFIGURE_COMMAND ""
    # BUILD_COMMAND make
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
)
ExternalProject_Get_property(itensor_project SOURCE_DIR)
set(ITENSOR_SOURCE_DIR ${SOURCE_DIR})

find_package(HDF5)
find_package(Boost)

macro(configure_dmrg_binary target)
    target_include_directories(${target} SYSTEM BEFORE PRIVATE
        dependencies/CLI11/include
        dependencies/fmt/include
        dependencies/highfive/include
        dependencies/json/include
        dependencies/xtensor/include
        dependencies/xtl/include
    )
    target_include_directories(${target} SYSTEM BEFORE PRIVATE ${ITENSOR_SOURCE_DIR})
    target_link_directories(${target} BEFORE PRIVATE ${ITENSOR_SOURCE_DIR}/lib)
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
add_dependencies(dmrg_common itensor_project)


add_executable(
    dmrg
    dmrg.cpp
)
configure_dmrg_binary(dmrg)
target_link_libraries(dmrg PRIVATE dmrg_common)
