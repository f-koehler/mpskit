cmake_minimum_required(VERSION 3.16)

project(dmrg LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O")

include(ExternalProject)
ExternalProject_Add(
    itensor_project
    GIT_REPOSITORY https://github.com/ITensor/ITensor.git
    GIT_TAG v3.1.10
    PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/options.mk <SOURCE_DIR>
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -j10
    BUILD_IN_SOURCE ON
    INSTALL_COMMAND ""
)
ExternalProject_Get_property(itensor_project SOURCE_DIR)
set(ITENSOR_SOURCE_DIR ${SOURCE_DIR})

add_subdirectory(fmt)
add_subdirectory(json)

find_package(fmt)
find_package(HDF5)
find_package(Boost)

add_executable(
    dmrg
    dmrg.cpp
    dmrg/bose_hubbard_1d.cpp
    dmrg/transverse_ising_1d.cpp
    dmrg/util.cpp
)
add_dependencies(dmrg itensor_project)
target_include_directories(dmrg SYSTEM BEFORE PUBLIC ${ITENSOR_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/highfive/include)
target_link_directories(dmrg BEFORE PUBLIC ${ITENSOR_SOURCE_DIR}/lib)
target_link_libraries(dmrg lapack blas itensor pthread)
target_link_libraries(dmrg fmt::fmt nlohmann_json::nlohmann_json HDF5::HDF5 Boost::boost)
target_compile_options(dmrg PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wconversion -Wsign-conversion -pedantic -Werror -pedantic-errors>
)
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_property(TARGET dmrg PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif()
