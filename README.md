# QuestBloom

**THIS MOD WAS CREATED WITH AI.** AI was used to write and modify the code. This is an experimental mod; headset test results and limitations are recorded in [VALIDATION.md](VALIDATION.md).

Separate experimental bloom mod for **Beat Saber Quest 1.40.8_7379**, ARM64, Scotland2.

Version: **0.1.3**. This version provides **whole-game bloom**, including sabers, notes and scene lights. The user reported a frozen bloom image at Bloom size 2.5 in 0.1.2. At 512 pixels the game's Pyramid renderer uses only one level at that size and never writes the final bloom texture. This update enforces a safe size range and makes VainSabers compatibility automatic. On October 6, 2026, the user confirmed that QuestBloom works and approved its release. GPU cost has not been measured.

## Install and adjust

Install `QuestBloom.qmod` using your Quest mod manager, then open **Settings → Mod Settings → QuestBloom**.

- **Enable bloom**: turn the effect on/off. Intensity zero also disables it.
- **Intensity**: 0–0.20; default **0.04**. It controls the final composite strength for both supported renderers.
- **Bloom size**: 3–8; default **4**. Changes the Pyramid blur radius or the Kawase iteration count independently of texture resolution. Older values below 3 are raised to 3 on load.
- **Quality**: default **Standard (512)**. High uses 768 pixels; Ultra uses 1024 pixels.

VainSabers materials receive their glow shader keyword/pass automatically while bloom is active. This is compatibility handling, not a saber-only effect. It restores their original state when bloom is disabled. VainSabers itself is optional; there is no separate compatibility toggle.

The mod replaces the game's active main effect with one runtime clone of the existing Game bloom profile. It retains the original renderer, shader references, fade handling and VR texture handling. It does not add a second full-screen bloom camera. Disabling the mod restores the main effect selected by the game (or another graphics mod).

Standard uses the game's 512-pixel profile width. Bloom size 4 uses radius 4 for Pyramid or three iterations for Kawase; the supported size range uses three to five Kawase iterations. Quality changes the texture width without changing the configured size. Raising texture resolution addresses the coarse sampling reported in 0.1.0, but increases pixel work compared with its 256-pixel default. Actual frame cost still needs measurement on Quest. Game resolution, antialiasing, mirrors and other saved graphics settings are unchanged.

On the first upgrade from 0.1.0, Intensity and Quality reset to **0.04 / Standard**, while Enable bloom is preserved. Subsequent changes persist normally. Pyramid intensity previously changed an internal filter parameter while the final blend stayed at 1; it now changes the final blend directly.

Upgrading from 0.1.1 preserves the existing controls and initializes Bloom size to its previous profile value (4 for Standard, 6 for High). Upgrading from 0.1.2 preserves valid sizes and raises unsafe sizes to 3. Changes apply while the game is running and save automatically. Intensity displays three decimal places so its 0.005 steps remain visible.

If using GraphicsTweaks, start with its bloom option off so the on/off comparison is clear. Both mods operate on the game's main effect; QuestBloom's strength controls this runtime profile. Disabling QuestBloom restores the graphics profile selected by GraphicsTweaks if it was active.

## Build

Requirements: Android NDK r27, CMake 3.22+, Ninja, QPM, PowerShell 7.

```powershell
qpm restore
$env:ANDROID_NDK_HOME = 'C:/Android/ndk/r27/android-ndk-r27-windows/android-ndk-r27'
./scripts/build.ps1
./scripts/package.ps1
```

If CMake/Ninja are not on PATH, pass `-CMakePath` and `-NinjaPath`. A local build may use already downloaded matching dependencies with `-DependenciesPath`; standalone checkouts use `qpm restore`. Versions are pinned in `qpm.json` and `qpm.shared.json`.

The project uses native C++ and shaders already present in the installed game. **Unity, an AssetBundle build and ADB are not required.** It contains no redistributed Beat Saber assemblies or assets. The completed VainSabers project is independent.

## Validation

See [VALIDATION.md](VALIDATION.md) for local checks, the user-reported headset result and additional test cases. No measured performance claim is made.

`scripts/test-bloom-size.ps1 -NdkPath <NDK> -NodePath <node>` runs the production size bounds as WebAssembly and checks them against the game's pyramid level calculation without Unity or ADB.

## Credits

Uses QuestPackageManager/beatsaber-hook, custom-types, bs-cordl, Scotland2 and Quest-BSML. GraphicsTweaks was consulted to understand the game's main-effect selection. The game's local managed assemblies were inspected to verify profile fields and rendering flow; they are not included in this source project.
