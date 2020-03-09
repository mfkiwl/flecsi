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

#include <iostream>
#include <sstream>
#include <string>

#include <flecsi/utils/demangle.hh>
#include <flecsi/utils/flog.hh>
#include <flecsi/utils/ftest/output.hh>

flog_register_tag(ftest);

namespace flecsi {
namespace utils {
namespace ftest {

struct state_t {

  state_t(std::string name) {
    name_ = name;
    error_stream_.str(std::string());
  } // initialize

  ~state_t() {
    flog_tag_guard(ftest);

    if(error_stream_.str().size()) {
      std::stringstream stream;
      stream << FLOG_OUTPUT_LTRED("TEST FAILED " << name_) << FLOG_COLOR_PLAIN
             << std::endl;
      stream << error_stream_.str();
      flog(utility) << stream.str();
    }
    else {
      flog(utility) << FLOG_OUTPUT_LTGREEN("TEST PASSED " << name_)
                    << FLOG_COLOR_PLAIN << std::endl;
    } // if
  } // process

  int & result() {
    return result_;
  }

  const std::string & name() const {
    return name_;
  }

  std::stringstream & stringstream() {
    return error_stream_;
  } // stream

private:
  int result_ = 0;
  std::string name_;
  std::stringstream error_stream_;

}; // struct state_t

struct assert_handler_t {

  assert_handler_t(const char * condition,
    const char * file,
    int line,
    state_t & runtime)
    : runtime_(runtime) {
    runtime_.result() = 1;
    runtime_.stringstream()
      << FLOG_OUTPUT_LTRED("ASSERT FAILED") << ": assertion '" << condition
      << "' failed in " << FLOG_OUTPUT_BROWN(file << ":" << line)
      << FLOG_COLOR_BROWN << " ";
  } // assert_handler_t

  ~assert_handler_t() {
    runtime_.stringstream() << FLOG_COLOR_PLAIN << std::endl;
    std::stringstream stream;
    stream << FLOG_OUTPUT_LTRED("TEST FAILED " << runtime_.name())
           << FLOG_COLOR_PLAIN << std::endl;
    stream << runtime_.stringstream().str();
  } // ~assert_handler_t

  template<typename T>
  assert_handler_t & operator<<(const T & value) {
    runtime_.stringstream() << value;
    return *this;
  } // operator <<

  assert_handler_t & operator<<(
    ::std::ostream & (*basic_manipulator)(::std::ostream & stream)) {
    runtime_.stringstream() << basic_manipulator;
    return *this;
  } // operator <<

  operator int() {
    return 1;
  }

private:
  state_t & runtime_;

}; // assert_handler_t

struct expect_handler_t {

  expect_handler_t(const char * condition,
    const char * file,
    int line,
    state_t & runtime)
    : runtime_(runtime) {
    runtime_.result() = 1;
    runtime_.stringstream()
      << FLOG_OUTPUT_YELLOW("EXPECT FAILED") << ": unexpected '" << condition
      << "' occurred in " << FLOG_OUTPUT_BROWN(file << ":" << line)
      << FLOG_COLOR_BROWN << " ";
  } // expect_handler_t

  ~expect_handler_t() {
    runtime_.stringstream() << FLOG_COLOR_PLAIN << std::endl;
  } // ~expect_handler_t

  template<typename T>
  expect_handler_t & operator<<(const T & value) {
    runtime_.stringstream() << value;
    return *this;
  } // operator <<

  expect_handler_t & operator<<(
    ::std::ostream & (*basic_manipulator)(::std::ostream & stream)) {
    runtime_.stringstream() << basic_manipulator;
    return *this;
  } // operator <<

