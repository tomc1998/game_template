#pragma once

#include <iostream>

/** Controls rendering to a GL context */
struct renderer {
  /** A reference to the ECS */
  const entt::DefaultRegistry& registry;

  /* Shaders */

  shader main_dyn_opaque_shader;

  /* Buffers */

  /** Main dynamic content buffers */
  GLuint main_dyn_opaque_vao, main_dyn_opaque_vbo;
  /** CPU backing for vbo */
  std::vector<char> main_dyn_opaque_buffer;

  renderer(const entt::DefaultRegistry& registry)
    : registry(registry),
      main_dyn_opaque_shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl",
                             {std::make_pair("pos", 0),
                                 std::make_pair("uv", 1),
                                 std::make_pair("col", 2)}) {
    // Setup main dyn opaque vao
    main_dyn_opaque_buffer.reserve(1024 * 1024);
    glGenVertexArrays(1, &main_dyn_opaque_vao);
    glGenBuffers(1, &main_dyn_opaque_vbo);
    glBindVertexArray(main_dyn_opaque_vao);
    glBindBuffer(GL_ARRAY_BUFFER, main_dyn_opaque_vbo);
    // vec2 pos
    // vec2 uv
    // vec4(uint) col
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 20, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)8);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 20, (void*)16);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
  }
  renderer(const renderer&) = delete;

  /** Upload all the data to the VBOs */
  void buffer_data() {
    main_dyn_opaque_buffer.clear();
    // Justbuffer some test data
    float data[10 * 3];
    int* col0 = reinterpret_cast<int*>(&data[4]);
    int* col1 = reinterpret_cast<int*>(&data[9]);
    int* col2 = reinterpret_cast<int*>(&data[14]);
    int* col3 = reinterpret_cast<int*>(&data[19]);
    int* col4 = reinterpret_cast<int*>(&data[24]);
    int* col5 = reinterpret_cast<int*>(&data[29]);
    // WARNING: Colors are flipped (ABGR) because endianness
    data[0]  = 0.0; data[1]  = 0.0; data[2]  = 0.0; data[3]  = 0.0;
    *col0 = 0xFFA070A0;
    data[5]  = 1.0; data[6]  = 0.0; data[7]  = 0.0; data[8]  = 0.0;
    *col1 = 0xFFB070A0;
    data[10] = 1.0; data[11] = 1.0; data[12] = 0.0; data[13] = 0.0;
    *col2 = 0xFFC070A0;
    data[15]  = 0.0; data[16]  = 0.0;
    *col3 = 0xFFD070A0;
    data[20]  = 0.5; data[21]  = 0.0;
    *col4 = 0xFFE070A0;
    data[25] = 0.5; data[26] = 1.0;
    *col5 = 0xFFF070A0;
    glBindVertexArray(main_dyn_opaque_vao);
    glBindBuffer(GL_ARRAY_BUFFER, main_dyn_opaque_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
    std::cout << "Err: " << glGetError() << std::endl;;
  }

  /** Assuming a GL context is current, render to that. */
  void render() {
    main_dyn_opaque_shader.use();
    buffer_data();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(main_dyn_opaque_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
};
