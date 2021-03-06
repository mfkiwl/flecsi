/*
    @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
   /@@/////  /@@          @@////@@ @@////// /@@
   /@@       /@@  @@@@@  @@    // /@@       /@@
   /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
   /@@////   /@@/@@@@@@@/@@       ////////@@/@@
   /@@       /@@/@@//// //@@    @@       /@@/@@
   /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
   //       ///  //////   //////  ////////  //

   Copyright (c) 2016, Los Alamos National Security, LLC
   All rights reserved.
                                                                              */

#pragma once

#include <flecsi/execution/execution.h>
#include <stddef.h>

namespace flecsi {
namespace tutorial {

struct coloring_map_t {
  size_t vertices;
  size_t cells;
}; // struct coloring_map_t

void add_colorings(coloring_map_t map);
flecsi_register_mpi_task(add_colorings, flecsi::tutorial);

} // namespace tutorial
} // namespace flecsi
