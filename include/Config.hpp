#pragma once
#include "BloomSize.hpp"
#include <algorithm>
#include <cmath>

namespace QuestBloom {
struct Config {
    bool enabled = true;
    float intensity = 0.04f;
    int quality = 0; // 0: 512px; 1: 768px; 2: 1024px
    float radius = 4;

    void Sanitize() {
        intensity = std::isfinite(intensity) ? std::clamp(intensity, 0.0f, 0.2f) : 0.04f;
        quality = std::clamp(quality, 0, 2);
        radius = std::isfinite(radius) ? ClampBloomSize(radius) : 4.f;
    }
    bool Active() const { return enabled && intensity > 0; }
    int TextureWidth() const { return quality == 0 ? 512 : quality == 1 ? 768 : 1024; }
    int KawaseIterations() const { return std::clamp(static_cast<int>(std::lround(radius * .5f + 1)), 2, 5); }
    void Load();
    void Save();
};
Config& Settings();
}
