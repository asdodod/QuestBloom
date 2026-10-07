# QuestBloom

**THIS MOD WAS CREATED WITH AI.** AI was used to write and modify this mod.

Configurable bloom for the whole game on **Beat Saber Quest 1.40.8_7379**, ARM64, **Scotland2**. Adds glow to sabers, notes and environment lights.

## Installation

Download `QuestBloom.qmod` from [Releases](https://github.com/asdodod/QuestBloom/releases) and install it with your Quest mod manager, such as ModsBeforeFriday. Restart Beat Saber, then open **Settings → Mod Settings → QuestBloom**.

## Settings

| Setting | What it does | Default |
| --- | --- | --- |
| Enable bloom | Turns bloom on or off | On |
| Intensity | Glow strength, from 0 to 0.20 | 0.04 |
| Bloom size | Glow spread, from 3 to 8 | 4 |
| Quality | Standard (512), High (768), Ultra (1024) | Standard |

Changes apply immediately and save automatically. Higher quality reduces coarse sampling and uses more GPU resources. Start with Standard and adjust intensity to taste.

VainSabers is optional. Compatibility with its glow materials is automatic. If another graphics mod also enables bloom, turn that option off before adjusting QuestBloom.

See [Patch notes](PATCH_NOTES.md) for version changes.

## Building

Requirements: Android NDK r27, QPM, CMake 3.22+, Ninja and PowerShell 7.

```powershell
qpm restore
$env:ANDROID_NDK_HOME = 'C:/Android/ndk/r27/android-ndk-r27-windows/android-ndk-r27'
./scripts/build.ps1
./scripts/package.ps1
```

Unity and an AssetBundle are not required. Game binaries and game assets are not included.

## Credits

Uses QuestPackageManager, beatsaber-hook, custom-types, bs-cordl, Scotland2 and Quest-BSML. GraphicsTweaks helped inform the game's effect integration. See [third-party notices](THIRD_PARTY_NOTICES.md).
