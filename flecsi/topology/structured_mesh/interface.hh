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

/*! @file */

#if !defined(__FLECSI_PRIVATE__)
#error Do not include this file directly!
#else
#include <flecsi/data/common/data_reference.hh>
#include <flecsi/topology/structured_mesh/types.hh>
#endif

namespace flecsi {
namespace topology {

//----------------------------------------------------------------------------//
// Mesh topology.
//----------------------------------------------------------------------------//

/*!
  @ingroup topology
 */

template<typename POLICY_TYPE>
struct structured_mesh_topology_u : public structured_mesh_topology_base_t,
                                    public data::data_reference_base_t {
}; // struct structured_mesh_topology_u

} // namespace topology
} // namespace flecsi