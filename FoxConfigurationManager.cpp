#include "FoxConfigurationManager.hpp"
#include <sstream>

namespace fox {
    namespace utils {
        FoxConfigurationManager& FoxConfigurationManager::getInstance() {
            static FoxConfigurationManager instance;
            return instance;
        }

        void FoxConfigurationManager::loadConfig(const std::string& filePath) {
            std::ifstream file(filePath);
            std::string line;
            if (file.is_open()) {
                while (std::getline(file, line)) {
                    std::string key, value;
                    std::istringstream iss(line);
                    std::getline(iss, key, '=');
                    std::getline(iss, value, '=');
                    configMap[key] = value;
                }
            }
        }

        std::string FoxConfigurationManager::get(const std::string& key) {
            return configMap[key];
        }
    }
}



