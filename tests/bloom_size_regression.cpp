#include "BloomSize.hpp"
extern "C" float BoundSize(float size) {
    return QuestBloom::ClampBloomSize(size);
}
extern "C" float MinimumSize() {
    return QuestBloom::MinimumBloomSize;
}
extern "C" float MaximumSize() {
    return QuestBloom::MaximumBloomSize;
}
