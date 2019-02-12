#pragma once

#include <iostream>

#include "texture_atlas.hpp"

/** Controls rendering to a GL context */
struct renderer {
  /** A reference to the ECS */
  entt::DefaultRegistry& registry;

  Matrix proj_mat;
  GLint proj_mat_loc;
  texture_atlas atlas;
  GLint tex_loc;

  /* Shaders */

  shader main_dyn_opaque_shader;

  /* Buffers */

  /** Main dynamic content buffers */
  GLuint main_dyn_opaque_vao, main_dyn_opaque_vbo;
  /** CPU backing for vbo */
  std::vector<char> main_dyn_opaque_buffer;

  renderer(entt::DefaultRegistry& registry)
    : registry(registry),
      atlas("assets/img/test.png"),
      main_dyn_opaque_shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl",
                             {std::make_pair("pos", 0),
                                 std::make_pair("uv", 1),
                                 std::make_pair("col", 2)}) {
    // Bind uniforms
    main_dyn_opaque_shader.use();
    proj_mat_loc = main_dyn_opaque_shader.get_uniform_loc("proj_mat");
    tex_loc = main_dyn_opaque_shader.get_uniform_loc("tex");
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

  /** Render a coloured square where the current grid focus is. */
  void render_grid_focus() {
  }

  template <typename OutputIter>
  void fill_rect(float x, float y, float w, float h, const color &c,
                 const texture_region &tex, OutputIter out) {
    // Build the data on the stack then copy over
    union {
      float fdata;
      color idata;
    } data[10 * 3];
    const auto uv_rect = tex.to_uv_rect();
    data[0].fdata  = x; // TRI 0
    data[1].fdata  = y;
    data[2].fdata  = uv_rect.u; data[3].fdata  = uv_rect.v; data[4].idata  = c;
    data[5].fdata  = x;
    data[6].fdata  = y + h;
    data[7].fdata  = uv_rect.u; data[8].fdata  = uv_rect.t; data[9].idata  = c;
    data[10].fdata = x + w;
    data[11].fdata = y + h;
    data[12].fdata = uv_rect.s; data[13].fdata = uv_rect.t; data[14].idata = c;
    data[15].fdata = x; // TRI 1
    data[16].fdata = y;
    data[17].fdata = uv_rect.u; data[18].fdata = uv_rect.v; data[19].idata = c;
    data[20].fdata = x + w;
    data[21].fdata = y;
    data[22].fdata = uv_rect.s; data[23].fdata = uv_rect.v; data[24].idata = c;
    data[25].fdata = x + w;
    data[26].fdata = y + h;
    data[27].fdata = uv_rect.s; data[28].fdata = uv_rect.t; data[29].idata = c;
    std::copy((char*)(&data[0]), (char*)(&data[30]), out);
  }

  /** Create a buffer of all the data */
  void buffer_data() {
    const auto& white = *atlas.get_region("white");
    main_dyn_opaque_buffer.clear();
    registry.view<cpos, cdebug_draw>()
      .each([this, &white](auto entity, auto &pos, auto &debug_draw) {
          this->fill_rect(pos.vec.x - debug_draw.vec.x, pos.vec.y - debug_draw.vec.y,
                          debug_draw.vec.x * 2, debug_draw.vec.y * 2,
                          debug_draw.col, white, std::back_inserter(main_dyn_opaque_buffer));
        });
  }

  /** Assuming a GL context is current, render to that. */
  void render() {
    main_dyn_opaque_shader.use();
    buffer_data();
    upload_data<true>();
    // Upload uniforms
    glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, proj_mat.data);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.tex);
    glUniform1i(tex_loc, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(main_dyn_opaque_vao);
    glDrawArrays(GL_TRIANGLES, 0, main_dyn_opaque_buffer.size() / 20);
  }
};
