#version 330 core

uniform mat4 proj_mat;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;

out vec4 v_col;
out vec2 v_uv;

void main() {
	gl_Position = proj_mat * vec4(pos, 0, 1);
  v_col = col;
  v_uv = uv;
}
