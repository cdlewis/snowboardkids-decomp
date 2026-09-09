#!/usr/bin/env python3
"""Check exported N64 layouts and linker aliases after a matching ROM build."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
SOURCE = r'''
#include "game/race/camera/race_camera.h"
#include "game/menu/menu_scratch.h"
#include "game/menu/controller_pak/controller_pak_ui.h"
#include "game/engine/render_callback.h"
#include "game/race/ui/race_ui_effects.h"
#include "game/race/player/race_player_model_renderer.h"
#include "game/engine/system_runtime.h"
#include "game/engine/system_runtime.h"
#define OFFSET(type, field, expected) \
    _Static_assert(__builtin_offsetof(type, field) == expected, #type "." #field)
_Static_assert(sizeof(void *) == 4, "N64 pointer width");
_Static_assert(sizeof(RaceCamera) == 0xB0, "camera stride");
OFFSET(RaceCamera, initialized, 0xAC);
_Static_assert(sizeof(gRaceCameras) == 0x2C0, "four cameras");
_Static_assert(sizeof(MenuScratchSlot) == 4, "scratch stride");
OFFSET(MenuScratchSlot, task, 0);
OFFSET(MenuScratchSlot, endingSnowboard, 0);
OFFSET(ControllerPakRumbleCheckPromptTransition, state, 2);
OFFSET(ControllerPakRumbleCheckPromptTransition, messageIndex, 4);
OFFSET(ControllerPakRumbleCheckPromptTransition, confirmSelection, 9);
OFFSET(ControllerPakRumbleCheckPromptState, messageIndex, 2);
OFFSET(ControllerPakRumbleCheckPromptState, confirmSelection, 7);
OFFSET(ControllerPakRumbleCheckPromptActor, messageIndex, 0x24);
_Static_assert(sizeof(RenderCallbackQueueSlot) == 0xC, "queue stride");
OFFSET(RenderCallbackQueueSlot, head, 0);
_Static_assert(sizeof(gModelRenderCallbackQueues) == 0x60, "eight queue slots");
OFFSET(RaceUiOverlayActor, matrix, 0x34);
OFFSET(RaceUiOverlayActor, panelAPalette, 0x38);
OFFSET(RaceUiOverlayActor, panelAImage, 0x3C);
OFFSET(RaceUiOverlayActor, panelBPalette, 0x40);
OFFSET(RaceUiOverlayActor, panelBImage, 0x44);
OFFSET(RaceUiOverlayActor, matrixDirty, 0x48);
_Static_assert(sizeof(RaceUiOverlayActor) == 0x4C, "starter actor size");
_Static_assert(sizeof(gRaceHudItemEffectTileOffsets) == 8, "item table byte stride");
_Static_assert(sizeof(gRaceHudActionEffectTileIds) == 8, "action table byte stride");
_Static_assert(sizeof(gRacePlayerModelPart0DisplayLists) == 24, "model row width");
_Static_assert(sizeof(gRaceGhostPlayerModelPart12DisplayLists) == 24, "ghost row width");
'''


def main():
    with tempfile.TemporaryDirectory(prefix="sk-interface-layouts-") as temp:
        source = Path(temp) / "layouts.c"
        source.write_text(SOURCE)
        subprocess.run([
            "clang", "-target", "mips-unknown-elf", "-std=c11", "-fsyntax-only",
            "-ffreestanding", "-fno-builtin", "-funsigned-char", "-Iinclude",
            "-DLANGUAGE_C", "-D_LANGUAGE_C", "-D_MIPS_SZLONG=32", "-DF3DEX_GBI",
            "-DNDEBUG", str(source),
        ], cwd=ROOT, check=True)
    symbols = {}
    output = subprocess.check_output([
        "mips-linux-gnu-nm", "-n", "build/snowboardkids.elf"
    ], cwd=ROOT, text=True)
    for line in output.splitlines():
        fields = line.split()
        if len(fields) == 3:
            symbols[fields[2]] = int(fields[0], 16)
    expected = {
        "gRaceCameras": 0x801121E0, "gRaceCamerasEnd": 0x801124A0,
        "gControllerPakRumbleCheckPromptTransition": 0x8010ADD0,
        "gControllerPakRumbleCheckPromptState": 0x8010ADD2,
        "gControllerPakRumbleCheckPromptConfirmSelection": 0x8010ADD9,
        "gMenuScratch0": 0x8010ADE0, "gMenuScratch1": 0x8010ADE4,
        "gMenuScratch2": 0x8010ADE8,
        "gModelRenderCallbackQueues": 0x80124898,
        "gModelRenderCallbackList": 0x80124898,
        "gRaceObjectRenderCallbackList": 0x801248A4,
        "gSceneModelRenderCallbackList": 0x801248B0,
        "gRaceModelEffectRenderCallbackList": 0x801248BC,
        "D_801248C8": 0x801248C8, "gEffectRenderCallbackList": 0x801248D4,
        "D_801248E0": 0x801248E0, "D_801248EC": 0x801248EC,
        "gBackdropRenderCallbackList": 0x801248F8,
        "gRspOutputBuffer": 0x80360000, "gRspYieldBuffer": 0x80368000,
        "gRspDramStack": 0x80368C00, "gDepthBuffer": 0x80369000,
        "D_369000": 0x00369000,
    }
    for name, address in expected.items():
        actual = symbols.get(name)
        if actual != address:
            raise SystemExit(f"{name}: expected {address:#x}, got {actual!r}")
    print(f"N64 public layouts and {len(expected)} symbol addresses/aliases verified.")


if __name__ == "__main__":
    main()
