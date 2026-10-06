#include "main.hpp"
#include "Config.hpp"
#include "SettingsView.hpp"
#include "BeatSaber/Settings/Settings.hpp"
#include "BeatSaber/Settings/QualitySettings.hpp"
#include "GlobalNamespace/SettingsApplicatorSO.hpp"
#include "GlobalNamespace/MainEffectGraphicsSettingsPresetsSO.hpp"
#include "GlobalNamespace/MainEffectContainerSO.hpp"
#include "GlobalNamespace/KawaseBloomMainEffectSO.hpp"
#include "GlobalNamespace/PyramidBloomMainEffectSO.hpp"
#include "GlobalNamespace/SceneType.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/HideFlags.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Shader.hpp"
#include "bsml/shared/BSML.hpp"
#include "custom-types/shared/register.hpp"
#include <algorithm>
#include <string>
#include <vector>

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
Configuration& GetConfiguration() {
    static Configuration config(modInfo);
    return config;
}

namespace QuestBloom {
namespace {
UnityW<GlobalNamespace::SettingsApplicatorSO> applicator;
UnityW<GlobalNamespace::MainEffectSO> originalEffect;
UnityW<GlobalNamespace::MainEffectSO> clonedSource;
UnityW<GlobalNamespace::MainEffectSO> bloomEffect;
const char* status = "Waiting for game graphics settings";
struct MaterialState {
    UnityW<UnityEngine::Material> material;
    bool alphaEnabled;
    bool glowDisabled;
};
std::vector<MaterialState> materials;

bool Supported(GlobalNamespace::MainEffectSO* effect) {
    return il2cpp_utils::try_cast<GlobalNamespace::KawaseBloomMainEffectSO>(effect).has_value()
        || il2cpp_utils::try_cast<GlobalNamespace::PyramidBloomMainEffectSO>(effect).has_value();
}

void Configure(GlobalNamespace::MainEffectSO* effect) {
    const auto& settings = Settings();
    if (auto cast = il2cpp_utils::try_cast<GlobalNamespace::KawaseBloomMainEffectSO>(effect)) {
        auto* kawase = *cast;
        kawase->____bloomIntensity = settings.intensity;
        kawase->____bloomTextureWidth = settings.TextureWidth();
        kawase->____bloomIterations = settings.KawaseIterations();
        kawase->____bloomBoost = 0;
        kawase->____baseColorBoost = 1;
        kawase->____baseColorBoostThreshold = 0;
    } else if (auto cast = il2cpp_utils::try_cast<GlobalNamespace::PyramidBloomMainEffectSO>(effect)) {
        auto* pyramid = *cast;
        // RenderBloom's internal intensity also has a downsample offset; it is
        // not the final strength control. Scale the composited texture instead.
        pyramid->____bloomBlendFactor = settings.intensity;
        pyramid->____bloomTextureWidth = settings.TextureWidth();
        pyramid->____bloomRadius = settings.radius;
        pyramid->____baseColorBoost = 1;
        pyramid->____baseColorBoostThreshold = 0;
    }
}

bool IsVainMaterial(UnityEngine::Material* material) {
    if (!material) return false;
    // Do not patch bundle templates: new clones must inherit the port's original state.
    const auto materialName = static_cast<std::string>(material->get_name());
    if (materialName.find("(Instance)") == std::string::npos) return false;
    auto shader = material->get_shader();
    if (!shader) return false;
    const auto name = static_cast<std::string>(shader->get_name());
    return name == "VainSabers/Blur Part" || name.starts_with("VainSabers/vs_blurpart") || name == "Unlit/vs_flatglow"
        || name == "Unlit/vs_flatglow_2side";
}

bool GlowActive() {
    return Settings().Active() && bloomEffect && applicator
        && applicator->____mainEffectContainer
        && applicator->____mainEffectContainer->get_mainEffect() == bloomEffect;
}

void TrackMaterial(UnityEngine::Material* material) {
    if (!GlowActive() || !IsVainMaterial(material)) return;
    std::erase_if(materials, [](const MaterialState& state) { return !state.material; });
    auto found = std::find_if(materials.begin(), materials.end(),
        [material](const MaterialState& state) { return state.material.unsafePtr() == material; });
    if (found == materials.end())
        materials.push_back({material, material->GetShaderPassEnabled("ALPHA"),
            material->IsKeywordEnabled("_DISABLE_GLOW_PASS")});
    // Blur parts use a shader keyword; ribbon/tip trails use the named ALPHA pass.
    // ShaderPassEnabled alone cannot enable the unnamed glow pass of a blur part.
    material->DisableKeyword("_DISABLE_GLOW_PASS");
    material->SetShaderPassEnabled("ALPHA", true);
}

void RefreshMaterials() {
    static bool wasActive = false;
    if (!GlowActive()) {
        wasActive = false;
        for (auto& state : materials) {
            if (!state.material) continue;
            state.material->SetShaderPassEnabled("ALPHA", state.alphaEnabled);
            if (state.glowDisabled) state.material->EnableKeyword("_DISABLE_GLOW_PASS");
            else state.material->DisableKeyword("_DISABLE_GLOW_PASS");
        }
        materials.clear();
        return;
    }
    if (wasActive) return;
    wasActive = true;
    // Existing materials need one scan when enabling. AssignSharedMaterial covers new
    // objects thereafter; moving the intensity slider does not rescan the scene.
    for (auto material : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>())
        TrackMaterial(material);
}
} // namespace

const char* Status() { return status; }

void Apply() {
    Settings().Sanitize();
    if (!applicator || !applicator->____mainEffectContainer
        || !applicator->____mainEffectContainer->____postProcessEnabled) return;
    auto container = applicator->____mainEffectContainer;
    if (!Settings().Active()) {
        if (originalEffect) container->Init(originalEffect);
        status = "Bloom off";
        RefreshMaterials();
        return;
    }
    auto presetsSO = applicator->____mainEffectGraphicsSettingsPresets;
    if (!presetsSO || presetsSO->get_presets().size() < 2 || !presetsSO->get_presets()[1]) {
        status = "Game bloom profile unavailable";
        RefreshMaterials();
        return;
    }
    auto source = presetsSO->get_presets()[1]->mainEffect;
    if (!source || !Supported(source)) {
        status = "Unsupported game bloom profile";
        RefreshMaterials();
        return;
    }
    if (!bloomEffect || source != clonedSource) {
        // Restore before destroying an old clone so cameras never reference a dead effect.
        if (originalEffect) container->Init(originalEffect);
        if (bloomEffect) UnityEngine::Object::Destroy(bloomEffect);
        bloomEffect = UnityEngine::Object::Instantiate<GlobalNamespace::MainEffectSO*>(source);
        clonedSource = source;
        if (!bloomEffect) {
            status = "Unable to create bloom profile";
            RefreshMaterials();
            return;
        }
        bloomEffect->set_name("QuestBloom Runtime Profile");
        bloomEffect->set_hideFlags(UnityEngine::HideFlags::HideAndDontSave);
    }
    Configure(bloomEffect);
    container->Init(bloomEffect);
    status = Settings().quality == 0 ? "Game bloom active / Standard (512)"
        : Settings().quality == 1 ? "Game bloom active / High (768)" : "Game bloom active / Ultra (1024)";
    RefreshMaterials();
}
} // namespace QuestBloom

