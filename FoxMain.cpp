#define GLFW_INCLUDE_NONE

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include "glad.h"
#include <glfw3.h>
#include <iostream>

#include "FoxGLFWSetup.hpp"
#include "FoxRendererSetup.hpp"
#include "FoxSceneSwitcher.hpp"
#include "FoxTimeManager.hpp"

int main() {
    fox::gl::FoxGLFWSetup glfwSetup;
    GLFWwindow* window = glfwSetup.getWindow();

    fox::gl::FoxRendererSetup rendererSetup;

    fox::utils::FoxSceneSwitcher sceneSwitcher(window);
    sceneSwitcher.switchTo("FoxScene");

    fox::utils::FoxTimeManager timeManager;

    while (!glfwWindowShouldClose(window)) {
        timeManager.update();
        float deltaTime = timeManager.getDeltaTime();

        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sceneSwitcher.update(deltaTime);
        sceneSwitcher.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    sceneSwitcher.destroy();

    glfwTerminate();
    return 0;
}
