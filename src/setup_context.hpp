#pragma once

/** Setup a GL context */
GLFWwindow* setup_context() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* w = glfwCreateWindow(800, 600, "TestGLGame", nullptr, nullptr);
  glfwMakeContextCurrent(w);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  return w;
}
