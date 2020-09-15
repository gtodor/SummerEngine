#pragma once

#include <GL/glew.h>
#include <string>


enum ShaderType {
  VertexShader,
  FragmentShader
};

class ShaderProgram {
 private:
  GLuint m_shader_program;
  std::string m_error;
  bool m_hasError;
  
  std::string load_shader(std::string fileName);
  GLuint compile_shader(std::string shaderName, ShaderType shaderType);
  GLuint link_shaders(GLuint* shaders, int size);
  
 public:
  ShaderProgram(std::string vertexShaderName, std::string fragmentShaderName);
  GLuint program();
  bool hasError();
  std::string getError();
};
