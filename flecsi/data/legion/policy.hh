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

#include <flecsi/data/legion/types.hh>
#include <flecsi/data/reference.hh>
#include <flecsi/data/storage_classes.hh>
#include <flecsi/runtime/backend.hh>
#include <flecsi/runtime/types.hh>
#include <flecsi/topology/core.hh>
#include <flecsi/topology/unstructured/types.hh>
#include <flecsi/utils/flog.hh>

#if !defined(FLECSI_ENABLE_LEGION)
#error FLECSI_ENABLE_LEGION not defined! This file depends on Legion!
#endif

#include <legion.h>

flog_register_tag(topologies);

namespace flecsi {
namespace data {

template<class C>
struct topology_id {
  // NB: C-style cast supports private inheritance
  topology_id() : id(runtime::context_t::instance().record(*(C *)this)) {}
  topology_id(const topology_id &) : topology_id() {}
  ~topology_id() {
    runtime::context_t::instance().forget(id);
  }
  topology_id & operator=(const topology_id &) noexcept {
    return *this;
  }

  std::size_t id;
};

/*----------------------------------------------------------------------------*
  Unstructured Mesh Topology.
 *----------------------------------------------------------------------------*/

#if 0
  struct entity_walker_t : public utils::tuple_walker<index_walker_t> {

    entity_walker_t(coloring_t const & coloring) : coloring_(coloring) {}

    template<typename ENTITY_TYPE>
    void visit_type() {
    } // visit_type

  private:
    coloring_t coloring_;

  }; // struct entity_walker_t
#endif

inline topology_data<topology::unstructured_base>::topology_data(
  const type::coloring & coloring) {

#if 0
  auto legion_runtime = Legion::Runtime::get_runtime();
  auto legion_context = Legion::Runtime::get_context();
  auto & flecsi_context = runtime::context_t::instance();

  auto & dense_field_info_store = flecsi_context.get_field_info_store<type,...>(storage_label_t::dense);

  auto & ragged_field_info_store = flecsi_context.get_field_info_store<...>(storage_label_t::ragged);

  auto & sparse_field_info_store = flecsi_context.get_field_info_store<...>(storage_label_t::sparse);

#else
  (void)coloring;
#endif
}

// NOTE THAT THE HANDLE TYPE FOR THIS TYPE WILL NEED TO CAPTURE THE
// UNDERLYING TOPOLOGY TYPE, i.e., topology::mesh_t<MESH_POLICY>

#if 0
template<typename MESH_POLICY>
struct client_handle_specialization<topology::mesh_t<MESH_POLICY>> {

  using client_t = topology::mesh_t<MESH_POLICY>;

  template<size_t NAMESPACE, size_t NAME>
  static client_handle<client_t, 0> get_client_handle() {
    client_handle<client_t, 0> h;
    return h;
  } // get_client_handle

}; // client_handle_specialization<topology::mesh_t<MESH_POLICY>>
#endif

} // namespace data
} // namespace flecsi
