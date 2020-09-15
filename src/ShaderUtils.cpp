#include <iostream>
#include <fstream>
#include <cassert>
#include "ShaderUtils.h"

ShaderProgram::ShaderProgram(
    std::string vertexShaderName,
    std::string fragmentShaderName
) : m_shader_program(0),
    m_error(""),
    m_hasError(false)
{
  try{
    GLuint vertexShader = compile_shader(vertexShaderName, VertexShader);
    GLuint fragmentShader = compile_shader(fragmentShaderName, FragmentShader);
    GLuint shaders[] = {vertexShader, fragmentShader};
    m_shader_program = link_shaders(shaders, 2);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  } catch(std::string error) {
    m_error = error;
    m_hasError = true;
  }
}

std::string ShaderProgram::load_shader(std::string fileName)
{
  constexpr auto read_size = std::size_t{4096};
  std::string shaderContent;
  std::string buffer(read_size, '\0');
  std::ifstream ifs(fileName);
  assert(ifs.good());
  while(ifs.read(&buffer[0], read_size)) {
    shaderContent.append(buffer, 0, ifs.gcount());
  }
  shaderContent.append(buffer, 0, ifs.gcount());
  return shaderContent;
}

GLuint ShaderProgram::compile_shader(std::string shaderName, ShaderType shaderType)
{
  GLuint type;
  bool canCompile = true;
  switch (shaderType) {
  case VertexShader: type = GL_VERTEX_SHADER; break;
  case FragmentShader: type = GL_FRAGMENT_SHADER; break;
  default: canCompile = false; break;
  }

  if (canCompile) {
    std::string shaderData = load_shader(shaderName);
    unsigned int shader;
    shader = glCreateShader(type);
    const char* sdata = shaderData.c_str();
    //std::cout << sdata << std::endl;
    glShaderSource(shader, 1, &sdata, NULL);
    glCompileShader(shader);

    //check for errors
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success){
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      throw infoLog;
    }

    return shader;
  } else {
    throw "Shader type must be one of VertexShader or FragmentShader";
  }
}

GLuint ShaderProgram::link_shaders(GLuint* shaders, int size)
{
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  for (int i = 0; i < size; i++) {
    glAttachShader(shaderProgram, shaders[i]);
  }
  glLinkProgram(shaderProgram);

  int  success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw infoLog;
  }

  return shaderProgram;
}

GLuint ShaderProgram::program()
{
  assert(!m_hasError);
  return m_shader_program;
}

bool ShaderProgram::hasError()
{
  return m_hasError;
}

std::string ShaderProgram::getError()
{
  return m_error;
}
  
