#pragma once
struct cpos {
  float x;
  float y;
};

struct cvel {
  float dx;
  float dy;
};

struct cdebug_draw {
  /** Half width, since pos is the centre */
  float w;
  /** Half height, since pos is the centre */
  float h;
  /** R G B A */
  unsigned char col[4];

  cdebug_draw(float w, float h, unsigned char _col[4])
    : w(w), h(h) {
    std::memcpy(col, _col, 4);
  };
};
