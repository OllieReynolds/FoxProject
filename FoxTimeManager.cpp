#include "FoxTimeManager.hpp"
#include <glfw3.h>

namespace fox {
    namespace utils {
        FoxTimeManager::FoxTimeManager() : lastFrame(0.0f), deltaTime(0.0f) {}

        void FoxTimeManager::update() {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        float FoxTimeManager::getDeltaTime() const {
            return deltaTime;
        }

    }
}
