#pragma once

struct color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;

  float rf() {
    return (float) r / 255.0;
  }
  float gf() {
    return (float) g / 255.0;
  }
  float bf() {
    return (float) b / 255.0;
  }
  float af() {
    return (float) a / 255.0;
  }
};
