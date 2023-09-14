project(teracada)

## Set compiler variables
cmake_minimum_required(VERSION 3.17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(ENV{'CC'} 'g++')
set(CMAKE_CXX_STANDARD 17)          # -std=c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)       # turn-off gnu compiler extensions
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unknown-pragmas -Wno-unused-label -Wno-format")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Setting project lib names
set ( _PROJECT_LIB32 ${PROJECT_NAME}32 )
set ( _PROJECT_LIB64 ${PROJECT_NAME}64 )

# Setting include directory path
set ( _INCPATH_PROJECT include/ )

# Setting python extension module directory
set(_SRCS_PYTHON_EXT
  pyapi/
)

# Setting sources
set (_SRCS_TERACADA
  core/data_structures/teracada_array.cc
  core/data_structures/teracada_array_capi.cc
  core/data_structures/teracada_stats.cc
  core/data_structures/teracada_error.cc
  core/regression/teracada_regression.cc
  core/regression/teracada_regression_capi.cc
)

# Setting unit tests
set (_SRCS_UNIT_TESTS 
  core/data_structures/teracada_array_unit_tests.cc
)

## Add project and other dependency rcore project's header files and link directories
include_directories(${_INCPATH_PROJECT})
include_directories(../logman ../utility_belt)
link_directories(../logman ../utility_belt)
set ( _RCORE_LINK_LIBRARIES
  logman
  utility_belt
)

# Create RCORE directories: artifacts and include/teracada, if doesn't already exists
# file(MAKE_DIRECTORY ../artifacts)
# file(MAKE_DIRECTORY ../include/teracada)

# Build project Teracada-32
add_library(${_PROJECT_LIB32} SHARED ${_SRCS_TERACADA})
target_link_libraries(${_PROJECT_LIB32} PRIVATE ${_RCORE_LINK_LIBRARIES})
target_compile_definitions(${_PROJECT_LIB32} PRIVATE _TERACADA_DTYPE32=1)

## Building unit-tests executable with Teracada-32
add_executable(${_PROJECT_LIB32}_unit_tests ${_SRCS_UNIT_TESTS})
target_link_libraries(${_PROJECT_LIB32}_unit_tests PRIVATE ${_PROJECT_LIB32} ${_RCORE_LINK_LIBRARIES})
target_compile_definitions(${_PROJECT_LIB32}_unit_tests PRIVATE _TERACADA_DTYPE32=1)

## Build project Teracada-64
add_library(${_PROJECT_LIB64} SHARED ${_SRCS_TERACADA})
target_link_libraries(${_PROJECT_LIB64} PRIVATE ${_RCORE_LINK_LIBRARIES})
target_compile_definitions(${_PROJECT_LIB64} PRIVATE _TERACADA_DTYPE64=1)

## Building unit-tests executable with Teracada-64
add_executable(${_PROJECT_LIB64}_unit_tests ${_SRCS_UNIT_TESTS})
target_link_libraries(${_PROJECT_LIB64}_unit_tests PRIVATE ${_PROJECT_LIB64} ${_RCORE_LINK_LIBRARIES})
target_compile_definitions(${_PROJECT_LIB64}_unit_tests PRIVATE _TERACADA_DTYPE64=1)

# Build Teracada-Python module
add_custom_command(
  TARGET ${_PROJECT_LIB64} POST_BUILD
  COMMENT "Building Teracada-Python compiler extension module:"
  WORKING_DIRECTORY ${_SRCS_PYTHON_EXT}
  COMMAND bash -c "python3 setup.py build"
)

# Set output directory in target properties
set_target_properties( ${_PROJECT_LIB32} ${_PROJECT_LIB64} ${_PROJECT_LIB32}_unit_tests ${_PROJECT_LIB64}_unit_tests
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY artifacts/
    LIBRARY_OUTPUT_DIRECTORY artifacts/
    RUNTIME_OUTPUT_DIRECTORY artifacts/
)

# Set install destinations for Teracada-64
install(TARGETS ${_PROJECT_LIB32} ${_PROJECT_LIB64}
  ARCHIVE DESTINATION /usr/local/lib64/
  LIBRARY DESTINATION /usr/local/lib64/
)

# Install rule for all the project headers
install(DIRECTORY ${_INCPATH_PROJECT} DESTINATION /usr/local/include/teracada)

# Install Teracada-Python module
install(CODE "message(\"Installing Teracada-Python module: \")" CODE "execute_process(WORKING_DIRECTORY ${_SRCS_PYTHON_EXT} COMMAND python3 setup.py install)")

# Copy all the Teracada lib artifacts to RCORE artifacts directory
# add_custom_command(
#   TARGET ${_PROJECT_LIB32} ${_PROJECT_LIB64} POST_BUILD
#   COMMENT "Copying Teracada lib artifacts to RCORE artifacts: "
#   COMMAND bash -c "/bin/cp --verbose artifacts/*.so ../artifacts/"
# )

# Copy all the Teracada include headers to RCORE include directory
# add_custom_command(
#   TARGET ${_PROJECT_LIB32} ${_PROJECT_LIB64} POST_BUILD
#   COMMENT "Copying Teracada include headers to RCORE include directory: "
#   COMMAND bash -c "/bin/cp --verbose include/* ../include/teracada/"
# )

