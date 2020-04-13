/*
    @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
   /@@/////  /@@          @@////@@ @@////// /@@
   /@@       /@@  @@@@@  @@    // /@@       /@@
   /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
   /@@////   /@@/@@@@@@@/@@       ////////@@/@@
   /@@       /@@/@@//// //@@    @@       /@@/@@
   /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
   //       ///  //////   //////  ////////  //

   Copyright (c) 2016, Triad National Security, LLC
   All rights reserved.
                                                                              */
#pragma once

/*!  @file */

#if !defined(__FLECSI_PRIVATE__)
#error Do not include this file directly!
#endif

#include "flecsi/run/types.hh"

#include <cstddef>
#include <limits>
#include <vector>

namespace flecsi {
namespace data {

/*!
  The field_info_t type provides a structure for capturing runtime field
  information.
 */

struct field_info_t {
  field_id_t fid = FIELD_ID_MAX;
  size_t type_size = std::numeric_limits<size_t>::max();
}; // struct field_info_t

using fields = std::vector<const field_info_t *>;

} // namespace data
} // namespace flecsi
