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
#include "FoxRocketTrackNames.hpp"

int main() {
    // Initialize Rocket Sync
    fox::utils::FoxRocketSync globalRocketSync(fox::utils::RocketTracks::GLOBAL_LIGHT_INTENSITY.c_str());

    // Setup GLFW and window
    fox::gl::FoxGLFWSetup glfwSetup;
    GLFWwindow* window = glfwSetup.getWindow();

    // Setup OpenGL renderer
    fox::gl::FoxRendererSetup rendererSetup;

    // Initialize Scene Switcher
    fox::utils::FoxSceneSwitcher sceneSwitcher(window);
    sceneSwitcher.setGlobalRocketSync(globalRocketSync);
    sceneSwitcher.switchTo("FoxScene");

    // Initialize Time Manager
    fox::utils::FoxTimeManager timeManager;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        globalRocketSync.update(0.0);

        timeManager.update();
        float deltaTime = timeManager.getDeltaTime();

        sceneSwitcher.update(deltaTime);
        sceneSwitcher.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    sceneSwitcher.destroy();

    glfwTerminate();
    return 0;
}
