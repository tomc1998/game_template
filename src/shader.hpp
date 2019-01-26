#pragma once

#include <fstream>

struct shader_src {
  std::vector<std::string> lines;

  shader_src(const shader_src&) = delete;
  shader_src(const char* filename) {
    std::ifstream in(filename);
    if (!in.good()) {
      std::cerr << "Error: Cannot open file " << filename << std::endl;
      std::exit(1);
    }
    std::string line;
    while(std::getline(in, line)) {
      lines.push_back(line + "\n");
    }
  }
};

const char* shader_type_as_name(GLint shader_type) {
  if (shader_type == GL_VERTEX_SHADER) {
    return "VERTEX_SHADER";
  } else if (shader_type == GL_FRAGMENT_SHADER) {
    return "FRAGMENT_SHADER";
  } else {
    assert(false && "Unknown shader type");
  }
}

GLuint compile_shader(GLint shader_type, const shader_src& src) {
  GLuint shader = glCreateShader(shader_type);
  std::vector<const char*> lines(src.lines.size(), nullptr);
  std::vector<int> line_lengths(src.lines.size(), 0);
  std::transform(src.lines.begin(), src.lines.end(), lines.begin(),
                 [](const auto &s) { return s.c_str(); });
  std::transform(src.lines.begin(), src.lines.end(), line_lengths.begin(),
                 [](const auto &s) { return s.size(); });
  glShaderSource(shader, src.lines.size(), lines.data(), line_lengths.data());
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    int log_size;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    char* log = (char*)std::malloc(log_size);
    glGetShaderInfoLog(shader, log_size, NULL, log);
    std::cerr << "Error: Shader compilation error for shader " <<
      shader_type_as_name(shader_type) << ":\n" << log << std::endl;
    std::exit(1);
  }
  return shader;
}

class shader {
  GLuint program;
public:
  shader(const shader&) = delete;

  shader(const char* vert_src_path, const char* frag_src_path,
         std::vector<std::pair<const char*, GLint>> attrib_bindings) {
    shader_src vert_src(vert_src_path);
    shader_src frag_src(frag_src_path);
    const auto vert = compile_shader(GL_VERTEX_SHADER, vert_src);
    const auto frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    for (const auto& binding : attrib_bindings) {
      glBindAttribLocation(program, binding.second, binding.first);
    }
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      int log_size;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
      char* log = (char*)std::malloc(log_size);
      glGetProgramInfoLog(program, log_size, NULL, log);
      std::cerr << "Error: Linker error:\n" << log << std::endl;
      std::exit(1);
    }
  }

  void use() {
    glUseProgram(program);
  }
};
