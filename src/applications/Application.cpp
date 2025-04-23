/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "applications/Application.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/transforms.hpp"
#include "maths/trigonometry.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/meshes.hpp"

Application::Application()
    : ApplicationBase("OpenGL Engine"),
      wireframe(false), cullface(true), cursorVisible(false),
      areAxesDrawn(false), isGridDrawn(false), isGroundDrawn(true),
      hasGlobalLighting(false),
      shader(nullptr),
      projection(perspective(M_PI_4f, window.getRatio(), 0.1f, 100.0f)),
      camera(Point(0.0f, 2.0f, 5.0f)) {

    /* ---- Repeatable Keys ---- */
    repeatableKeys.emplace(GLFW_KEY_W, false);
    repeatableKeys.emplace(GLFW_KEY_S, false);
    repeatableKeys.emplace(GLFW_KEY_A, false);
    repeatableKeys.emplace(GLFW_KEY_D, false);
    repeatableKeys.emplace(GLFW_KEY_SPACE, false);
    repeatableKeys.emplace(GLFW_KEY_LEFT_SHIFT, false);

    /* ---- GLFW Callbacks ---- */
    setCallbacks<Application>(window, true, true, true, false, true, false);

    /* ---- Lights ---- */
    // Directional Light
    directionalLight.direction = normalize(vec3(-2.0f, -3.0f, -2.0f));
    directionalLight.ambient = vec3(0.1f);
    directionalLight.diffuse = vec3(1.0f);
    directionalLight.specular = vec3(1.0f);

    // Flashlight
    flashlight.cutOff = std::cos(radians(12.5f));
    flashlight.outerCutOff = std::cos(radians(15.0f));

    flashlight.constant = 1.0f;
    flashlight.linear = 0.09f;
    flashlight.quadratic = 0.032f;

    flashlight.ambient = vec3(0.0f);
    flashlight.diffuse = vec3(1.0f);
    flashlight.specular = vec3(1.0f);

    // Point Lights
    vec3 pointLightPositions[]{
        vec3(5.0f, 1.0f, 5.0f),
        vec3(5.0f, 1.0f, -5.0f),
        vec3(-5.0f, 1.0f, 5.0f),
        vec3(-5.0f, 1.0f, -5.0f)
    };

    vec3 pointLightColors[]{
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(1.0f, 0.0f, 1.0f)
    };

    pointLights.resize(4);
    for(unsigned int i = 0 ; i < pointLights.size() ; ++i) {
        pointLights[i].position = pointLightPositions[i];

        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09f;
        pointLights[i].quadratic = 0.032f;

        pointLights[i].ambient = vec3(0.25f);
        pointLights[i].diffuse = pointLightColors[i];
        pointLights[i].specular = vec3(1.0f);
    }

    /* ---- Shaders ---- */
    std::string paths[2]{"shaders/application/default.vert", "shaders/application/default.frag"};
    shader = new Shader(paths, 2, "Default");
    initUniforms();
}

Application::~Application() {
    delete shader;
}

