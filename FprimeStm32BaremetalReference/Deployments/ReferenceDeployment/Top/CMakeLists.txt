####
# F Prime CMakeLists.txt:
#
# SOURCES: list of source files (to be compiled)
# AUTOCODER_INPUTS: list of files to be passed to the autocoders
# DEPENDS: list of libraries that this module depends on
#
# More information in the F´ CMake API documentation:
# https://fprime.jpl.nasa.gov/latest/docs/reference/api/cmake/API/
#
####

register_fprime_module(
    ReferenceDeployment_Top
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/instances.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/system.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/topology.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ReferenceDeploymentTopology.cpp"
    DEPENDS
        Fw_Logger
        FprimeStm32Allocator
)

target_include_directories(ReferenceDeployment_Top PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../.."
    "${CMAKE_CURRENT_BINARY_DIR}/../.."
)
