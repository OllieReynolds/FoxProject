#include "FoxRendererSetup.hpp"
#include "FoxUtils.hpp"
#include "glad.h"

namespace fox {
    namespace gl {
        FoxRendererSetup::FoxRendererSetup() {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(fox::utils::MessageCallback, 0);

            glEnable(GL_DEPTH_TEST);
        }
    }
}
