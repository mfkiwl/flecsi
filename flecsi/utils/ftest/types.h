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

#include "output.h"

#include <iostream>
#include <string>
#include <sstream>

#include <flecsi/utils/flog.h>

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
      stream << FLOG_OUTPUT_LTRED("TEST FAILED " << name_) <<
        FLOG_COLOR_PLAIN << std::endl;
      stream << error_stream_.str();
      flog(utility) << stream.str();
      std::exit(1);
    }
    else {
      flog(utility) << FLOG_OUTPUT_LTGREEN("TEST PASSED " << name_) <<
        FLOG_COLOR_PLAIN << std::endl;
    } // if
  } // process

  const std::string & name() const {
    return name_;
  }

  std::stringstream & stringstream() {
    return error_stream_;
  } // stream

private:

  std::string name_;
  std::stringstream error_stream_;

}; // struct state_t

struct fatal_handler_t {

  fatal_handler_t(
    const char * condition,
    const char * file,
    int line,
    state_t & runtime
  )
    :
      runtime_(runtime)
  {
    runtime_.stringstream() << FLOG_OUTPUT_LTRED("ASSERT FAILED") <<
      ": assertion '" << condition << "' failed in " <<
      FLOG_OUTPUT_BROWN(file << ":" << line) << FLOG_COLOR_BROWN << " "; 
  } // fatal_handler_t

  ~fatal_handler_t() {
    flog_tag_guard(ftest);
    runtime_.stringstream() << FLOG_COLOR_PLAIN << std::endl;
    std::stringstream stream;
    stream << FLOG_OUTPUT_LTRED("TEST FAILED " << runtime_.name()) <<
      FLOG_COLOR_PLAIN << std::endl;
    stream << runtime_.stringstream().str();
    flog(utility) << stream.str();
    std::exit(1);
  } // ~fatal_handler_t

  std::ostream & stream() {
    return runtime_.stringstream();
  } // stream

private:

  state_t & runtime_;

}; // fatal_handler_t

struct nonfatal_handler_t {

  nonfatal_handler_t(
    const char * condition,
    const char * file,
    int line,
    state_t & runtime
  )
    :
      runtime_(runtime)
  {
    runtime_.stringstream() <<
      FLOG_OUTPUT_YELLOW("EXPECT FAILED") <<
      ": unexpected '" << condition << "' occurred in " <<
      FLOG_OUTPUT_BROWN(file << ":" << line) << FLOG_COLOR_BROWN << " "; 
  } // nonfatal_handler_t

  ~nonfatal_handler_t() {
    runtime_.stringstream() << FLOG_COLOR_PLAIN << std::endl;
  } // ~nonfatal_handler_t

  std::ostream & stream() {
    return runtime_.stringstream();
  } // stream

private:

  state_t & runtime_;

}; // nonfatal_handler_t

inline bool string_compare(const char * lhs, const char * rhs) {
  if(lhs == nullptr) { return rhs == nullptr; }
  if(rhs == nullptr) { return false; }
  return strcmp(lhs, rhs) == 0;
} // string_compare

inline bool string_case_compare(const char * lhs, const char * rhs) {
  if(lhs == nullptr) { return rhs == nullptr; }
  if(rhs == nullptr) { return false; }
  return strcasecmp(lhs, rhs) == 0;
} // string_case_compare

} // namespace ftest
} // namespace utils
} // namespace flecsi

#define FTEST()                                                                \
  flecsi::utils::flog::flog_t::instance().config_stream().add_buffer(          \
    "flog", std::clog, true);                                                  \
  flecsi::utils::ftest::state_t __ftest_state_instance(__FUNCTION__)

#define EXPECT_TRUE(condition)                                                 \
  (condition) ||                                                               \
  flecsi::utils::ftest::nonfatal_handler_t(#condition, __FILE__, __LINE__,     \
    __ftest_state_instance).stream()

#define EXPECT_FALSE(condition)                                                \
  !(condition) ||                                                              \
  flecsi::utils::ftest::nonfatal_handler_t(#condition, __FILE__, __LINE__,     \
    __ftest_state_instance).stream()

#define ASSERT_TRUE(condition)                                                 \
  (condition) ||                                                               \
  flecsi::utils::ftest::fatal_handler_t(#condition, __FILE__, __LINE__,        \
    __ftest_state_instance).stream()

#define ASSERT_FALSE(condition)                                                \
  !(condition) ||                                                              \
  flecsi::utils::ftest::fatal_handler_t(#condition, __FILE__, __LINE__,        \
    __ftest_state_instance).stream()

#define ASSERT_EQ(val1, val2)                                                  \
  ASSERT_TRUE((val1) == (val2))

#define EXPECT_EQ(val1, val2)                                                  \
  EXPECT_TRUE((val1) == (val2))

#define ASSERT_NE(val1, val2)                                                  \
  ASSERT_TRUE((val1) != (val2))

#define EXPECT_NE(val1, val2)                                                  \
  EXPECT_TRUE((val1) != (val2))

#define ASSERT_LT(val1, val2)                                                  \
  ASSERT_TRUE((val1) < (val2))

