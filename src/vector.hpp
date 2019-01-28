#pragma once

struct vec2 {
  float x;
  float y;

  static inline vec2 zero() { return { 0, 0 }; }

  inline float len2() const {
    return x * x + y * y;
  }

  inline float len() const {
    return std::sqrt(len2());
  }

  inline vec2 sub(const vec2& other) const {
    return { x - other.x, y - other.y };
  }

  inline vec2 div(const float f) const {
    return { x / f, y / f };
  }

  inline vec2 scl(float s) const {
    return {x * s, y * s};
  }

  inline vec2 nor() const {
    float this_len = len();
    return {x / this_len, y / this_len};
  }
};
