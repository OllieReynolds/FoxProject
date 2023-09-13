#include "FoxGLFWSetup.hpp"
#include "glad.h"
#include <iostream>

#include "FoxGlobals.hpp"

namespace fox {
    namespace gl {
        FoxGLFWSetup::FoxGLFWSetup() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
            if (window == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(-1);
            }

            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, fox::utils::framebuffer_size_callback);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize GLAD" << std::endl;
                glfwTerminate();
                exit(-1);
            }
        }

        FoxGLFWSetup::~FoxGLFWSetup() {
            glfwTerminate();
        }

        GLFWwindow* FoxGLFWSetup::getWindow() const {
            return window;
        }
    }
}
