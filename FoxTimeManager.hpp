#pragma once

namespace fox {
    namespace utils {
        class FoxTimeManager {
        public:
            FoxTimeManager();
            void update();
            float getDeltaTime() const;
        private:
            float lastFrame;
            float deltaTime;
        };
    }
}

