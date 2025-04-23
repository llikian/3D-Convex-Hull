/***************************************************************************************************
 * @file  Shader.hpp
 * @brief Declaration of the Shader class
 **************************************************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "maths/vec2.hpp"
#include "maths/vec3.hpp"
#include "maths/vec4.hpp"
#include "maths/mat4.hpp"

/**
 * @class Shader
 * @brief Compiles, links and creates a shader program that can be then be bound. Can set the value
 * of uniforms in the shader program.
 */
class Shader {
public:

    /**
     * @brief Creates a shader program and compiles then attaches the shaders at the specified paths
     * to it.
     * @param paths The paths to each of the different shaders.
     * @param count The amount of shaders to attach.
     * @param name The name of the shader program.
     */
    Shader(const std::string* paths, unsigned int count, const std::string& name);

    /**
     * @brief Creates a shader program by attaching previously compiled shaders.
     * @param shaderIDs Array containing the IDs of each of the compiled shaders.
     * @param count The amount of shaders to attach.
     * @param name The name of the shader program.
     */
    Shader(const unsigned int* shaderIDs, unsigned int count, const std::string& name);

    /**
     * @brief Deletes the shader program.
     */
    ~Shader();

    /**
     * @brief Compiles a shader and returns its corresponding id.
     * @param path The path to the shader file.
     * @return The shader's corresponding id.
     */
    static unsigned int compileShader(const std::string& path);

    /**
     * @brief Uses the shader program.
     */
    void use();

    /**
     * @brief Finds and adds all the shader's uniforms' id's to the map.
     */
    void getUniforms();

    /**
     * @brief Sets the value of a uniform of any of the available types.
     * @param uniform The uniform's name.
     * @param value The new value of the uniform.
     */
    template<typename... Value>
    void setUniform(const std::string& uniform, Value... value);

private:

    /**
     * @brief Sets the value of a uniform of type int.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void setUniform(int location, int value);

    /**
     * @brief Sets the value of a uniform of type ivec2.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     */
    static void setUniform(int location, int x, int y);

    /**
     * @brief Sets the value of a uniform of type ivec3.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     */
    static void setUniform(int location, int x, int y, int z);

    /**
     * @brief Sets the value of a uniform of type ivec4.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     * @param w The new w value of the uniform.
     */
    static void setUniform(int location, int x, int y, int z, int w);

    /**
     * @brief Sets the value of a uniform of type unsigned int.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void setUniform(int location, unsigned int value);

    /**
     * @brief Sets the value of a uniform of type bool.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void setUniform(int location, bool value);

    /**
     * @brief Sets the value of a uniform of type float.
     * @param location The uniform's location.
     * @param value The new value of the uniform.
     */
    static void setUniform(int location, float value);

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     */
    static void setUniform(int location, float x, float y);

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     */
    static void setUniform(int location, float x, float y, float z);

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param location The uniform's location.
     * @param x The new x value of the uniform.
     * @param y The new y value of the uniform.
     * @param z The new z value of the uniform.
     * @param w The new w value of the uniform.
     */
    static void setUniform(int location, float x, float y, float z, float w);

    /**
     * @brief Sets the value of a uniform of type vec2.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void setUniform(int location, const vec2& vec);

    /**
     * @brief Sets the value of a uniform of type vec3.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void setUniform(int location, const vec3& vec);

    /**
     * @brief Sets the value of a uniform of type vec4.
     * @param location The uniform's location.
     * @param vec The new value of the uniform.
     */
    static void setUniform(int location, const vec4& vec);

    /**
     * @brief Sets the value of a uniform of type mat4.
     * @param location The uniform's location.
     * @param matrix The new value of the uniform.
     */
    static void setUniform(int location, const mat4& matrix);

    unsigned int id; ///< The shader program's id.
    std::string name; ///< The shader's name.
    std::unordered_map<std::string, int> uniforms; ///< Stores uniforms id's.
    std::unordered_map<std::string, bool> unknownUniforms; ///< Stores unknown uniforms.
};

#include "Shader.tpp"
