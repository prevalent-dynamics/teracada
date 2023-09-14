#!/bin/bash

set -eux

_BUILD_WITH_DEPENDENCIES=${1:-${_BUILD_WITH_DEPENDENCIES:-false}}

if [[ "$_BUILD_WITH_DEPENDENCIES" == true ]]; then
  (cd ..; cmake3 . && make); rtn=$?
  exit $rtn
fi

g++ -std=c++17 \
  -I ../include -I ../../logman/ -I ../../utility_belt \
  ../core/teracada_array.cc \
  ../core/teracada_array_capi.cc \
  ../core/teracada_stats.cc \
  ../core/teracada_error.cc \
  ../unit_tests/unit_tests.cc \
  ../../logman/liblogman.a ../../utility_belt/libutility_belt.a \
  -o ../artifacts/teracada_array


# # Teracada-Matrix
# g++ -std=c++17 \
#   -I ../include -I ../../logman/ -I ../../utility_belt \
#   ../core/teracada_array.cc \
#   ../core/teracada_error.cc \
#   ../core/teracada_matrix.cc \
#   ../unit_tests/unit_tests_matrix.cc \
#   ../../logman/liblogman.a ../../utility_belt/libutility_belt.a \
#   -o ../artifacts/teracada_matrix


# Teracada-Graph
# g++ -std=c++17 \
#   -I ../include -I ../../logman/ -I ../../utility_belt \
#   ../core/teracada_array.cc \
#   ../core/teracada_error.cc \
#   ../core/teracada_matrix.cc \
#   ../core/teracada_graph.cc \
#   ../unit_tests/unit_tests_graph.cc \
#   ../../logman/liblogman.a ../../utility_belt/libutility_belt.a \
#   -o ../artifacts/teracada_matrix


# g++ -std=c++17 -shared -fPIC \
#   -I ../include -I ../../logman/ -I ../../utility_belt \
#   ../core/teracada_array.cc \
#   ../core/teracada_array_capi.cc \
#   ../core/teracada_matrix.cc \
#   ../../logman/liblogman.a ../../utility_belt/libutility_belt.a \
#   -o ../artifacts/libteracada.so

# g++ -std=c++17 -shared -fPIC \
#   -I ../include -I ../../logman/ -I ../../utility_belt \
#   ../core/teracada_array.cc \
#   ../core/teracada_array_capi.cc \
#   ../core/teracada_error.cc \
#   ../../logman/liblogman.a ../../utility_belt/libutility_belt.a \
#   -o ../artifacts/libteracada.so
