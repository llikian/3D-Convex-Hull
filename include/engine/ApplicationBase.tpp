/***************************************************************************************************
 * @file  ApplicationBase.tpp
 * @brief Implementation of the ApplicationBase class's template methods
 **************************************************************************************************/

template<typename App>
void ApplicationBase::setCallbacks(GLFWwindow* window,
                                   bool windowSizeCallback,
                                   bool frameBufferSizeCallback,
                                   bool keyCallback,
                                   bool mouseButtonCallback,
                                   bool cursorPositionCallback,
                                   bool scrollCallback) {
    if(windowSizeCallback) {
        auto callback = [](GLFWwindow* window, int width, int height) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleWindowSizeCallback(width, height);
        };

        glfwSetWindowSizeCallback(window, callback);
    }

    if(frameBufferSizeCallback) {
        auto callback = [](GLFWwindow* window, int width, int height) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleFrameBufferSizeCallback(width, height);
        };

        glfwSetFramebufferSizeCallback(window, callback);
    }

    if(keyCallback) {
        auto callback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleKeyCallback(key, scancode, action, mods);
        };

        glfwSetKeyCallback(window, callback);
    }

    if(mouseButtonCallback) {
        auto callback = [](GLFWwindow* window, int button, int action, int mods) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleMouseButtonCallback(button, action, mods);
        };

        glfwSetMouseButtonCallback(window, callback);
    }

    if(cursorPositionCallback) {
        auto callback = [](GLFWwindow* window, double xPos, double yPos) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleCursorPositionCallback(xPos, yPos);
        };

        glfwSetCursorPosCallback(window, callback);
    }

    if(scrollCallback) {
        auto callback = [](GLFWwindow* window, double xOffset, double yOffset) {
            static_cast<App*>(glfwGetWindowUserPointer(window))->handleScrollCallback(xOffset, yOffset);
        };

        glfwSetScrollCallback(window, callback);
    }
}