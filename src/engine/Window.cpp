/***************************************************************************************************
 * @file  cpp
 * @brief Implementation of the Window class
 **************************************************************************************************/

#include "engine/Window.hpp"

#include <stdexcept>
#include "stb_image.h"

Window::Window(const std::string& name, void* windowUserPointer) : window(nullptr) {
    /* ---- GLFW ---- */
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1, 1, name.c_str(), nullptr, nullptr);
    if(!window) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, windowUserPointer);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height));

    /* ---- GLAD ---- */
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    /* ---- OpenGL ---- */
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);

    // Sets the default texture to a plain white color
    const unsigned char white[3]{255, 255, 255};
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);

    /* ---- STB ---- */
    stbi_set_flip_vertically_on_load(true);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

Window::operator GLFWwindow*() {
    return window;
}

void Window::updateSize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
}

unsigned int Window::getWidth() const {
    return width;
}

unsigned int Window::getHeight() const {
    return height;
}

vec2 Window::getResolution() const {
    return vec2(static_cast<float>(width), static_cast<float>(height));
}

float Window::getRatio() const {
    return static_cast<float>(width) / static_cast<float>(height);
}
