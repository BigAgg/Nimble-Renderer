#include "shader.h"
#include "NimbleHelpers.h"
#include <fstream>
#include <iostream>
#include <sstream>

const char *VERTEX_SHADER_SOURCE = {
#include "Vertex.glsl"
};

const char *FRAGMENT_SHADER_SOURCE = {
#include "Fragment.glsl"
};

void Shader::setup(const char *vertexPath, const char *fragmentPath) {
  ready = true;
  // 1. retrieve the vertex / fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  bool loadVertex = true;
  bool loadFragment = true;
  if ((std::string)vertexPath == "") {
    vertexCode = VERTEX_SHADER_SOURCE;
    loadVertex = false;
  }
  if ((std::string)fragmentPath == "") {
    fragmentCode = FRAGMENT_SHADER_SOURCE;
    loadFragment = false;
  }
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    std::stringstream vShaderStream, fShaderStream;
    if (loadVertex) {
      vShaderFile.open(vertexPath);
      vShaderStream << vShaderFile.rdbuf();
      vShaderFile.close();
      vertexCode = vShaderStream.str();
    }
    if (loadFragment) {
      fShaderFile.open(fragmentPath);
      fShaderStream << fShaderFile.rdbuf();
      fShaderFile.close();
      fragmentCode = fShaderStream.str();
    }
  } catch (std::ifstream::failure e) {
    std::string str = e.what();
    logging::log("ERROR", "SHADER::FILE_NOT_SUCCESFULLY_READ\n" + str + "\n");
    ready = false;
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  // 2. Compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // Print compile errors if any
  // Creating the shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
  // Delete the shader programs
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

bool Shader::isReady() { return ready; }

void Shader::checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1025];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      logging::log(
          "ERROR",
          "SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog +
              "\n -- --------------------------------------------------- -- "
              "\n");
      ready = false;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      logging::log(
          "ERROR",
          "PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog +
              "\n -- --------------------------------------------------- -- "
              "\n");
      ready = false;
    }
  }
}
