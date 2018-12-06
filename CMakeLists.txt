cmake_minimum_required (VERSION 3.11)
project(QuadratureAnalyzer)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
QuadratureAnalyserAnalyzer.cpp
QuadratureAnalyserAnalyzerResults.cpp
QuadratureAnalyserAnalyzerSettings.cpp
QuadratureAnalyserSimulationDataGenerator.cpp
QuadratureAnalyserAnalyzer.h
QuadratureAnalyserAnalyzerResults.h
QuadratureAnalyserAnalyzerSettings.h
QuadratureAnalyserSimulationDataGenerator.h
)

add_analyzer_plugin(QuadratureAnalyzer SOURCES ${SOURCES})
