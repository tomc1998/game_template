#pragma once

#include <iostream>

/** Controls rendering to a GL context */
struct renderer {
  /** A reference to the ECS */
  entt::DefaultRegistry& registry;

  Matrix proj_mat;
  GLint proj_mat_loc;

  /* Shaders */

  shader main_dyn_opaque_shader;

  /* Buffers */

  /** Main dynamic content buffers */
  GLuint main_dyn_opaque_vao, main_dyn_opaque_vbo;
  /** CPU backing for vbo */
  std::vector<char> main_dyn_opaque_buffer;

  renderer(entt::DefaultRegistry& registry)
    : registry(registry),
      main_dyn_opaque_shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl",
                             {std::make_pair("pos", 0),
                                 std::make_pair("uv", 1),
                                 std::make_pair("col", 2)}) {
    // Bind uniforms
    main_dyn_opaque_shader.use();
    proj_mat_loc = main_dyn_opaque_shader.get_uniform_loc("proj_mat");
    proj_mat = Matrix::ortho(0, 800, 0, 600);

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
    buffer_data();
    upload_data<false>();
  }
  renderer(const renderer&) = delete;

  /** Upload the data to the GPU */
  template <bool bufferSubData>
  void upload_data() {
    glBindVertexArray(main_dyn_opaque_vao);
    glBindBuffer(GL_ARRAY_BUFFER, main_dyn_opaque_vbo);
    if (bufferSubData) {
      glBufferSubData(GL_ARRAY_BUFFER, 0, main_dyn_opaque_buffer.size()
                      * sizeof(main_dyn_opaque_buffer[0]),
                      main_dyn_opaque_buffer.data());
    } else {
      glBufferData(GL_ARRAY_BUFFER, main_dyn_opaque_buffer.size()
                   * sizeof(main_dyn_opaque_buffer[0]),
                   main_dyn_opaque_buffer.data(), GL_DYNAMIC_DRAW);
    }
  }

  /** Create a buffer of all the data */
  void buffer_data() {
    main_dyn_opaque_buffer.clear();
    registry.view<cpos, cdebug_draw>()
      .each([this](auto entity, auto &pos, auto &debug_draw) {
          // Build the data on the stack then copy over
          union {
            float fdata;
            unsigned idata;
          } data[10 * 3];
          // WARNING: Colors are flipped (ABGR) because endianness
          // UV values are skipped since they're currently unused
          data[0].fdata  = pos.x - debug_draw.w; data[1].fdata  = pos.y - debug_draw.h; // TRI 0
          data[4].idata  = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];
          data[5].fdata  = pos.x - debug_draw.w; data[6].fdata  = pos.y + debug_draw.h;
          data[9].idata  = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];
          data[10].fdata = pos.x + debug_draw.w; data[11].fdata = pos.y + debug_draw.h;
          data[14].idata = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];
          data[15].fdata = pos.x - debug_draw.w; data[16].fdata = pos.y - debug_draw.h; // TRI 1
          data[19].idata = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];
          data[20].fdata = pos.x + debug_draw.w; data[21].fdata = pos.y - debug_draw.h;
          data[24].idata = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];
          data[25].fdata = pos.x + debug_draw.w; data[26].fdata = pos.y + debug_draw.h;
          data[29].idata = (debug_draw.col[3] << 24) | (debug_draw.col[2] << 16) | (debug_draw.col[1] << 8) | debug_draw.col[0];

          main_dyn_opaque_buffer.insert(main_dyn_opaque_buffer.end(),
                                        (char*)(&data[0]), (char*)(&data[30]));
        });
  }

  /** Assuming a GL context is current, render to that. */
  void render() {
    main_dyn_opaque_shader.use();
    buffer_data();
    upload_data<true>();
    // Upload uniforms
    glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, proj_mat.data);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(main_dyn_opaque_vao);
    glDrawArrays(GL_TRIANGLES, 0, main_dyn_opaque_buffer.size() / 20);
  }
};
