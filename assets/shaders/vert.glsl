#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;

out vec4 v_col;

void main() {
	gl_Position = vec4(pos, 0, 1);
  v_col = col;
}
