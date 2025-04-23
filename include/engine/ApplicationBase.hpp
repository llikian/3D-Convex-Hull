/***************************************************************************************************
 * @file  ApplicationBase.hpp
 * @brief Declaration of the ApplicationBase class
 **************************************************************************************************/

#pragma once

#include <queue>
#include <unordered_map>

#include "Window.hpp"
#include "maths/vec2.hpp"

/**
 * @class ApplicationBase
 * @brief The core of the engine.
 */
class ApplicationBase {
public:
    /* ---- Constructor & Destructor ---- */

    /**
     * @brief Sets the default value of all member variables and constants.
     * @param name The name of the application.
     */
    ApplicationBase(const std::string& name);

    /* ---- Public Methods ---- */

    /**
     * @brief Contains the main loop.
     */
    virtual void run() = 0;

    /* ---- Callbacks ---- */

    /**
     * @brief Sets the wanted callback functions for a specific GLFWwindow.
     * @tparam App The type of application. Needs to be a child class of ApplicationBase.
     * @param window The GLFW window.
     * @param windowSizeCallback Whether the window size callback need to be set.
     * @param frameBufferSizeCallback Whether the framebuffer size callback need to be set.
     * @param keyCallback Whether the key callback need to be set.
     * @param mouseButtonCallback Whether the mouse button callback need to be set.
     * @param cursorPositionCallback Whether the cursor position callback need to be set.
     * @param scrollCallback Whether the scroll callback need to be set.
     */
    template<typename App>
    static void setCallbacks(GLFWwindow* window,
                      bool windowSizeCallback,
                      bool frameBufferSizeCallback,
                      bool keyCallback,
                      bool mouseButtonCallback,
                      bool cursorPositionCallback,
                      bool scrollCallback);

    /**
     * @brief Handles what happens when the window is resized. Updates the width and height in Window.
     * @param width The new width.
     * @param height The new height.
     */
    virtual void handleWindowSizeCallback(int width, int height);

    /**
     * @brief Handles what happens when the framebuffer is resized. Updates the OpenGL Viewport.
     * @param width The new width.
     * @param height The new height.
     */
    virtual void handleFrameBufferSizeCallback(int width, int height);

    /**
     * @brief Handles what happens when a key is pressed, released or held down. If the pressed key is repeatable and a
     * key is pressed, it is set as active in the repeatable keys map, if it is released we set it as inactive. If the
     * key is not repeatable, we add it to the key event queue.
     * @param key The affected key. This is a platform-independent key code using GLFW_KEY_*.
     * @param scancode The system-specific scancode for the key.
     * @param action The action performed. Can be one of three values:\n
     * - GLFW_PRESS: The key was pressed.\n
     * - GLFW_RELEASE: The key was released.\n
     * - GLFW_REPEAT: The key was held down and is being repeatedly sent.
     * @param mods Bit field representing the modifier keys that were held down when the event occured.
     * It is a bitwise OR combination of the following flags:\n
     * - GLFW_MOD_SHIFT\n
     * - GLFW_MOD_CONTROL\n
     * - GLFW_MOD_ALT\n
     * - GLFW_MOD_SUPER\n
     */
    virtual void handleKeyCallback(int key, int scancode, int action, int mods);

    /**
     * @brief Handles what happens when one of the mouse's button is pressed or released.
     * @param button The mouse button that was pressed or released. Can be one of three values:\n
     * - GLFW_MOUSE_BUTTON_LEFT
     * - GLFW_MOUSE_BUTTON_MIDDLE
     * - GLFW_MOUSE_BUTTON_RIGHT
     * @param action Tha action performed. Can be one of two values:\n
     * - GLFW_PRESS: The button was pressed.\n
     * - GLFW_RELEASE: The button was released.
     * @param mods Bit field representing the modifier keys that were held down when the event occured.
     * It is a bitwise OR combination of the following flags:\n
     * - GLFW_MOD_SHIFT\n
     * - GLFW_MOD_CONTROL\n
     * - GLFW_MOD_ALT\n
     * - GLFW_MOD_SUPER\n
     */
    virtual void handleMouseButtonCallback(int button, int action, int mods);

    /**
     * @brief Handles what happens when the cursor is moved. Updates the mouse position.
     * @param xPos The new horizontal position of the mouse.
     * @param yPos The new vertical position of the mouse.
     */
    virtual void handleCursorPositionCallback(double xPos, double yPos);

    /**
     * @brief Handles what happens when the mouse wheel is scrolled.
     * @param xOffset How much the mouse moved horizontally.
     * @param yOffset How much the mouse moved vertically.
     */
    virtual void handleScrollCallback(double xOffset, double yOffset);

protected:
    /* ---- Protected Methods ---- */

    /**
     * @brief Polls and handles events.
     */
    void handleEvents();

    /**
     * @brief Handles what happens when a specific key is pressed.
     * @param key The key that was pressed.
     */
    virtual void handleKeyEvent(int key) = 0;

    /* ---- Variables & Constants ---- */
    Window window;  ///< The GLFW window.

    std::queue<int> keyEvents; ///< Queue of all key events.
    std::unordered_map<int, bool> repeatableKeys; ///< List of all keys which are repeatable and whether they are active.

    vec2 mousePos; ///< The position of the mouse on the screen.

    float time;  ///< The current time in seconds;
    float delta; ///< The time difference between this frame and the previous in seconds.
};

#include "ApplicationBase.tpp"
