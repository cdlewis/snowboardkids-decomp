#ifndef MAIN_MENU_SCENE_ACTOR_4_H
#define MAIN_MENU_SCENE_ACTOR_4_H

#include "common.h"

typedef struct MainMenuSceneActor4 MainMenuSceneActor4;

typedef struct {
    /* 0x00 */ s32 posX;
    /* 0x04 */ s32 posY;
    /* 0x08 */ s32 posZ;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 actorId;
} MainMenuSceneActorShadow;

void func_80036380(void);
void func_80036388(MainMenuSceneActor4 *arg0);
void func_800363B4(MainMenuSceneActor4 *arg0);
void func_80036418(MainMenuSceneActor4 *arg0);
void func_800364B8(MainMenuSceneActor4 *arg0);
void func_80036520(MainMenuSceneActor4 *arg0);
void func_800365B4(MainMenuSceneActor4 *arg0);
void func_80036674(MainMenuSceneActor4 *arg0);
void func_80036704(MainMenuSceneActor4 *arg0);
void func_800367A8(MainMenuSceneActor4 *arg0);
void func_800368BC(MainMenuSceneActor4 *arg0);
void func_80036930(MainMenuSceneActor4 *arg0);
void func_800369BC(MainMenuSceneActor4 *arg0);
void func_80036A4C(MainMenuSceneActor4 *arg0);
void func_80036AC4(MainMenuSceneActor4 *arg0);
void func_80036B54(MainMenuSceneActor4 *arg0);
void func_80036C14(MainMenuSceneActor4 *arg0);
void func_80036C8C(MainMenuSceneActor4 *arg0);
void func_80036D08(MainMenuSceneActor4 *arg0);
void func_80036DAC(MainMenuSceneActor4 *arg0);
void func_80036E58(MainMenuSceneActor4 *arg0);
void func_80036EBC(MainMenuSceneActor4 *arg0);
void func_80036F6C(MainMenuSceneActor4 *arg0);
void func_80036FB4(MainMenuSceneActor4 *arg0);
void func_800373AC(MainMenuSceneActorShadow *arg0);

#endif
