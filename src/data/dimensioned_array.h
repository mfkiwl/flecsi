/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@ @@       @@@@@@@@ @@     @@ @@
 * /@@///// /@@      /@@///// //@@   @@ /@@
 * /@@      /@@      /@@       //@@ @@  /@@
 * /@@@@@@@ /@@      /@@@@@@@   //@@@   /@@
 * /@@////  /@@      /@@////     @@/@@  /@@
 * /@@      /@@      /@@        @@ //@@ /@@
 * /@@      /@@@@@@@@/@@@@@@@@ @@   //@@/@@
 * //       //////// //////// //     // // 
 * 
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef flexi_dimensioned_array_h
#define flexi_dimensioned_array_h

#include <array>
#include <cmath>

#include "../utils/common.h"

/*!
 * \file dimensioned_array.h
 * \authors bergen
 * \date Initial file creation: Sep 23, 2015
 */

namespace flexi {

enum class axis : size_t { x = 0, y = 1, z = 2 };

/*!
  \class dimensioned_array dimensioned_array.h
  \brief The dimensioned_array type provides a general base for defining
  contiguous array types that have a specific dimension.  Please look at
  the \ref point and \ref vector types for an example of its use.
 */
template <typename T, size_t D, size_t TD> class dimensioned_array {
public:
  dimensioned_array(const dimensioned_array &a) : data_(a.data_) {}


  //! \brief The value type.
  using value_type = T;

  //! \brief The dimension of the array.
  static constexpr size_t dimension = D;

  //! \brief Default constructor
  //! \remark Without this, the dimensioned_array(A... args) args will get 
  //!         called with zero args!
  dimensioned_array() {} // dimensioned_array

  //! \brief Constructor with initializer list
  //! \param[in] list the initializer list of values
  dimensioned_array(std::initializer_list<T> list) {
    assert(list.size() == D && "dimension size mismatch");
    std::copy(list.begin(), list.end(), data_.begin());
  } // dimensioned_array

  //! \brief Constructor with initializer list
  //! \param[in] list the initializer list of values
  template <typename... A> dimensioned_array(A... args) {
    static_assert( (sizeof...(A) == D),
                   "dimension size mismatch" );
    data_ = {args...};
  }

  //! \brief Constructor with one value.
  //! \param[in] val The value to set the array to
  dimensioned_array(const T & val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] = val;
  } // dimensioned_array

  //! Destructor
  ~dimensioned_array() {}


  //! \brief Return the size of the array.
  static constexpr size_t size() { return dimension; };

  /*!
    \brief

    ADD COMMENT ABOUT SUPPORT FOR ENUMS
   */
  template <typename E> T &operator[](E e) {
    return data_[static_cast<size_t>(e)];
  } // operator ()

  template <typename E> const T &operator[](E e) const {
    return data_[static_cast<size_t>(e)];
  } // operator ()


  //! \brief Assignment operator to another array.
  //! \param[in] rhs The array on the right hand side of the '='.
  //! \return A reference to the current object.
  dimensioned_array &operator=(const dimensioned_array &rhs) {
    if ( this != &rhs ) data_ = rhs.data_;
    return *this;
  }

  //! \brief Assignment operator to a constant.
  //! \param[in] val The constant on the right hand side of the '='.
  //! \return A reference to the current object.
  dimensioned_array &operator=(const T &val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] = val;    
    return *this;
  }

  // use std::move
  // http://stackoverflow.com/questions/11726171/numeric-vector-operator-overload-rvalue-reference-parameter
  
  //! \brief Addition binary operator involving another array.
  //! \param[in] rhs The array on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator+=(const dimensioned_array &rhs) {
    if ( this != &rhs ) {
      for ( size_t i=0; i<D; i++ ) 
        data_[i] += rhs[i];    
    }
    return *this;
  }

  //! \brief Addiition binary operator involving a constant.
  //! \param[in] val The constant on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator+=(const T &val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] += val;    
    return *this;
  }

  //! \brief Subtraction binary operator involving another array.
  //! \param[in] rhs The array on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator-=(const dimensioned_array &rhs) {
    if ( this != &rhs ) {
      for ( size_t i=0; i<D; i++ ) 
        data_[i] -= rhs[i];
    }
    return *this;
  }

  //! \brief Subtraction binary operator involving a constant.
  //! \param[in] val The constant on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator-=(const T &val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] -= val;    
    return *this;
  }


  //! \brief Multiplication binary operator involving another array.
  //! \param[in] rhs The array on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator*=(const dimensioned_array &rhs) {
    if ( this != &rhs ) {
      for ( size_t i=0; i<D; i++ ) 
        data_[i] *= rhs[i];    
    }
    return *this;
  }

  //! \brief Multiplication binary operator involving a constant.
  //! \param[in] val The constant on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator*=(const T &val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] *= val;    
    return *this;
  }

  //! \brief Division binary operator involving another array.
  //! \param[in] rhs The array on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator/=(const dimensioned_array &rhs) {
    if ( this != &rhs ) {
      for ( size_t i=0; i<D; i++ ) 
        data_[i] /= rhs[i];    
    }
    return *this;
  }

  //! \brief Division binary operator involving a constant.
  //! \param[in] val The constant on the right hand side of the operator.
  //! \return A reference to the current object.
  dimensioned_array &operator/=(const T &val) {
    for ( size_t i=0; i<D; i++ ) 
      data_[i] /= val;    
    return *this;
  }

private:

  //! \brief The main data container, which is just a std::array.
  std::array<T, D> data_;

}; // class dimensioned_array

  
//! \brief Addition operator involving two dimensioned_arrays.
//! \tparam T  The array base value type.
//! \tparam D  The array dimension.
//! \tparam TD Type differentiator.
//! \param[in] lhs The dimensioned_array on the left hand side of the operator.
//! \param[in] rhs The dimensioned_array on the right hand side of the operator.
//! \return A reference to the current object.
template <typename T, size_t D, size_t TD>
dimensioned_array<T,D,TD> operator+( const dimensioned_array<T,D,TD>& lhs, 
                                     const dimensioned_array<T,D,TD>& rhs )
{
  dimensioned_array<T,D,TD> tmp(lhs);
  tmp += rhs;
  return tmp;
}

//! \brief Subtraction operator involving two dimensioned_arrays.
//! \tparam T  The array base value type.
//! \tparam D  The array dimension.
//! \tparam TD Type differentiator.
//! \param[in] lhs The dimensioned_array on the left hand side of the operator.
//! \param[in] rhs The dimensioned_array on the right hand side of the operator.
//! \return A reference to the current object.
template <typename T, size_t D, size_t TD>
dimensioned_array<T,D,TD> operator-( const dimensioned_array<T,D,TD>& lhs, 
                                     const dimensioned_array<T,D,TD>& rhs )
{
  dimensioned_array<T,D,TD> tmp(lhs);
  tmp -= rhs;
  return tmp;
}

//! \brief Output operator for dimensioned_array.
//! \tparam T  The array base value type.
//! \tparam D  The array dimension.
//! \tparam TD Type differentiator.
//! \param[in,out] os  The ostream to dump output to.
//! \param[in]     rhs The dimensioned_array on the right hand side of the operator.
//! \return A reference to the current ostream.
template <typename T, size_t D, size_t TD>
std::ostream& operator<<(std::ostream& os, const dimensioned_array<T,D,TD>& a)
{
  os << "[";
  for ( size_t i=0; i<D; i++ ) 
    os << " " << a[i];
  os << " ]";
  return os;
}

} // namespace flexi

#endif // flexi_dimensioned_array_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
