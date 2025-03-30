cmake_minimum_required(VERSION 3.12)
project(G2lib)

# Configurazione pybind11
find_package(pybind11 REQUIRED)
add_subdirectory(../submodules/pybind11)

# File sorgente Python
file(GLOB PYTHON_SOURCES *.cc)

# Crea il modulo Python
pybind11_add_module(G2lib 
    ${PYTHON_SOURCES}
    python-Triangle2D.cc
    python-LineSegment.cc
    python-G2lib.cc
    python-Clothoid.cc
    python-CircleArc.cc
    python-Biarc.cc
    python-BaseCurve.cc
    python-AABBtree.cc
    python-ClothoidSpline-Solver.cc
    python-ClothoidSpline-LMSolver.cc
    python-ClothoidSpline-IpoptSolver.cc
    python-ClothoidSpline-Interpolation.cc
)

# Impostazioni specifiche per Windows
if(WIN32)
    set_target_properties(G2lib PROPERTIES
        SUFFIX ".pyd"
        OUTPUT_NAME "G2lib"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
    )
    
    target_compile_definitions(G2lib PRIVATE
        _USE_MATH_DEFINES
        NOMINMAX
    )
endif()

# Collegamento con le librerie
target_link_libraries(G2lib PRIVATE
    ClothoidsStatic
    UtilsStatic
    QuarticStatic
)

# Include directory
target_include_directories(G2lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIRS}
)
