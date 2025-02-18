#include "shader.h"

const char* VERTEX_SHADER_SOURCE = {
#include "Vertex.glsl"
};

const char* FRAGMENT_SHADER_SOURCE = {
#include "Fragment.glsl"
};

void Shader::setup(const char* vertexPath, const char* fragmentPath) {
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
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << "\n";
		ready = false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
		ready = false;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
		ready = false;
	}
	// Creating the shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// Print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
		ready = false;
	}
	// Delete the shader programs
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

bool Shader::isReady() {
	return ready;
}
