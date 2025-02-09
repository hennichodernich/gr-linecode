# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT linecode_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT linecode_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND linecode_python_files
    softuart_rx_python.cc
    softuart_tx_python.cc
    diff_manch_dec_python.cc
    correlate_symbol_seq_tag_ff_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(linecode
   ../..
   gr::linecode
   "${linecode_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/linecode/
  DEPENDS linecode_python)

install(TARGETS linecode_python DESTINATION ${GR_PYTHON_DIR}/linecode COMPONENT pythonapi)
