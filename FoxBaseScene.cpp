// FoxBaseScene.cpp

#include "FoxBaseScene.hpp"

namespace fox {
    namespace scenes {
        void FoxBaseScene::setGlobalRocketSync(fox::utils::FoxRocketSync& globalRocketSync) {
            this->globalRocketSync = &globalRocketSync;
        }
    }
}