MAKE_HOOK_MATCH(ApplyGraphics, &GlobalNamespace::SettingsApplicatorSO::ApplyGraphicSettings,
    void, GlobalNamespace::SettingsApplicatorSO* self,
    ByRef<BeatSaber::Settings::Settings> settings, GlobalNamespace::SceneType sceneType) {
    // Keep resolution, antialiasing, mirrors, particles and saved game settings intact.
    ApplyGraphics(self, settings, sceneType);
    QuestBloom::applicator = self;
    QuestBloom::originalEffect = self->____mainEffectContainer
        ? self->____mainEffectContainer->get_mainEffect().unsafePtr() : nullptr;
    QuestBloom::Apply();
    bloomLogger.info("{}", QuestBloom::Status());
}

MAKE_HOOK_MATCH(AssignSharedMaterial, &UnityEngine::Renderer::set_sharedMaterial,
    void, UnityEngine::Renderer* self, UnityEngine::Material* material) {
    AssignSharedMaterial(self, material);
    QuestBloom::TrackMaterial(material);
}

MAKE_HOOK_MATCH(AssignMaterial, &UnityEngine::Renderer::set_material,
    void, UnityEngine::Renderer* self, UnityEngine::Material* material) {
    AssignMaterial(self, material);
    QuestBloom::TrackMaterial(material);
}

BLOOM_EXPORT void setup(CModInfo* info) noexcept {
    *info = modInfo.to_c();
    GetConfiguration().Load();
    QuestBloom::Settings().Load();
}

BLOOM_EXPORT void late_load() noexcept {
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    BSML::Init();
    INSTALL_HOOK(bloomLogger, ApplyGraphics);
    INSTALL_HOOK(bloomLogger, AssignSharedMaterial);
    INSTALL_HOOK(bloomLogger, AssignMaterial);
    BSML::Register::RegisterSettingsMenu<QuestBloom::SettingsView*>("QuestBloom");
    // Handles a late loader as well as normal startup. The hook covers later scene changes.
    auto all = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SettingsApplicatorSO*>();
    if (all.size() > 0 && all[0] && all[0]->____mainEffectContainer) {
        QuestBloom::applicator = all[0];
        QuestBloom::originalEffect = all[0]->____mainEffectContainer->get_mainEffect();
        QuestBloom::Apply();
    }
    bloomLogger.info("QuestBloom {} loaded: intensity {}, quality {}", VERSION,
        QuestBloom::Settings().intensity, QuestBloom::Settings().quality);
}
