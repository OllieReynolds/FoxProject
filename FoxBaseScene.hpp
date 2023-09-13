#pragma once

#include "FoxGlobals.hpp"

namespace fox {
    namespace scenes {
        class FoxBaseScene {
        public:
            virtual ~FoxBaseScene() {}

            virtual void init(GLFWwindow* window) = 0;
            virtual void update(float deltaTime, GLFWwindow* window) = 0;
            virtual void render() = 0;
            virtual void destroy() = 0;
        };
    }
}
