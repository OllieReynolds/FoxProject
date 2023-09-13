#pragma once

struct GLFWwindow;

namespace fox {
    namespace gl {
        class FoxGLFWSetup {
        public:
            FoxGLFWSetup();
            ~FoxGLFWSetup();
            GLFWwindow* getWindow() const;
        private:
            GLFWwindow* window;
        };
    }
}
