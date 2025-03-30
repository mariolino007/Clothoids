##
# PYTHON Wrapper for Clothoids
#
# License MIT - See LICENSE file
# 2019 Matteo Ragni, Claudio Kerov Ghiglianovich,
#      Enrico Bertolazzi, Marco Frego
#

cmake_minimum_required(VERSION 3.10.2)

project(G2lib VERSION 1.0)

option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

if(BUILD_PYTHON_BINDINGS)
    # Configurazione specifica per Python
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

    # Path di output espliciti
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)

    find_package(pybind11 REQUIRED)
    find_package(Eigen3 3.3 REQUIRED)

    # Aggiungi solo i sorgenti Python
    add_subdirectory(../src Clothoids)

    set(PYTHON_SOURCES
        src/python-Triangle2D.cc
        src/python-LineSegment.cc
        src/python-G2lib.cc
        src/python-Clothoid.cc
        src/python-CircleArc.cc
        src/python-Biarc.cc
        src/python-BaseCurve.cc
        src/python-AABBtree.cc
        src/python-ClothoidSpline-Solver.cc
        src/python-ClothoidSpline-LMSolver.cc
        src/python-ClothoidSpline-IpoptSolver.cc
        src/python-ClothoidSpline-Interpolation.cc)

    # Crea il modulo Python
    pybind11_add_module(G2lib ${PYTHON_SOURCES})

    target_include_directories(G2lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIRS}
    )

    target_link_libraries(G2lib PRIVATE 
        ClothoidsStatic 
        UtilsStatic 
        QuarticStatic
    )

    # Configurazione specifica per Windows
    if(WIN32)
        target_compile_definitions(G2lib PRIVATE
            _USE_MATH_DEFINES
            NOMINMAX
            Py_ENABLE_SHARED=1
        )
        set_target_properties(G2lib PROPERTIES
            SUFFIX ".pyd"
            OUTPUT_NAME "G2lib"
        )
    endif()

    # Impostazioni compilazione
    target_compile_features(G2lib PUBLIC cxx_std_17)
    include(./CMakeLists-Flags.txt)
    target_assign_flags(G2lib)

endif()
