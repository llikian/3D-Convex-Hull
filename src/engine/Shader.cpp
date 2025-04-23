/***************************************************************************************************
 * @file  Shader.cpp
 * @brief Implementation of the Shader class
 **************************************************************************************************/

#include "engine/Shader.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string* paths, unsigned int count, const std::string& name = "") :
    id(glCreateProgram()),
    name(name) {

    /* ---- Shader Name ---- */
    if(name.size() == 0) {
        this->name = "shader" + std::to_string(id);
    }

    /* ---- Shaders ---- */
    unsigned int shaderID;
    for(unsigned int i = 0 ; i < count ; ++i) {
        shaderID = compileShader(paths[i]);
        glAttachShader(id, shaderID);
        glDeleteShader(shaderID);
    }

    /* ---- Shader Program ---- */
    glLinkProgram(id);

    int messageLength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetProgramInfoLog(id, messageLength, nullptr, message);

        std::string errorMessage = "Failed to link shader program '" + name + ":\n" + message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }

    getUniforms();
}

Shader::Shader(const unsigned int* shaderIDs, unsigned int count, const std::string& name) :
    id(glCreateProgram()),
    name(name) {

    /* ---- Shader Name ---- */
    if(name.size() == 0) {
        this->name = "shader" + std::to_string(id);
    }

    /* ---- Shaders ---- */
    for(unsigned int i = 0 ; i < count ; ++i) {
        glAttachShader(id, shaderIDs[i]);
    }

    /* ---- Shader Program ---- */
    glLinkProgram(id);

    int messageLength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetProgramInfoLog(id, messageLength, nullptr, message);

        std::string errorMessage = "Failed to link shader program '" + name + ":\n" + message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }

    getUniforms();
}

Shader::~Shader() {
    glDeleteProgram(id);
}

unsigned int Shader::compileShader(const std::string& path) {
    std::string extension = path.substr(path.find_last_of('.') + 1);

    std::string shaderTypeName;
    unsigned int shaderType;
    switch(extension[0]) {
        case 'v':
            shaderTypeName = "vertex";
            shaderType = GL_VERTEX_SHADER;
            break;
        case 'f':
            shaderTypeName = "fragment";
            shaderType = GL_FRAGMENT_SHADER;
            break;
        case 't':
            if(extension[3] == 'c') {
                shaderTypeName = "tesselation control";
                shaderType = GL_TESS_CONTROL_SHADER;
            } else {
                shaderTypeName = "tesselation evaluation";
                shaderType = GL_TESS_EVALUATION_SHADER;
            }
            break;
        case 'c':
            shaderTypeName = "compute";
            shaderType = GL_COMPUTE_SHADER;
            break;
        case 'g':
            shaderTypeName = "geometry";
            shaderType = GL_GEOMETRY_SHADER;
            break;
        default:
            throw std::runtime_error("Unknown shader extension: " + extension);
    }

    std::ifstream file(path);
    if(!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open " + shaderTypeName + " shader."));
    }

    std::string rawCode = (std::stringstream() << file.rdbuf()).str();
    const char* code = rawCode.c_str();
    unsigned int shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    int messageLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &messageLength);
    if(messageLength > 0) {
        char* message = new char[messageLength];
        glGetShaderInfoLog(shaderID, messageLength, nullptr, message);

        std::string errorMessage = "Failed to compile " + shaderTypeName + " shader '" + path + "':\n" + message;

        delete[] message;

        throw std::runtime_error(errorMessage);
    }

    return shaderID;
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::getUniforms() {
    use();

    int MAX_CHAR;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &MAX_CHAR);

    int length;
    int size;
    unsigned int type;
    char* uniformName = new char[MAX_CHAR];

    int count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

    for(int i = 0u ; i < count ; ++i) {
        glGetActiveUniform(id, i, MAX_CHAR, &length, &size, &type, uniformName);

        if(size == 1) {
            uniforms.emplace(uniformName, glGetUniformLocation(id, uniformName));
        } else {
            std::string nameIndex;
            for(int j = 0 ; j < size ; ++j) {
                uniformName[length - 2] = '\0';
                nameIndex = uniformName;
                nameIndex += std::to_string(j) + ']';
                uniforms.emplace(nameIndex, glGetUniformLocation(id, nameIndex.c_str()));
            }
        }
    }

    delete[] uniformName;
}

void Shader::setUniform(int location, int value) {
    glUniform1i(location, value);
}

void Shader::setUniform(int location, int x, int y) {
    glUniform2i(location, x, y);
}

void Shader::setUniform(int location, int x, int y, int z) {
    glUniform3i(location, x, y, z);
}

void Shader::setUniform(int location, int x, int y, int z, int w) {
    glUniform4i(location, x, y, z, w);
}

void Shader::setUniform(int location, unsigned int value) {
    glUniform1ui(location, value);
}

void Shader::setUniform(int location, bool value) {
    glUniform1i(location, static_cast<int>(value));
}

void Shader::setUniform(int location, float value) {
    glUniform1f(location, value);
}

void Shader::setUniform(int location, float x, float y) {
    glUniform2f(location, x, y);
}

void Shader::setUniform(int location, float x, float y, float z) {
    glUniform3f(location, x, y, z);
}

void Shader::setUniform(int location, float x, float y, float z, float w) {
    glUniform4f(location, x, y, z, w);
}

void Shader::setUniform(int location, const vec2& vec) {
    glUniform2fv(location, 1, &vec.x);
}

void Shader::setUniform(int location, const vec3& vec) {
    glUniform3fv(location, 1, &vec.x);
}

void Shader::setUniform(int location, const vec4& vec) {
    glUniform4fv(location, 1, &vec.x);
}

void Shader::setUniform(int location, const mat4& matrix) {
    glUniformMatrix4fv(location, 1, false, &(matrix(0, 0)));
}
