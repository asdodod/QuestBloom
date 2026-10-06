#include "SettingsView.hpp"
#include "Config.hpp"
#include "main.hpp"
#include "bsml/shared/BSML.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include <array>

DEFINE_TYPE(QuestBloom, SettingsView);

namespace QuestBloom {
void SettingsView::UpdateStatus() {
    if (statusText) statusText->set_text(Status());
}

void SettingsView::DidActivate(bool firstActivation, bool, bool) {
    if (!firstActivation) {
        UpdateStatus();
        return;
    }
    auto layout = BSML::Lite::CreateVerticalLayoutGroup(get_transform());
    content = layout->get_gameObject();
    auto rect = content->GetComponent<UnityEngine::RectTransform*>();
    rect->set_anchorMin({0.5f, 0.5f});
    rect->set_anchorMax({0.5f, 0.5f});
    rect->set_sizeDelta({100, 80});
    layout->set_spacing(1);
    layout->set_childForceExpandHeight(false);
    layout->set_childControlHeight(true);
    auto parent = layout->get_transform();
    auto header = BSML::Lite::CreateText(parent, "QuestBloom", TMPro::FontStyles::Normal, 5);
    header->set_alignment(TMPro::TextAlignmentOptions::Center);
    auto& cfg = Settings();
    BSML::Lite::CreateToggle(parent, "Enable bloom", cfg.enabled, [this](bool value) {
        Settings().enabled = value;
        Settings().Save();
        Apply();
        UpdateStatus();
    });
    auto intensitySlider = BSML::Lite::CreateSliderSetting(parent, "Intensity", 0.005f, cfg.intensity, 0, 0.2f,
        0.15f, true, UnityEngine::Vector2{0, 0}, [this](float value) {
            Settings().intensity = value;
            Settings().Save();
            Apply();
            UpdateStatus();
        });
    intensitySlider->digits = 3;
    intensitySlider->set_Value(cfg.intensity);
    auto radiusSlider = BSML::Lite::CreateSliderSetting(parent, "Bloom size", 0.5f, cfg.radius,
        MinimumBloomSize, MaximumBloomSize,
        0.15f, true, UnityEngine::Vector2{0, 0}, [this](float value) {
            Settings().radius = value;
            Settings().Save();
            Apply();
            UpdateStatus();
        });
    radiusSlider->digits = 1;
    radiusSlider->set_Value(cfg.radius);
    std::array<std::string_view, 3> qualities{"Standard (512)", "High (768)", "Ultra (1024)"};
    BSML::Lite::CreateDropdown(parent, "Quality", qualities[cfg.quality], qualities, [this](StringW value) {
        const auto name = static_cast<std::string>(value);
        Settings().quality = name == "Ultra (1024)" ? 2 : name == "High (768)" ? 1 : 0;
        Settings().Save();
        Apply();
        UpdateStatus();
    });
    auto note = BSML::Lite::CreateText(parent, "Whole-game bloom. Start with 0.04 intensity.",
        TMPro::FontStyles::Normal, 3);
    note->set_alignment(TMPro::TextAlignmentOptions::Center);
    note->set_enableWordWrapping(true);
    statusText = BSML::Lite::CreateText(parent, Status(), TMPro::FontStyles::Normal, 2.8f);
    statusText->set_alignment(TMPro::TextAlignmentOptions::Center);
}
}
