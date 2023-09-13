#include "FoxSceneSwitcher.hpp"
#include "FoxScene.hpp"
#include "FoxRocketSync.hpp"

namespace fox {
    namespace utils {
        FoxSceneSwitcher::FoxSceneSwitcher(GLFWwindow* window) : currentScene(nullptr), window(window), globalRocketSync{ nullptr } {}

        void FoxSceneSwitcher::setGlobalRocketSync(FoxRocketSync& rocketSync) {
            this->globalRocketSync = &rocketSync;
        }

        void FoxSceneSwitcher::switchTo(const std::string& sceneName) {
            if (currentScene) {
                currentScene->destroy();
            }
            if (scenes.find(sceneName) == scenes.end()) {
                if (sceneName == "FoxScene") {
                    scenes[sceneName] = std::make_unique<fox::scenes::FoxScene>();
                }

                scenes[sceneName]->init(window);
                if (globalRocketSync) {
                    scenes[sceneName]->setGlobalRocketSync(*globalRocketSync);
                }
            }
            currentScene = scenes[sceneName].get();
        }

        void FoxSceneSwitcher::update(float deltaTime) {
            if (currentScene) {
                currentScene->update(deltaTime, window);
            }
        }

        void FoxSceneSwitcher::render() {
            if (currentScene) {
                currentScene->render();
            }
        }

        void FoxSceneSwitcher::destroy() {
            if (currentScene) {
                currentScene->destroy();
            }
        }
    }
}

