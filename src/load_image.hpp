#pragma once

/** texture handle + metadata */
struct loaded_texture {
  int w, h;
  GLuint tex;
};

GLint libpng_color_type_to_gl(int color_type) {
  if (color_type == PNG_COLOR_TYPE_RGB) {
    return GL_RGB;
  } else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
    return GL_RGBA;
  } else {
    std::cerr << "Unknown color type " << color_type << std::endl;
    std::exit(1);
  }
}

/** Load a texture from a file, upload to GPU, then return the tex handle */
loaded_texture load_texture(const char* filename) {
  unsigned char header[8];
  std::FILE* f = std::fopen(filename, "rb");
  if (!f) {
    std::cerr << "Error reading file " << filename << std::endl;
    std::exit(1);
  }

  assert(fread(header, 1, 8, f));
  if (png_sig_cmp(header, 0,8)) {
    std::cerr << "File "  << filename << " not recognised as PNG" << std::endl;
    std::exit(1);
  }

  const auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  const auto info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, f);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  const auto width = png_get_image_width(png_ptr, info_ptr);
  const auto height = png_get_image_height(png_ptr, info_ptr);
  const auto color_type = png_get_color_type(png_ptr, info_ptr);

  if (color_type == PNG_COLOR_TYPE_PALETTE) {
    std::cerr << "Indexed PNGs not supported" << std::endl;
    std::exit(1);
  } else if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGB_ALPHA) {
    std::cerr << "Invalid color type" << std::endl;
    std::exit(1);
  }

  // Alloc space for image and row pointers
  //const auto num_passes = png_set_interlace_handling(png_ptr);
  char** row_pointers = new char*[height];
  const auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);
  char* data = new char[row_bytes * height];

  for(unsigned y = 0; y < height; ++y) {
    row_pointers[y] = data + y * row_bytes;
  }
  png_read_image(png_ptr, (unsigned char**)row_pointers);
  fclose(f);

  delete[] row_pointers;

  GLuint handle;
  glGenTextures(1, &handle);
  glBindTexture(GL_TEXTURE_2D, handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, libpng_color_type_to_gl(color_type),
               width, height, 0, libpng_color_type_to_gl(color_type),
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  return { (int)width, (int)height, handle };
}