  operator int() {
    return 1;
  }

private:
  state_t & runtime_;

}; // expect_handler_t

template<typename T1, typename T2>
inline bool
test_equal(const T1 & v1, const T2 & v2) {
  return v1 == v2;
}

template<typename T1, typename T2>
inline bool
test_less(const T1 & v1, const T2 & v2) {
  return v1 < v2;
}

template<typename T1, typename T2>
inline bool
test_greater(const T1 & v1, const T2 & v2) {
  return v1 > v2;
}

inline bool
string_compare(const char * lhs, const char * rhs) {
  if(lhs == nullptr) {
    return rhs == nullptr;
  }
  if(rhs == nullptr) {
    return false;
  }
  return strcmp(lhs, rhs) == 0;
} // string_compare

inline bool
string_case_compare(const char * lhs, const char * rhs) {
  if(lhs == nullptr) {
    return rhs == nullptr;
  }
  if(rhs == nullptr) {
    return false;
  }
  return strcasecmp(lhs, rhs) == 0;
} // string_case_compare

} // namespace ftest
} // namespace utils
} // namespace flecsi

#define FTEST()                                                                \
  flecsi::utils::flog::flog_t::instance().config_stream().add_buffer(          \
    "flog", std::clog, true);                                                  \
  flecsi::utils::ftest::state_t __ftest_state_instance(__FUNCTION__)

#define FTEST_RESULT() __ftest_state_instance.result()

#define FTEST_TYPE(name) flecsi::utils::demangle((name))

#define FTEST_TTYPE(type) flecsi::utils::demangle(typeid(type).name())

