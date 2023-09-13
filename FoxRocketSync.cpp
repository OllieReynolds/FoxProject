#include "FoxRocketSync.hpp"
#include <iostream>
#include <cstdlib>

namespace fox {
    namespace utils {
        FoxRocketSync::FoxRocketSync(const char* trackFile) {
            rocket = sync_create_device(trackFile);
            if (!rocket) {
                std::cerr << "Failed to create Rocket device." << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        FoxRocketSync::~FoxRocketSync() {
            if (rocket) {
                sync_destroy_device(rocket);
            }
        }

        void FoxRocketSync::update(double row) {
            if (sync_update(rocket, (int)row, nullptr, 0)) {
                // Handle track reloading or other tasks here
            }
        }

        float FoxRocketSync::getTrackValue(const char* trackName, double row) {
            return sync_get_val(sync_get_track(rocket, trackName), row);
        }
    }
}
