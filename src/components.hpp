#pragma once
struct cpos {
  vec2 vec;
};

struct cvel {
  vec2 vec;
};

struct cdebug_draw {
  /** Half size, origin at pos */
  vec2 vec;
  /** R G B A */
  color col;

  cdebug_draw(float x, float y, unsigned char col[4])
    : vec { x, y }, col{col[0], col[1], col[2], col[3]} {};

  cdebug_draw(const vec2 &vec, unsigned char col[4])
    : vec { vec }, col{col[0], col[1], col[2], col[3]} {};
};