#define ASSERT_TRUE(condition)                                                 \
  if(condition)                                                                \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(                             \
      #condition, __FILE__, __LINE__, __ftest_state_instance)

#define EXPECT_TRUE(condition)                                                 \
  if(condition)                                                                \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(                                    \
      #condition, __FILE__, __LINE__, __ftest_state_instance)

#define ASSERT_FALSE(condition)                                                \
  if(!(condition))                                                             \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(                             \
      #condition, __FILE__, __LINE__, __ftest_state_instance)

#define EXPECT_FALSE(condition)                                                \
  if(!(condition))                                                             \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(                                    \
      #condition, __FILE__, __LINE__, __ftest_state_instance)

#define ASSERT_EQ(val1, val2)                                                  \
  ASSERT_TRUE(flecsi::utils::ftest::test_equal((val1), (val2)))

#define EXPECT_EQ(val1, val2)                                                  \
  EXPECT_TRUE(flecsi::utils::ftest::test_equal((val1), (val2)))

#define ASSERT_NE(val1, val2)                                                  \
  ASSERT_TRUE(!flecsi::utils::ftest::test_equal((val1), (val2)))

#define EXPECT_NE(val1, val2)                                                  \
  EXPECT_TRUE(!flecsi::utils::ftest::test_equal((val1), (val2)))

#define ASSERT_LT(val1, val2)                                                  \
  ASSERT_TRUE(flecsi::utils::ftest::test_less((val1), (val2)))

#define EXPECT_LT(val1, val2)                                                  \
  EXPECT_TRUE(flecsi::utils::ftest::test_less((val1), (val2)))

#define ASSERT_LE(val1, val2)                                                  \
  ASSERT_TRUE(flecsi::utils::ftest::test_greater((val2), (val1)))

#define EXPECT_LE(val1, val2)                                                  \
  EXPECT_TRUE(flecsi::utils::ftest::test_greater((val2), (val1)))

#define ASSERT_GT(val1, val2)                                                  \
  ASSERT_TRUE(flecsi::utils::ftest::test_greater((val1), (val2)))

#define EXPECT_GT(val1, val2)                                                  \
  EXPECT_TRUE(flecsi::utils::ftest::test_greater((val1), (val2)))

#define ASSERT_GE(val1, val2)                                                  \
  ASSERT_TRUE(flecsi::utils::ftest::test_less((val2), (val1)))

#define EXPECT_GE(val1, val2)                                                  \
  EXPECT_TRUE(flecsi::utils::ftest::test_less((val2), (val1)))

#define ASSERT_STREQ(str1, str2)                                               \
  if(flecsi::utils::ftest::string_compare(str1, str2))                         \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(                             \
      str1 " == " str2, __FILE__, __LINE__, __ftest_state_instance)

#define EXPECT_STREQ(str1, str2)                                               \
  if(flecsi::utils::ftest::string_compare(str1, str2))                         \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(                                    \
      str1 " == " str2, __FILE__, __LINE__, __ftest_state_instance)

#define ASSERT_STRNE(str1, str2)                                               \
  if(!flecsi::utils::ftest::string_compare(str1, str2))                        \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(                             \
      str1 " != " str2, __FILE__, __LINE__, __ftest_state_instance)

#define EXPECT_STRNE(str1, str2)                                               \
  if(!flecsi::utils::ftest::string_compare(str1, str2))                        \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(                                    \
      str1 " != " str2, __FILE__, __LINE__, __ftest_state_instance)

#define ASSERT_STRCASEEQ(str1, str2)                                           \
  if(flecsi::utils::ftest::string_case_compare(str1, str2))                    \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(str1 " == " str2             \
                                                       " (case insensitive)",  \
      __FILE__,                                                                \
      __LINE__,                                                                \
      __ftest_state_instance)

#define EXPECT_STRCASEEQ(str1, str2)                                           \
  if(flecsi::utils::ftest::string_case_compare(str1, str2))                    \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(str1 " == " str2                    \
                                                " (case insensitive)",         \
      __FILE__,                                                                \
      __LINE__,                                                                \
      __ftest_state_instance)

#define ASSERT_STRCASENE(str1, str2)                                           \
  if(!flecsi::utils::ftest::string_case_compare(str1, str2))                   \
    ;                                                                          \
  else                                                                         \
    return flecsi::utils::ftest::assert_handler_t(str1 " == " str2             \
                                                       " (case insensitive)",  \
      __FILE__,                                                                \
      __LINE__,                                                                \
      __ftest_state_instance)

#define EXPECT_STRCASENE(str1, str2)                                           \
  if(!flecsi::utils::ftest::string_case_compare(str1, str2))                   \
    ;                                                                          \
  else                                                                         \
    flecsi::utils::ftest::expect_handler_t(str1 " == " str2                    \
                                                " (case insensitive)",         \
      __FILE__,                                                                \
      __LINE__,                                                                \
      __ftest_state_instance)

// Provide access to the output stream to allow user to capture output
#define FTEST_CAPTURE()                                                        \
  flecsi::utils::ftest::test_output_t::instance().get_stream()

// Return captured output as a std::string
#define FTEST_DUMP()                                                           \
  flecsi::utils::ftest::test_output_t::instance().get_buffer()

// Compare captured output to a blessed file
#define FTEST_EQUAL_BLESSED(f)                                                 \
  flecsi::utils::ftest::test_output_t::instance().equal_blessed((f))

// Write captured output to file
#define FTEST_WRITE(f)                                                         \
  flecsi::utils::ftest::test_output_t::instance().to_file((f))

// Dump captured output on failure
#if !defined(_MSC_VER)
#define FTEST_ASSERT(ASSERTION, ...)                                           \
  ASSERT_##ASSERTION(__VA_ARGS__) << FTEST_DUMP()
#else
  // MSVC has a brain-dead preprocessor...
#define FTEST_ASSERT(ASSERTION, x, y) ASSERT_##ASSERTION(x, y) << FTEST_DUMP()
#endif

// Dump captured output on failure
#if !defined(_MSC_VER)
#define FTEST_EXPECT(EXPECTATION, ...)                                         \
  EXPECT_##EXPECTATION(__VA_ARGS__) << FTEST_DUMP()
#else
  // MSVC has a brain-dead preprocessor...
#define FTEST_EXPECT(EXPECTATION, x, y)                                        \
  EXPECT_##EXPECTATION(x, y) << FTEST_DUMP()
#endif

// compare collections with varying levels of assertions
#define FTEST_CHECK_EQUAL_COLLECTIONS(...)                                     \
  flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__)

#define FTEST_ASSERT_EQUAL_COLLECTIONS(...)                                    \
  ASSERT_TRUE(flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__) << FTEST_DUMP()

#define FTEST_EXPECT_EQUAL_COLLECTIONS(...)                                    \
  EXPECT_TRUE(flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__))        \
    << FTEST_DUMP()