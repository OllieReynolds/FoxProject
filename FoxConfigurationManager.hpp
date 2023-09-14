#pragma once

// Standard Library Includes
#include <string>
#include <unordered_map>
#include <fstream>

// Namespace Declaration
namespace fox {
    namespace utils {

        // Class Declaration
        class FoxConfigurationManager {
        public:
            // Singleton Accessor
            static FoxConfigurationManager& getInstance();

            // Public Member Functions
            std::string get(const std::string& key);
            void loadConfig(const std::string& filePath);

        private:
            // Constructors and Assignment Operators (Private for Singleton)
            FoxConfigurationManager() = default;
            ~FoxConfigurationManager() = default;
            FoxConfigurationManager(const FoxConfigurationManager&) = delete;
            FoxConfigurationManager& operator=(const FoxConfigurationManager&) = delete;

            // Private Member Variables
            std::unordered_map<std::string, std::string> configMap;
        };

        // Utility Function
        inline std::string getConfig(const std::string& key) {
            return FoxConfigurationManager::getInstance().get(key);
        }

    } // namespace utils
} // namespace fox
