#pragma once

#include "FoxGlobals.hpp"
#include "FoxRocketSync.hpp"

#include <filesystem>

namespace fox {
    namespace scenes {
        class FoxBaseScene {
        public:
            virtual ~FoxBaseScene() {}

            virtual void init(GLFWwindow* window) = 0;
            virtual void update(float deltaTime, GLFWwindow* window);
            virtual void render() = 0;
            virtual void destroy() = 0;

            void setGlobalRocketSync(fox::utils::FoxRocketSync& globalRocketSync);

            void setReloadShadersFlag(bool flag);

        protected:
            struct ShaderInfo {
                unsigned int program;  // OpenGL shader program ID
                std::string vertPath;  // File path for the vertex shader
                std::string fragPath;  // File path for the fragment shader
                std::filesystem::file_time_type lastModTimeVert;  // Last modification time for vertex shader
                std::filesystem::file_time_type lastModTimeFrag;  // Last modification time for fragment shader
            };

            void addShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
            void reloadShadersIfModified();

            void checkForShaderReload();

            unsigned int getShaderProgram(const std::string& name);

            std::map<std::string, ShaderInfo> shaders;

            fox::utils::FoxRocketSync* globalRocketSync = nullptr;

            bool reloadShadersFlag = false;
        };
    }
}
