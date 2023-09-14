#pragma once

// Standard Includes
#include <filesystem>
#include <map>
#include <string>

// Project Includes
#include "FoxGlobals.hpp"
#include "FoxRocketSync.hpp"

// Forward Declarations
struct GLFWwindow;

namespace fox {
    namespace scenes {

        // Class Declaration
        class FoxBaseScene {
        public:
            // Constructors and Destructors
            virtual ~FoxBaseScene();

            // Public Member Functions (Virtual for derived classes)
            virtual void init(GLFWwindow* window) = 0;
            virtual void update(float deltaTime, GLFWwindow* window);
            virtual void render() = 0;
            virtual void destroy() = 0;

            // Public Member Functions (Non-virtual)
            void setGlobalRocketSync(fox::utils::FoxRocketSync& globalRocketSync);
            void setReloadShadersFlag(bool flag);

        protected:
            // Protected Types
            struct ShaderInfo {
                unsigned int program;
                std::string vertPath;
                std::string fragPath;
                std::filesystem::file_time_type lastModTimeVert;
                std::filesystem::file_time_type lastModTimeFrag;
            };

            // Protected Member Functions
            void addShader(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath = "");
            void reloadShadersIfModified();
            void checkForShaderReload();
            unsigned int getShaderProgram(const std::string& name);

            // Protected Member Variables
            std::map<std::string, ShaderInfo> shaders;
            fox::utils::FoxRocketSync* globalRocketSync = nullptr;
            bool reloadShadersFlag = false;
        };

    }  // namespace scenes
}  // namespace fox
