/**
   Example tests
*/

#pragma once

/** An example test. */
void test_test_example() {
  assert_eq(2, 1+1);
  assert_ne(3, 1+1);
  assert_true(1+2 == 2+1);
  assert_false(1+2 == 4);
}

/** Return a list of all the tests to run - this is called from test_runner.hpp */
std::vector<test> test_test_all_tests() {
  return {
    {"test_example", test_test_example}
  };
}
