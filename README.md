<div align="center">

<h1>✨ QuestBloom</h1>

**Bring the glow to Quest. Tune it your way.**

[![Download](https://img.shields.io/badge/⬇_Download-QMOD-00c8f8?style=for-the-badge)](https://github.com/asdodod/QuestBloom/releases/latest)
[![Patch notes](https://img.shields.io/badge/✦_Patch_Notes-0.1.3-e147ac?style=for-the-badge)](PATCH_NOTES.md)
[![VainSabers](https://img.shields.io/badge/⚔_Pair_With-VainSabers-8055e7?style=for-the-badge)](https://github.com/asdodod/vsq-port)

![Version](https://img.shields.io/badge/Version-0.1.3-00c8f8?style=flat-square)
![Beat Saber](https://img.shields.io/badge/Beat_Saber-1.40.8__7379-white?style=flat-square)
![Platform](https://img.shields.io/badge/Quest-ARM64-8055e7?style=flat-square)
![Modloader](https://img.shields.io/badge/Modloader-Scotland2-8055e7?style=flat-square)

</div>

---

> 🤖 **THIS MOD WAS CREATED WITH AI.** AI was used to write and modify this mod.

## ✨ What is QuestBloom?

Configurable **whole-game bloom** for Beat Saber on standalone Quest. Add glow to sabers, notes and environment lights, then adjust its strength, spread and quality in-game.

| | Your glow, your settings |
| :--- | :--- |
| 💡 **Enable / disable** | Toggle bloom whenever you want |
| 🎚️ **Intensity** | Control how strong the glow looks |
| 🌈 **Bloom size** | Adjust how far the glow spreads |
| 🔍 **Quality** | Choose Standard, High or Ultra |
| ⚔️ **VainSabers support** | Automatic compatibility with its glow materials |
| 💾 **Live changes** | Settings apply immediately and save automatically |

## 📥 Install

1. Use a modded **Beat Saber 1.40.8_7379** installation with **Scotland2**.
2. Download **QuestBloom.qmod** from [Releases](https://github.com/asdodod/QuestBloom/releases/latest).
3. Install it with your Quest mod manager, such as [ModsBeforeFriday](https://mbf.bsquest.xyz/), including its required dependencies.
4. Restart Beat Saber and open **Settings → Mod Settings → QuestBloom**.
5. Enable bloom and adjust it to taste.

## 🎛️ Find your look

| Setting | Range / options | Default |
| :--- | :--- | :--- |
| **Enable bloom** | On / Off | On |
| **Intensity** | 0.00–0.20 | 0.04 |
| **Bloom size** | 3–8 | 4 |
| **Quality** | Standard 512 · High 768 · Ultra 1024 | Standard |

Start with the defaults. Lower **Intensity** for a subtle glow; raise **Bloom size** for a wider halo. Higher **Quality** reduces coarse sampling and uses more GPU resources.

If another graphics mod also enables bloom, disable that bloom option before adjusting QuestBloom.

## ⚔️ Works with VainSabers

[**VainSabers Quest**](https://github.com/asdodod/vsq-port) is optional. QuestBloom works across the game without it, and enables compatibility with VainSabers glow materials automatically while bloom is active.

QuestBloom uses the game's Quest bloom renderer. It does not promise an identical copy of the PC bloom effect.

## 🔧 Build from source

Requirements: **QPM**, **CMake 3.22+**, **Ninja**, **Android NDK r27** and **PowerShell 7**.

```powershell
qpm restore
$env:ANDROID_NDK_HOME = 'C:/Android/ndk/r27/android-ndk-r27-windows/android-ndk-r27'
./scripts/build.ps1
./scripts/package.ps1
```

Unity and an AssetBundle are not required. Game binaries and game assets are not included.

## ❤️ Credits

Built with **QuestPackageManager**, **beatsaber-hook**, **custom-types**, **bs-cordl**, **Scotland2** and **Quest-BSML**. **GraphicsTweaks** helped inform the game's effect integration.

See [third-party notices](THIRD_PARTY_NOTICES.md) for attribution.

---

<div align="center">

**[Download](https://github.com/asdodod/QuestBloom/releases/latest) · [Patch notes](PATCH_NOTES.md) · [Report a bug](https://github.com/asdodod/QuestBloom/issues)**

</div>
