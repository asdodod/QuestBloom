# Validation — 0.1.3

## Local checks completed

- Android NDK r27 / Clang 18 Release build completed for `aarch64-none-linux-android24` against the pinned 1.40.8 Cordl headers. Build used matching dependencies already downloaded in the adjacent VainSabers workspace.
- Linker completed with `--no-undefined`.
- Library is ELF64 / AArch64, with Scotland2 `setup` and `late_load` exports.
- No unresolved direct UnityEngine, inline-hook or Paper logging bridge symbols. Dynamic dependency libraries match the QMOD dependencies and the Scotland2 loader.
- `mod.json` passes the official QuestPatcher QMOD JSON schema.
- PowerShell build/package scripts pass syntax parsing.
- Packaged library bytes match the built library by SHA256. ZIP entries contain the manifest, native library and third-party notices.
- Main-effect selection/rendering flow and relevant bloom profile fields were checked against the installed PC 1.40.8 assemblies and Quest 1.40.8 headers. The runtime clone keeps the game's own rendering implementation.
- Pyramid final composite strength now follows Intensity instead of remaining at 1. Its internal filter intensity is retained from the cloned game profile.
- Default Intensity is 0.04. Standard / High / Ultra texture widths are 512 / 768 / 1024. Independent Bloom size ranges from 3 to 8 (default 4); it sets Pyramid radius or three to five Kawase iterations.
- Version 0.1.0's strength and quality controls migrate once; enabled is retained. The compatibility toggle is removed and VainSabers support is automatic while the whole-game effect is active.
- Renderer `material` and `sharedMaterial` assignments both register newly created VainSabers materials, including tip trails.
- All four settings (Enable, Intensity, Bloom size, Quality) save through the same configuration and apply immediately. Intensity uses three displayed decimal places. Version 0.1.1 settings are retained and its previous size is used on upgrade; unsafe sizes from 0.1.2 are clamped on load.
- Inspected the installed game's PyramidBloomRendererSO: one pyramid level produces no final upsample/write. At width 512 and size 2.5 this leaves the bloom destination undefined and can retain an old image. Minimum size 3 guarantees at least two levels for the supported widths.
- Production bounds ran as WebAssembly against the reference level calculation: 375 size/resolution/aspect cases all write the destination. Old size 2.5 reproduced the missing-write path. This is a calculation regression, not headset rendering validation.

## User-reported headset result

QMOD SHA256: `37F92B9B057DAF9FE58FC2E08745DC5FE73E37651EA81723C46B320AC71F61C1`.
Library SHA256: `87E13F6EB44DA3CC60EA7AFFE3B795408EE7E5478B571B8104CAA25C2C834610`.

The user loaded 0.1.0 on Quest and reported excessive brightness and large pixels, then reported frozen glow at Bloom size 2.5 in 0.1.2. On October 6, 2026, the user reported that QuestBloom is ready and requested repository/release publication. This is user-reported acceptance of 0.1.3; it does not establish that every case below was tested. No frame-time measurement is available.

## Additional headset checks

1. Install on **1.40.8_7379 / Scotland2** and open **Settings → Mod Settings → QuestBloom**. Verify all four controls, including draggable Intensity and Bloom size sliders. Intensity increments should be visible to three decimal places. A saved size of 2.5 must load as 3; move the head at size 3 and check for any frozen glow image.
2. Start at **0.04 / Standard** (the upgrade resets these controls once). Compare enabled/disabled using the same song and saber preset. Confirm both eyes show the same effect, with no black frame, bright flash or displaced image. Compare High if coarse pixels remain.
3. With VainSabers enabled, check normal/inverted/lit parts, motion blur, blade trails and tip trails. Compatibility is automatic. Toggle bloom itself and confirm material state restores without changing geometry or saber colors. Also confirm notes and scene lights receive bloom.
4. Set Intensity to zero, disable/re-enable bloom, change size and all three quality modes, return to the menu, switch environments and restart the game. Confirm saved settings persist and the effect is restored correctly. Also check these transitions without VainSabers installed.
5. If GraphicsTweaks is installed, compare with its bloom option off, then on. Ensure there is one bloom effect and that disabling QuestBloom restores the previous game profile.
6. Measure GPU frame time / dropped frames on the same map with bloom off, Standard, High and Ultra, especially on Quest 2 with fast swings. Higher resolution costs more than 0.1.0; no measured FPS gain is claimed.

Saber-only masking is **not implemented** in 0.1.3. This build uses the whole-game option requested in the conversation.
