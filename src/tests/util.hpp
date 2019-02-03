/**
   A collection of utilities like 'assert' for tests.
 */

#pragma once

struct assert_exception {
  std::string msg;
};

/** Throws an exception on failure and exits - this will be caught by the test runner for logging. */
void assert_true__(bool v, const char* expr) {
  if (!v) {
    throw assert_exception { std::string("Expected '") + expr + "' to be true." };
  }
}

/** Throws an exception on failure and exits - this will be caught by the test runner for logging. */
void assert_false__(bool v, const char* expr) {
  if (v) {
    throw assert_exception { std::string("Expected '") + expr + "' to be false." };
  }
}

/** Throws an exception on failure and exits - this will be caught by the test runner for logging. */
template <typename T>
void assert_eq__(T v0, T v1, const char* expr0, const char* expr1) {
  if (v0 != v1) {
    throw assert_exception { std::string("Expected '") + expr0 + "' to be equal to '" + expr1 + "'." };
  }
}

/** Throws an exception on failure and exits - this will be caught by the test runner for logging. */
template <typename T>
void assert_ne__(T v0, T v1, const char* expr0, const char* expr1) {
  if (v0 == v1) {
    throw assert_exception { std::string("Expected '") + expr0 + "' to not be equal to '" + expr1 + "'." };
  }
}

/* Overloads for nullptr_t */
template <typename T>
void assert_eq__(T v0, std::nullptr_t v1, const char* expr0, const char* expr1) {
  assert_eq__(v0, (T)v1, expr0, expr1);
}

/* Overloads for nullptr_t */
template <typename T>
void assert_ne__(T v0, std::nullptr_t v1, const char* expr0, const char* expr1) {
  assert_ne__(v0, (T)v1, expr0, expr1);
}

#define assert_true(e) assert_true__(e, #e)
#define assert_false(e) assert_false__(e, #e)
#define assert_eq(e0, e1) assert_eq__(e0, e1, #e0, #e1)
#define assert_ne(e0, e1) assert_ne__(e0, e1, #e0, #e1)
