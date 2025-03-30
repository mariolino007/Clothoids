##
# PYTHON Wrapper for Clothoids
#
# License MIT - See LICENSE file
# 2019 Matteo Ragni, Claudio Kerov Ghiglianovich,
#      Enrico Bertolazzi, Marco Frego
#

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(G2lib VERSION 1.0)

option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(BUILD_PYTHON_BINDINGS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)

    find_package(pybind11 REQUIRED)
    find_package(Eigen3 3.3 REQUIRED)

    add_subdirectory(../src Clothoids)

    set(G2LIB_SOURCES
        "src/python-Triangle2D.cc"
        "src/python-LineSegment.cc"
        "src/python-G2lib.cc"
        "src/python-Clothoid.cc"
        "src/python-CircleArc.cc"
        "src/python-Biarc.cc"
        "src/python-BaseCurve.cc"
        "src/python-AABBtree.cc"
        "src/python-ClothoidSpline-Solver.cc"
        "src/python-ClothoidSpline-LMSolver.cc"
        "src/python-ClothoidSpline-IpoptSolver.cc"
        "src/python-ClothoidSpline-Interpolation.cc")

    pybind11_add_module(${PROJECT_NAME} MODULE ${G2LIB_SOURCES})

    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ClothoidsStatic 
        UtilsStatic 
        QuarticStatic
    )

    if(WIN32)
        target_sources(${PROJECT_NAME} PRIVATE exports.def)
        target_link_options(${PROJECT_NAME} PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/exports.def")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            SUFFIX ".pyd"
            OUTPUT_NAME "G2lib"
        )
    endif()

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
    include(./CMakeLists-Flags.txt)
    target_assign_flags(${PROJECT_NAME})

endif()