void Application::run() {
    Mesh grid = Meshes::grid(10.0f, 10);
    Mesh axes = Meshes::axes(1.0f);
    Mesh cube = Meshes::cube();
    Mesh tcube = Meshes::texturedCube();
    Mesh wcube = Meshes::wireframeCube();
    Mesh pcube = Meshes::plainCube();
    Mesh sphere = Meshes::sphere(16, 32);
    Mesh tsphere = Meshes::texturedSphere(16, 32);
    Mesh plane = Meshes::nplane(200.0f);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Texture texContainer("data/textures/container.jpg");
    Texture texCube("data/textures/cube.png");
    Texture texGrass("data/textures/grass_block.png");
    Texture texDirt("data/textures/dirt.png");
    Texture texStone("data/textures/stone.png");
    Texture texVenus("data/textures/venus.jpg");

    Texture pointLightTextures[4]{
        Texture(pointLights[0].diffuse),
        Texture(pointLights[1].diffuse),
        Texture(pointLights[2].diffuse),
        Texture(pointLights[3].diffuse)
    };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    Texture texGround("data/textures/ground.png");

    /* ---- Main Loop ---- */
    while(!glfwWindowShouldClose(window)) {
        handleEvents();

        glClearColor(0.1, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        lightPos = 10.0f * vec3(cosf(time), sinf(time), cosf(time));

        shader->use();
        updateUniforms();

        calculateMVP(mat4(1.0f));

        if(isGroundDrawn) {
            bindTexture(texGround);
            plane.draw();
        }

        bindTexture(0);

        if(isGridDrawn) { grid.draw(); }

        if(areAxesDrawn) {
            calculateMVP(translate(camera.getPosition() + 2.0f * camera.getDirection()) * scale(0.5f));
            axes.draw();
        }

        bindTexture(texVenus);
        calculateMVP(mat4(1.0f));
        tsphere.draw();

        calculateMVP(translate(0.0f, 1.75f, 0.0f).scale(0.75f));
        sphere.draw();

        bindTexture(texContainer);
        calculateMVP(translate(3.0f, 0.5f, 0.0f));
        cube.draw();

        bindTexture(texStone);
        calculateMVP(translate(-3.0f, 0.5f, 0.0f));
        cube.draw();

        bindTexture(texDirt);
        calculateMVP(translate(-3.0f, 1.5f, 0.0f));
        cube.draw();

        bindTexture(texGrass);
        calculateMVP(translate(-3.0f, 2.5f, 0.0f));
        tcube.draw();

        for(unsigned int i = 0 ; i < pointLights.size() ; ++i) {
            bindTexture(pointLightTextures[i]);
            calculateMVP(translate(pointLights[i].position).scale(0.25f));
            pcube.draw();
        }

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
        case GLFW_KEY_G:
            isGridDrawn = !isGridDrawn;
            break;
        case GLFW_KEY_H:
            isGroundDrawn = !isGroundDrawn;
            break;
        case GLFW_KEY_J:
            hasGlobalLighting = !hasGlobalLighting;
            shader->setUniform("globalLighting", hasGlobalLighting);
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
        case GLFW_KEY_SPACE:
            camera.move(CameraControls::upward, delta);
            break;
        case GLFW_KEY_LEFT_SHIFT:
            camera.move(CameraControls::downward, delta);
            break;
    }
}

void Application::initUniforms() {
    shader->use();

    shader->setUniform("material.diffuse", 0);
    shader->setUniform("material.specular", 1);
    shader->setUniform("material.shininess", 32.0f);

    shader->setUniform("directionalLight.direction", directionalLight.direction);
    shader->setUniform("directionalLight.ambient", directionalLight.ambient);
    shader->setUniform("directionalLight.diffuse", directionalLight.diffuse);
    shader->setUniform("directionalLight.specular", directionalLight.specular);

    shader->setUniform("spotlight.cutOff", flashlight.cutOff);
    shader->setUniform("spotlight.outerCutOff", flashlight.outerCutOff);
    shader->setUniform("spotlight.constant", flashlight.constant);
    shader->setUniform("spotlight.linear", flashlight.linear);
    shader->setUniform("spotlight.quadratic", flashlight.quadratic);
    shader->setUniform("spotlight.ambient", flashlight.ambient);
    shader->setUniform("spotlight.diffuse", flashlight.diffuse);
    shader->setUniform("spotlight.specular", flashlight.specular);

    std::string nameStart("pointLights["), name;
    for(unsigned int i = 0 ; i < pointLights.size() ; ++i) {
        name = nameStart + std::to_string(i) + "].";

        shader->setUniform(name + "position", pointLights[i].position);
        shader->setUniform(name + "constant", pointLights[i].constant);
        shader->setUniform(name + "linear", pointLights[i].linear);
        shader->setUniform(name + "quadratic", pointLights[i].quadratic);
        shader->setUniform(name + "ambient", pointLights[i].ambient);
        shader->setUniform(name + "diffuse", pointLights[i].diffuse);
        shader->setUniform(name + "specular", pointLights[i].specular);
    }

    calculateMVP(mat4(1.0f));
}

void Application::updateUniforms() {
    shader->setUniform("cameraPos", camera.getPosition());

    shader->setUniform("spotlight.position", camera.getPosition());
    shader->setUniform("spotlight.direction", camera.getDirection());
}

void Application::calculateMVP(const mat4& model) {
    shader->setUniform("mvp", camera.getVPmatrix(projection) * model);
    shader->setUniform("model", model);
}

void Application::bindTexture(const Texture& texture, unsigned int texUnit) {
    texture.bind(texUnit);
}

void Application::bindTexture(unsigned int textureID, unsigned int texUnit) {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Application::bindTexture(const Texture& texture) {
    texture.bind();
}

void Application::bindTexture(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}
