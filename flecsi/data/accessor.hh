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

/*!
  @file

  This file contains implementations of field accessor types.
 */

#if !defined(__FLECSI_PRIVATE__)
#error Do not include this file directly!
#endif

#include "flecsi/data/reference.hh"
#include <flecsi/data/field.hh>

namespace flecsi {
namespace data {

template<typename DATA_TYPE, size_t PRIVILEGES>
struct accessor<singular, DATA_TYPE, PRIVILEGES> {
  using value_type = DATA_TYPE;
  using base_type = accessor<dense, DATA_TYPE, PRIVILEGES>;
  using element_type = typename base_type::element_type;

  template<class Topo, topo::index_space_t<Topo> Space>
  accessor(field_reference<DATA_TYPE, singular, Topo, Space> const & ref)
    : base(ref.template cast<dense>()) {}
  accessor(const base_type & b) : base(b) {}

  element_type & get() const {
    return base(0);
  } // data
  operator element_type &() const {
    return get();
  } // value

  const accessor & operator=(const DATA_TYPE & value) const {
    return const_cast<accessor &>(*this) = value;
  } // operator=
  accessor & operator=(const DATA_TYPE & value) {
    get() = value;
    return *this;
  } // operator=

  base_type & get_base() {
    return base;
  }
  const base_type & get_base() const {
    return base;
  }
  friend base_type * get_null_base(accessor *) { // for task_prologue_t
    return nullptr;
  }

private:
  base_type base;
}; // struct accessor

template<typename DATA_TYPE, size_t PRIVILEGES>
struct accessor<dense, DATA_TYPE, PRIVILEGES> : reference_base {
  using value_type = DATA_TYPE;
  using element_type = std::
    conditional_t<privilege_write(PRIVILEGES), value_type, const value_type>;

  template<class Topo, topo::index_space_t<Topo> Space>
  accessor(field_reference<DATA_TYPE, dense, Topo, Space> const & ref)
    : accessor(ref.fid()) {}
  explicit accessor(std::size_t f) : reference_base(f) {}

  /*!
    Provide logical array-based access to the data referenced by this
    accessor.

    @param index The index of the logical array to access.
   */

  element_type & operator()(size_t index) const {
    flog_assert(index < size_, "index out of range");
    return data_[index];
  } // operator()

  element_type * data() const {
    return data_;
  } // data

private:
  friend void bind(accessor & a, size_t size, element_type * data) {
    a.size_ = size;
    a.data_ = data;
  }

  // These must be initialized to copy into a user's accessor parameter.
  size_t size_ = 0;
  element_type * data_ = nullptr;

}; // struct accessor

} // namespace data
} // namespace flecsi
