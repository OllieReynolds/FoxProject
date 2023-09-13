#define GLFW_INCLUDE_NONE

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS

#include "glad.h"
#include <glfw3.h>

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

        sceneSwitcher.update(deltaTime);
        sceneSwitcher.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    sceneSwitcher.destroy();

    glfwTerminate();
    return 0;
}
