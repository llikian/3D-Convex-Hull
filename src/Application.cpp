/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "mesh/meshes.hpp"

Application::Application()
    : ApplicationBase("OpenGL Engine"),
      wireframe(false), cullface(true), cursorVisible(false), areAxesDrawn(false),
      shader(nullptr),
      projection(perspective(M_PI_4f, window.getRatio(), 0.1f, 100.0f)),
      camera(Point(0.0f, 2.0f, 5.0f)),
      wireframeCube(Meshes::wireframeCube()), boundingCubeSize(20.0f),
      pointsAmount(100), hull(pointsAmount, -boundingCubeSize / 2.0f, boundingCubeSize / 2.0f) {
    /* ---- Repeatable Keys ---- */
    repeatableKeys.emplace(GLFW_KEY_W, false);
    repeatableKeys.emplace(GLFW_KEY_S, false);
    repeatableKeys.emplace(GLFW_KEY_A, false);
    repeatableKeys.emplace(GLFW_KEY_D, false);
    repeatableKeys.emplace(GLFW_KEY_SPACE, false);
    repeatableKeys.emplace(GLFW_KEY_LEFT_SHIFT, false);

    /* ---- GLFW Callbacks ---- */
    setCallbacks<Application>(window, true, true, true, false, true, false);

    /* ---- Shaders ---- */
    std::string paths[2]{ "shaders/application/default.vert", "shaders/application/default.frag" };
    shader = new Shader(paths, 2, "Default");
    initUniforms();
}

Application::~Application() {
    delete shader;
}

void Application::run() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPointSize(5.0f);

    /* ---- Main Loop ---- */
    while(!glfwWindowShouldClose(window)) {
        handleEvents();

        glClearColor(0.1, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        updateUniforms();

        calculateMVP(mat4(1.0f));

        hull.draw(shader);

        calculateMVP(scale(boundingCubeSize));
        shader->setUniform("useUniformColor", true);
        shader->setUniform("uColor", vec3(0.0f, 1.0f, 0.788f));
        wireframeCube.draw();
        shader->setUniform("useUniformColor", false);

        glfwSwapBuffers(window);
    }
}

void Application::handleWindowSizeCallback(int width, int height) {
    ApplicationBase::handleWindowSizeCallback(width, height);

    projection(0, 0) = 1.0f / (tanf(M_PI_4f / 2.0f) * window.getRatio());
}

void Application::handleCursorPositionCallback(double xPos, double yPos) {
    if(!cursorVisible) {
        camera.look(vec2(xPos - mousePos.x, yPos - mousePos.y));
    }

    ApplicationBase::handleCursorPositionCallback(xPos, yPos);
}

void Application::handleKeyEvent(int key) {
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_Z:
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_FILL : GL_LINE);
            wireframe = !wireframe;
            break;
        case GLFW_KEY_C:
            (cullface ? glDisable : glEnable)(GL_CULL_FACE);
            cullface = !cullface;
            break;
        case GLFW_KEY_TAB:
            glfwSetInputMode(window, GLFW_CURSOR, cursorVisible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            cursorVisible = !cursorVisible;
            break;
        case GLFW_KEY_Q:
            areAxesDrawn = !areAxesDrawn;
            break;
        case GLFW_KEY_W:
            camera.move(CameraControls::forward, delta);
            break;
        case GLFW_KEY_S:
            camera.move(CameraControls::backward, delta);
            break;
        case GLFW_KEY_A:
            camera.move(CameraControls::left, delta);
            break;
        case GLFW_KEY_D:
            camera.move(CameraControls::right, delta);
            break;
        case GLFW_KEY_R:
            hull.create(pointsAmount, -boundingCubeSize / 2.0f, boundingCubeSize / 2.0f);
            break;
        case GLFW_KEY_SPACE:
            camera.move(CameraControls::upward, delta);
            break;
        case GLFW_KEY_LEFT_SHIFT:
            camera.move(CameraControls::downward, delta);
            break;
        default:
            break;
    }
}

void Application::initUniforms() const {
    shader->use();
    calculateMVP(mat4(1.0f));
}

void Application::updateUniforms() const { }

void Application::calculateMVP(const mat4& model) const {
    shader->setUniform("mvp", camera.getVPmatrix(projection) * model);
    shader->setUniform("model", model);
}