#define EXPECT_LT(val1, val2)                                                  \
  EXPECT_TRUE((val1) < (val2))

#define ASSERT_LE(val1, val2)                                                  \
  ASSERT_TRUE((val1) <= (val2))

#define EXPECT_LE(val1, val2)                                                  \
  EXPECT_TRUE((val1) <= (val2))

#define ASSERT_GT(val1, val2)                                                  \
  ASSERT_TRUE((val1) > (val2))

#define EXPECT_GT(val1, val2)                                                  \
  EXPECT_TRUE((val1) > (val2))

#define ASSERT_GE(val1, val2)                                                  \
  ASSERT_TRUE((val1) >= (val2))

#define EXPECT_GE(val1, val2)                                                  \
  EXPECT_TRUE((val1) >= (val2))

#define ASSERT_STREQ(str1, str2)                                               \
  flecsi::utils::ftest::string_compare(str1, str2) ||                          \
  flecsi::utils::ftest::fatal_handler_t(str1 " == " str2, __FILE__, __LINE__).stream()

#define EXPECT_STREQ(str1, str2)                                               \
  flecsi::utils::ftest::string_compare(str1, str2) ||                          \
  flecsi::utils::ftest::nonfatal_handler_t(str1 " == " str2, __FILE__, __LINE__).stream()

#define ASSERT_STRNE(str1, str2)                                               \
  !flecsi::utils::ftest::string_compare(str1, str2) ||                         \
  flecsi::utils::ftest::fatal_handler_t(str1 " != " str2, __FILE__, __LINE__).stream()

#define EXPECT_STRNE(str1, str2)                                               \
  !flecsi::utils::ftest::string_compare(str1, str2) ||                         \
  flecsi::utils::ftest::nonfatal_handler_t(str1 " != " str2, __FILE__, __LINE__).stream()

#define ASSERT_STRCASEEQ(str1, str2)                                           \
  flecsi::utils::ftest::string_case_compare(str1, str2) ||                     \
  flecsi::utils::ftest::fatal_handler_t(str1 " == " str2 " (case insensitive)",               \
    __FILE__, __LINE__).stream()

#define EXPECT_STRCASEEQ(str1, str2)                                           \
  flecsi::utils::ftest::string_case_compare(str1, str2) ||                     \
  flecsi::utils::ftest::nonfatal_handler_t(str1 " == " str2 " (case insensitive)",            \
    __FILE__, __LINE__).stream()

#define ASSERT_STRCASENE(str1, str2)                                           \
  !flecsi::utils::ftest::string_case_compare(str1, str2) ||                    \
  flecsi::utils::ftest::fatal_handler_t(str1 " != " str2 " (case insensitive)",               \
    __FILE__, __LINE__).stream()

#define EXPECT_STRCASENE(str1, str2)                                           \
  !flecsi::utils::ftest::string_case_compare(str1, str2) ||                    \
  flecsi::utils::ftest::nonfatal_handler_t(str1 " != " str2 " (case insensitive)",            \
    __FILE__, __LINE__).stream()

// Provide access to the output stream to allow user to capture output
#define FTEST_CAPTURE() \
  flecsi::utils::ftest::test_output_t::instance().get_stream()

// Return captured output as a std::string
#define FTEST_DUMP() \
  flecsi::utils::ftest::test_output_t::instance().get_buffer()

// Compare captured output to a blessed file
#define FTEST_EQUAL_BLESSED(f) \
  flecsi::utils::ftest::test_output_t::instance().equal_blessed((f))

// Write captured output to file
#define FTEST_WRITE(f) \
  flecsi::utils::ftest::test_output_t::instance().to_file((f))

// Dump captured output on failure
#if !defined(_MSC_VER)
  #define FTEST_ASSERT(ASSERTION, ...) \
  ASSERT_ ## ASSERTION(__VA_ARGS__) << FTEST_DUMP()
#else
  // MSVC has a brain-dead preprocessor...
  #define FTEST_ASSERT(ASSERTION, x, y) \
    ASSERT_ ## ASSERTION(x, y) << FTEST_DUMP()
#endif

// Dump captured output on failure
#if !defined(_MSC_VER)
  #define FTEST_EXPECT(EXPECTATION, ...) \
  EXPECT_ ## EXPECTATION(__VA_ARGS__) << FTEST_DUMP()
#else
  // MSVC has a brain-dead preprocessor...
  #define FTEST_EXPECT(EXPECTATION, x, y) \
    EXPECT_ ## EXPECTATION(x, y) << FTEST_DUMP()
#endif

// compare collections with varying levels of assertions
#define FTEST_CHECK_EQUAL_COLLECTIONS(...) \
  flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__)

#define FTEST_ASSERT_EQUAL_COLLECTIONS(...) \
  ASSERT_TRUE(flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__) << FTEST_DUMP()

#define FTEST_EXPECT_EQUAL_COLLECTIONS(...) \
  EXPECT_TRUE(flecsi::utils::ftest::CheckEqualCollections(__VA_ARGS__) ) << FTEST_DUMP()