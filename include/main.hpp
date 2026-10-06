#pragma once
#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "Logger.hpp"

#define BLOOM_EXPORT extern "C" __attribute__((visibility("default")))
Configuration& GetConfiguration();

namespace QuestBloom {
void Apply();
const char* Status();
}
