#pragma once
#include <string>
#include <unordered_map>
#include <fstream>

namespace fox {
    namespace utils {
        class FoxConfigurationManager {
        public:
            static FoxConfigurationManager& getInstance();
            std::string get(const std::string& key);
            void loadConfig(const std::string& filePath);

        private:
            FoxConfigurationManager() = default;
            ~FoxConfigurationManager() = default;
            FoxConfigurationManager(const FoxConfigurationManager&) = delete;
            FoxConfigurationManager& operator=(const FoxConfigurationManager&) = delete;

            std::unordered_map<std::string, std::string> configMap;
        };

        inline std::string getConfig(const std::string& key) {
            return FoxConfigurationManager::getInstance().get(key);
        }
    }
}
