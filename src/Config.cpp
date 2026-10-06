#include "Config.hpp"
#include "main.hpp"

namespace QuestBloom {
Config& Settings() {
    static Config settings;
    return settings;
}

void Config::Load() {
    auto& doc = GetConfiguration().config;
    if (doc.IsObject()) {
        if (doc.HasMember("enabled") && doc["enabled"].IsBool())
            enabled = doc["enabled"].GetBool();
        if (doc.HasMember("intensity") && doc["intensity"].IsNumber())
            intensity = doc["intensity"].GetFloat();
        if (doc.HasMember("quality") && doc["quality"].IsInt())
            quality = doc["quality"].GetInt();
        radius = quality == 0 ? 4.f : 6.f;
        if (doc.HasMember("radius") && doc["radius"].IsNumber())
            radius = doc["radius"].GetFloat();
        // Revision 1 used an incorrect Pyramid composite strength and coarse
        // textures. Reset only those two controls once when upgrading it.
        if (!doc.HasMember("profileRevision") || !doc["profileRevision"].IsInt()
            || doc["profileRevision"].GetInt() < 2) {
            intensity = 0.04f;
            quality = 0;
            radius = 4;
            Save();
        }
    }
    Sanitize();
}

void Config::Save() {
    Sanitize();
    auto& doc = GetConfiguration().config;
    if (!doc.IsObject()) doc.SetObject();
    auto& allocator = doc.GetAllocator();
    auto put = [&](const char* name, auto value) {
        if (doc.HasMember(name)) doc[name] = value;
        else doc.AddMember(rapidjson::Value(name, allocator), rapidjson::Value(value), allocator);
    };
    put("enabled", enabled);
    put("intensity", intensity);
    put("quality", quality);
    put("radius", radius);
    // Compatibility is automatic; it does not restrict the whole-game effect.
    doc.RemoveMember("vainSabersGlow");
    put("profileRevision", 2);
    GetConfiguration().Write();
}
}
