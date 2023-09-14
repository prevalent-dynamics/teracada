#!/bin/bash

set -eux

_BUILD_WITH_DEPENDENCIES=${1:-${_BUILD_WITH_DEPENDENCIES:-false}}

if [[ "$_BUILD_WITH_DEPENDENCIES" == true ]]; then
  (cd ..; cmake3 . && make); rtn=$?
  exit $rtn
fi

g++ -std=c++17 \
  -I ../logman/ -I ../utility_belt \
  teracada_graph.cc unit_tests_teracada_graph.cc \
  ../logman/liblogman.a ../utility_belt/libutility_belt.a \
  -o teracada
