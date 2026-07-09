#include "viewport_manager.h"

// func_800704F0 best match: 50.342%

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_800704F0.s")

#ifdef NON_MATCHING
void func_800704F0(void) {
    volatile ViewportState *viewport = &D_801124B0[0];

    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[1];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[2];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;

    viewport = &D_801124B0[3];
    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;
}
#endif

void func_80070614(s32 arg0) {
    ViewportState *viewport = &D_801124B0[arg0];

    viewport->active = 0;
    viewport->screenBoundsValid = 0;
    viewport->unk2 = 0;
    viewport->overlayActive = 0;
    viewport->overlayR = 0;
    viewport->overlayG = 0;
    viewport->overlayB = 0;
    viewport->overlayAlpha = 0;
    viewport->unk14 = 0x1FF;
    viewport->unk16 = 0;
    viewport->unk1C = 0x1FF;
    viewport->unk1E = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_8007066C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070A70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/viewport_manager/func_80070C64.s")

void func_80070E58(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ViewportState *viewport = &D_801124B0[arg0];

    viewport->overlayActive = 1;
    viewport->overlayR = arg1;
    viewport->overlayG = arg2;
    viewport->overlayB = arg3;
}

void func_80070E90(s32 arg0) {
    D_801124B0[arg0].unk2 = 1;
}
