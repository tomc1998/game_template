/**
   This file should be included after all others, and will run the unit tests.
 */

#pragma once

namespace tests {
  using test_f = void (*)();

  struct test {
    const char* name;
    test_f fn;
  };

  // Include tests within this namespace
#include "util.hpp"
#include "test_test.hpp"

  void add_all_tests(std::vector<test>& tests, const std::vector<test>& to_append) {
    tests.insert(tests.end(), to_append.begin(), to_append.end());
  }

  int run_all_tests() {
    std::vector<test> tests;
    // Add all the tests
    add_all_tests(tests, test_test_all_tests());

    bool has_failure = 0;

    for (test t : tests) {
      try {
        t.fn();
        std::cerr << "\033[1;32mTEST SUCCESS:\033[1;0m " << t.name << std::endl;
      } catch (const assert_exception& e) {
        std::cerr << "\033[1;31mTEST FAILURE:\033[1;0m " << t.name << " - " << e.msg << std::endl;
        has_failure = 1;
      }
    }

    return has_failure;
  }
};
