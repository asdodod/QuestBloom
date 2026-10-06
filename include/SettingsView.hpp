#pragma once
#include "HMUI/ViewController.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/GameObject.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestBloom, SettingsView, HMUI::ViewController) {
    DECLARE_INSTANCE_FIELD(UnityW<UnityEngine::GameObject>, content);
    DECLARE_INSTANCE_FIELD(UnityW<HMUI::CurvedTextMeshPro>, statusText);
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate,
        bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
public:
    void UpdateStatus();
};
