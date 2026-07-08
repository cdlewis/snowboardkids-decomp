#ifndef CHARACTER_SELECT_UI_H
#define CHARACTER_SELECT_UI_H

#include "common.h"

typedef struct CharacterSelectWidgetActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ union {
        /* 0x1C */ s16 index;
        struct {
            /* 0x1C */ u8 state;
            /* 0x1D */ u8 pad1D;
        } bytes;
    } sprite;
    union {
        struct {
            /* 0x1E */ u8 state;
            /* 0x1F */ u8 timer;
        } bytes;
        /* 0x1E */ u16 alpha;
        /* 0x1E */ s16 signedAlpha;
    } transition;
    union {
        struct {
            /* 0x20 */ u8 selectedCharacterRow;
            /* 0x21 */ u8 pad21;
        } bytes;
        /* 0x20 */ s16 counter;
    } selection;
    union {
        struct {
            /* 0x22 */ u8 subState;
            /* 0x23 */ u8 subTimer;
        } bytes;
        /* 0x22 */ s16 value;
    } row;
} CharacterSelectWidgetActor;

typedef struct CharacterSelectMenuFrameActor {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 x[11];
    /* 0x2E */ s16 y[13];
    /* 0x48 */ u8 itemCount;
} CharacterSelectMenuFrameActor;

typedef struct CharacterSelectTime {
    /* 0x00 */ s8 minutes;
    /* 0x01 */ s8 seconds;
    /* 0x02 */ s16 centiseconds;
} CharacterSelectTime;

void func_8001BD70(CharacterSelectMenuFrameActor *arg0);
void func_8001C158(CharacterSelectMenuFrameActor *arg0);
void func_8001C83C(CharacterSelectMenuFrameActor *arg0);
void func_8001C96C(CharacterSelectMenuFrameActor *arg0);
void func_8001CC10(CharacterSelectMenuFrameActor *arg0);
void func_8001D254(CharacterSelectMenuFrameActor *arg0);
void func_8001D2F0(CharacterSelectWidgetActor *arg0);
void func_8001D7B8(CharacterSelectWidgetActor *arg0);
void func_8001DACC(CharacterSelectWidgetActor *arg0);
void func_8001DB0C(CharacterSelectWidgetActor *arg0);
void func_8001DD80(CharacterSelectWidgetActor *arg0);
void func_8001DFA0(CharacterSelectWidgetActor *arg0);
void func_8001DFE4(CharacterSelectWidgetActor *arg0);
void func_8001E258(CharacterSelectWidgetActor *arg0);
void func_8001E468(CharacterSelectWidgetActor *arg0);
void func_8001E4AC(CharacterSelectWidgetActor *arg0);
void func_8001E720(CharacterSelectWidgetActor *arg0);
void func_8001E930(CharacterSelectWidgetActor *arg0);
void func_8001E974(CharacterSelectWidgetActor *arg0);
void func_8001EBE8(CharacterSelectWidgetActor *arg0);
void func_8001EDF8(CharacterSelectWidgetActor *arg0);
void func_8001EE3C(CharacterSelectWidgetActor *arg0);
void func_8001F0B0(CharacterSelectWidgetActor *arg0);
void func_8001F2C0(CharacterSelectWidgetActor *arg0);
void func_8001F304(CharacterSelectWidgetActor *arg0);
void func_8001F578(CharacterSelectWidgetActor *arg0);
void func_8001F788(CharacterSelectWidgetActor *arg0);
void func_8001F7CC(CharacterSelectWidgetActor *arg0);
void func_8001FA40(CharacterSelectWidgetActor *arg0);
void func_8001FC50(CharacterSelectWidgetActor *arg0);
void func_8001FC94(CharacterSelectWidgetActor *arg0);
void func_8001FF08(CharacterSelectWidgetActor *arg0);
void func_80020118(CharacterSelectWidgetActor *arg0);
void func_8002015C(CharacterSelectWidgetActor *arg0);
void func_800203D0(CharacterSelectWidgetActor *arg0);
void func_800205E0(CharacterSelectWidgetActor *arg0);
void func_80020624(CharacterSelectWidgetActor *arg0);
void func_80020818(CharacterSelectWidgetActor *arg0);
void func_80020AA0(CharacterSelectWidgetActor *arg0);
void func_80020AE0(CharacterSelectWidgetActor *arg0);
void func_80020B70(CharacterSelectWidgetActor *arg0);
void func_80020CEC(CharacterSelectWidgetActor *arg0);
void func_80020D88(CharacterSelectWidgetActor *arg0);
void func_80020DEC(CharacterSelectWidgetActor *arg0);
void func_80020F44(CharacterSelectWidgetActor *arg0);
void func_80020F80(CharacterSelectWidgetActor *arg0);
void func_8002127C(CharacterSelectWidgetActor *arg0);
void func_800213D4(CharacterSelectWidgetActor *arg0);
void func_80021410(CharacterSelectWidgetActor *arg0);
void func_8002172C(CharacterSelectWidgetActor *arg0);
void func_8002186C(CharacterSelectWidgetActor *arg0);
void func_800218A4(CharacterSelectWidgetActor *arg0);
void func_800219E4(CharacterSelectWidgetActor *arg0);
void func_80021B20(CharacterSelectWidgetActor *arg0);
void func_80021B58(CharacterSelectWidgetActor *arg0);
void func_80021C98(CharacterSelectWidgetActor *arg0);
void func_80021E70(CharacterSelectWidgetActor *arg0);
void func_80021EA8(CharacterSelectWidgetActor *arg0);
void func_80021F80(CharacterSelectWidgetActor *arg0);
void func_8002215C(CharacterSelectWidgetActor *arg0);
void func_80022198(CharacterSelectWidgetActor *arg0);
void func_80022274(CharacterSelectWidgetActor *arg0);
void func_80022464(CharacterSelectWidgetActor *arg0);
void func_800224A0(CharacterSelectWidgetActor *arg0);
void func_8002262C(CharacterSelectWidgetActor *arg0);
void func_800227A0(CharacterSelectWidgetActor *arg0);
void func_800227D8(CharacterSelectWidgetActor *arg0);
void func_80023198(CharacterSelectWidgetActor *arg0);
void func_800232F4(CharacterSelectWidgetActor *arg0);
void func_8002332C(CharacterSelectWidgetActor *arg0);
void func_80023434(CharacterSelectWidgetActor *arg0);
void func_800235E0(CharacterSelectWidgetActor *arg0);
void func_80023618(CharacterSelectTime *arg0, s32 x, s32 y, s32 alpha);

#endif
