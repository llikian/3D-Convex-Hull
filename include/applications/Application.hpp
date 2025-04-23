/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include "engine/ApplicationBase.hpp"

#include "engine/Camera.hpp"
#include "engine/Light.hpp"
#include "engine/Shader.hpp"
#include "engine/Texture.hpp"
#include "maths/mat4.hpp"

/**
 * @class Application
 * @brief An Application used to test functionalities.
 */
class Application : public ApplicationBase {
public:
    /* ---- Constructor & Destructor ---- */

    /**
     * @brief Sets the default value of all member variables and constants.
     */
    Application();

    /**
     * @brief Frees all allocated memory.
     */
    ~Application();

    /* ---- Public Methods ---- */

    /**
     * @brief Contains the main loop.
     */
    void run();

    /* ---- Callbacks ----*/

    /**
     * @brief Handles what happens when the window is resized. Updates the width and height in Window, and
     * updates the projection matrix.
     * @param width The new width.
     * @param height The new height.
     */
    void handleWindowSizeCallback(int width, int height);

    /**
     * @brief Handles what happens when the cursor is moved. Handles the camera's movement and updates the
     * mouse position.
     * @param xPos The horizontal position of the mouse.
     * @param yPos The vertical position of the mouse.
     */
    void handleCursorPositionCallback(double xPos, double yPos);

private:
    /* ---- Private Methods ---- */

    /**
     * @brief Handles what happends when a specific key is pressed.
     * @param key The key.
     */
    void handleKeyEvent(int key);

    /**
     * @brief Initializes all the uniforms to their correct default values.
     */
    void initUniforms();

    /**
     * @brief Updates all the uniforms.
     */
    void updateUniforms();

    /**
     * @brief Calculates the MVP (Matrix-View-Projection) Matrix and sends it to the shader.
     * @param model The new value of the model matrix. It is a product of translation, scale
     * and rotation matrices used to apply transforms on the scene's objects.
     */
    void calculateMVP(const mat4& model);

    /**
     * @brief Binds a texture to a specific texture unit.
     * @param texture The texture to bind.
     * @param texUnit The texture unit to bind to.
     */
    void bindTexture(const Texture& texture, unsigned int texUnit);

    /**
     * @brief Binds a texture class to a specific texture unit.
     * @param textureID The texture's id.
     * @param texUnit The texture unit to bind to.
     */
    void bindTexture(unsigned int textureID, unsigned int texUnit);

    /**
     * @brief Binds a texture to the active texture unit.
     * @param texture The texture to bind.
     */
    void bindTexture(const Texture& texture);

    /**
     * @brief Binds a texture to the active texture unit.
     * @param textureID The texture's id.
     */
    void bindTexture(unsigned int textureID);

    /* ---- Variables & Constants ---- */
    bool wireframe;         ///< Whether to display in wireframe mode.
    bool cullface;          ///< Whether face culling is activated.
    bool cursorVisible;     ///< Whether the cursor is currently visible.
    bool areAxesDrawn;      ///< Whether the axes are drawn.
    bool isGridDrawn;       ///< Whether the grid is drawn.
    bool isGroundDrawn;     ///< Whether the ground is drawn.
    bool hasGlobalLighting; ///< Whether the scene has global lighting.

    Shader* shader; ///< The default shader program.

    mat4 projection; ///< The projection matrix.

    Camera camera; ///< A first person camera to move around the scene.

    DirectionalLight directionalLight;   ///< Light used for global illumination.
    FlashLight flashlight;               ///< A flashlight.
    std::vector<PointLight> pointLights; ///< Point lights used to light up smaller areas.
};