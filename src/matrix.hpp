#pragma once

/** A 4x4 matrix */
struct Matrix {
  float data[16];

  /** generate an ortho matrix for the given l/r/t/b */
  static Matrix ortho(float l, float r, float t, float b) {
    const float f = 1.0;
    const float n = -1.0;
    Matrix m;
    memset(m.data, 0, 16*sizeof(m.data[0]));
    m.data[0] = 2.0 / (r - l);
    m.data[3] = -(r + l)/(r - l);
    m.data[5] = 2.0 / (t - b);
    m.data[7] = -(t + b)/(t - b);
    m.data[10] = -2.0 / (f - n);
    m.data[11] = -(f + n)/(f - n);
    m.data[15] = 1.0;
    return m;
  }
};

