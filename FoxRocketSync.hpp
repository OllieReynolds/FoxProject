#pragma once
#include "sync.h"

namespace fox {
    namespace utils {
        class FoxRocketSync {
        public:
            FoxRocketSync(const char* trackFile);
            ~FoxRocketSync();
            void update(double row);
            float getTrackValue(const char* trackName, double row);
        private:
            sync_device* rocket;
        };
    }
}
