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
#include "FoxRocketSync.hpp"

int main() {
    fox::utils::FoxRocketSync globalRocketSync("global_tracks.rocket");

    fox::gl::FoxGLFWSetup glfwSetup;
    GLFWwindow* window = glfwSetup.getWindow();

    fox::gl::FoxRendererSetup rendererSetup;

    fox::utils::FoxSceneSwitcher sceneSwitcher(window);
    sceneSwitcher.setGlobalRocketSync(globalRocketSync);

    sceneSwitcher.switchTo("FoxScene");

    fox::utils::FoxTimeManager timeManager;

    while (!glfwWindowShouldClose(window)) {
        globalRocketSync.update(0.0);

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
