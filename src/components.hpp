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
  unsigned char col[4];

  cdebug_draw(float x, float y, unsigned char _col[4])
    : vec { x, y } {
    std::memcpy(col, _col, 4);
  };

  cdebug_draw(vec2 vec, unsigned char _col[4])
    : vec { vec } {
    std::memcpy(col, _col, 4);
  };
};
