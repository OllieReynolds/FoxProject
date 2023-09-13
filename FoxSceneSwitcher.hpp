#pragma once

#include <map>
#include <memory>
#include <string>
#include "FoxBaseScene.hpp"
#include <glfw3.h>
#include "FoxRocketSync.hpp"

namespace fox {
    namespace utils {
        class FoxSceneSwitcher {
        public:
            FoxSceneSwitcher(GLFWwindow* window);
            void switchTo(const std::string& sceneName);
            void update(float deltaTime);
            void render();
            void destroy();

            void setGlobalRocketSync(FoxRocketSync& rocketSync);
        private:
            std::map<std::string, std::unique_ptr<fox::scenes::FoxBaseScene>> scenes;
            fox::scenes::FoxBaseScene* currentScene;
            GLFWwindow* window;
            FoxRocketSync* globalRocketSync;
        };
    }
}

