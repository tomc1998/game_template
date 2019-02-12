/** Loads texture atlases */

#pragma once

struct texture_atlas;

struct uv_rect {
  float u, v, s, t;
};

struct texture_region {
  /** Pixel position of the texture */
  int x, y, w, h;
  /** Atlas size */
  int atlas_w, atlas_h;

  texture_region(int x, int y, int w, int h,
                 /** Atlas size */
                 const texture_atlas& atlas);

  uv_rect to_uv_rect() const {
    float u = (float)x / (float)atlas_w;
    float v = (float)y / (float)atlas_h;
    float s = u + (float)w / (float)atlas_w;
    float t = v + (float)h / (float)atlas_h;
    return { u, v, s, t };
  }
};

struct texture_atlas {
  GLuint tex;
  int w, h;

  std::unordered_map<const char*, texture_region> regions;

  /** Load a texture atlas from a file */
  texture_atlas(const char* filename) {
    const auto loaded_tex = load_texture(filename);
    tex = loaded_tex.tex;
    w = loaded_tex.w;
    h = loaded_tex.h;

    // TODO Just hardcode the regions for now - in the future, we'll read these
    // from some metadata file.
    regions.insert(std::make_pair("white", texture_region(0, 0, 1, 1, *this)));
  };

  texture_region* get_region(const char* name) {
    const auto ret = regions.find(name);
    if (ret == regions.end()) {
      return nullptr;
    } else {
      return &ret->second;
    }
  }
};


texture_region::texture_region(int x, int y, int w, int h,
                               /** Atlas size */
                               const texture_atlas& atlas)
: x(x), y(y), w(w), h(h), atlas_w(atlas.w), atlas_h(atlas.h) {}
