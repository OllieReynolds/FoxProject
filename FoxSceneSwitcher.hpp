#pragma once

// Standard Library Includes
#include <map>
#include <memory>
#include <string>

// Project Includes
#include "FoxBaseScene.hpp"
#include "FoxRocketSync.hpp"

// External Library Includes
#include <glfw3.h>

// Namespace Declaration
namespace fox {
    namespace utils {

        // Class Declaration
        class FoxSceneSwitcher {
        public:
            // Constructors
            FoxSceneSwitcher(GLFWwindow* window);

            // Public Member Functions
            void switchTo(const std::string& sceneName);
            void update(float deltaTime);
            void render();
            void destroy();

            // Setters
            void setGlobalRocketSync(FoxRocketSync& rocketSync);

        private:
            // Private Member Variables
            std::map<std::string, std::unique_ptr<fox::scenes::FoxBaseScene>> scenes;
            fox::scenes::FoxBaseScene* currentScene;
            GLFWwindow* window;
            FoxRocketSync* globalRocketSync;
        };

    } // namespace utils
} // namespace fox
