#pragma once

namespace QuestBloom {
// At the smallest supported width (512), sizes below 3 produce one pyramid
// level. The game's upsample loop then skips writing its destination texture.
inline constexpr float MinimumBloomSize = 3;
inline constexpr float MaximumBloomSize = 8;
constexpr float ClampBloomSize(float size) {
    return size != size ? 4 : size < MinimumBloomSize ? MinimumBloomSize
        : size > MaximumBloomSize ? MaximumBloomSize : size;
}
}
