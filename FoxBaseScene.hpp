#pragma once

#include "FoxGlobals.hpp"
#include "FoxRocketSync.hpp"

namespace fox {
    namespace scenes {
        class FoxBaseScene {
        public:
            virtual ~FoxBaseScene() {}

            virtual void init(GLFWwindow* window) = 0;
            virtual void update(float deltaTime, GLFWwindow* window) = 0;
            virtual void render() = 0;
            virtual void destroy() = 0;

            void setGlobalRocketSync(fox::utils::FoxRocketSync& globalRocketSync);

        protected:
            fox::utils::FoxRocketSync* globalRocketSync = nullptr;
        };
    }
}
