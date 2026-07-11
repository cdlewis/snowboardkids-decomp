#include "common.h"
#include "race_course_effects.h"
#include "relocatable_heap.h"
#include "callback_task_scheduler.h"
#include "asset_manager.h"
#include "spatial_math.h"
#include "fixed_point_math.h"
#include "race_course_motion.h"
#include "race_player_movement.h"

#define COURSE_INDEX_RELOAD (*(volatile s16 *)&gRaceCourseIndex)

typedef struct RaceCountdownEffect {
    char pad[0x18];
    s16 step;
    u16 timer;
} RaceCountdownEffect;

typedef struct RacePlayerEffect {
    char pad[0x10];
    u16 playerIndex;
} RacePlayerEffect;

typedef struct {
    s32 words[0x10];
} CourseRenderCommand;

typedef struct RaceCourseRenderEffect {
    char pad0[0x18];
    CourseRenderCommand *vertices;
} RaceCourseRenderEffect;

typedef struct {
    s16 displayListIndex;
    s16 pad2;
    Vec3i position;
    s16 rotation;
    s16 pad12;
} CourseRenderEntry;

typedef struct {
    s8 type;
    char pad1[3];
    Vec3i pos;
} CourseMarkerSpawnEntry;

typedef struct CourseEffectPlayer {
    /* 0x000 */ s16 unk0;
    /* 0x002 */ char pad2[0x11];
    /* 0x013 */ s8 isActive;
    /* 0x014 */ char pad14[8];
    /* 0x01C */ s32 posX;
    /* 0x020 */ char pad20[4];
    /* 0x024 */ s32 posZ;
    /* 0x028 */ char pad28[0x34];
    /* 0x05C */ s32 posY;
    /* 0x060 */ char pad60[0x220];
    /* 0x280 */ s32 collisionRadius;
    /* 0x284 */ char pad284[0x78];
    /* 0x2FC */ s32 flags;
    /* 0x300 */ char pad300[0x32];
    /* 0x332 */ s16 yaw;
    /* 0x334 */ s16 pitch;
    /* 0x336 */ char pad336[0x242];
    /* 0x578 */ s16 unk578;
    /* 0x57A */ char pad57A[0x92];
} CourseEffectPlayer;

typedef struct PatrolCourseObjectEffect {
    char pad[0x18];
    Vec3i pos;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    char pad34[4];
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 pad42;
    s16 displayListValid;
    s16 pad46;
    void *displayList;
    s16 unk4C;
    s16 unk4E;
    s32 unk50;
} PatrolCourseObjectEffect;

typedef struct {
    s16 rotation[9];
    s16 pad2A;
    Vec3i basePos;
} CourseEffectMatrixSource;

typedef struct CourseGateObjectEffect {
    char pad0[0x18];
    CourseEffectMatrixSource source;
    Vec3i pos1;
    Vec3i pos2;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    void *sourceMatrix;
    void *pos1Matrix;
    void *pos2Matrix;
} CourseGateObjectEffect;

typedef struct RaceMovingEffect {
    char pad0[0x18];
    Vec3i velocity;
    Vec3i pos;
    char unk30[0x20];
    s16 timer;
    s16 unk52;
    void *matrix;
} RaceMovingEffect;

typedef struct {
    char pad0[8];
    Vec3i unk8;
    char pad14[0xC];
    Vec3i pos;
    char pad2C[0x1C];
} CourseSpawnEntry;

typedef struct {
    s16 angle;
    s16 unk2;
    char pad4[0x44];
} CourseAngleEntry;

typedef struct RaceCourseMarkerEffect {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *texture;
    void *palette;
    s16 rotation;
    char pad22[2];
    Vtx *vertices;
    Vtx *baseVertices;
    s16 vertexCount;
    char pad2E[2];
    s32 texturePtr;
    s32 palettePtr;
    s32 useAltQueue;
    s32 unk3C;
} RaceCourseMarkerEffect;

typedef struct {
    s32 texturePtr;
    s32 palettePtr;
    void *baseVerticesInput;
    s16 vertexCount;
    s16 flags;
    u16 textureIndex;
    s16 unk12;
} CourseMarkerEntry;

typedef struct {
    void *baseVerticesInput;
    char pad4[0x10];
} CourseMarkerVertexResource;

typedef struct {
    u16 textureIndex;
    char pad2[0x12];
} CourseMarkerTextureResource;

typedef struct {
    s16 scaleX;
    s16 scaleY;
    s16 scaleZ;
    s16 unk6;
    Vec3i pos;
    s16 pitch;
    s16 yaw;
    s32 displayList;
} CourseTriggerEntry;

typedef struct {
    Vec3i dest;
    Vec3i source;
    char mtx[0x20];
    s32 pad38;
} CourseTriggerScratch;

typedef struct RaceCourseTriggerEffect {
    char pad0[0x10];
    u16 entryIndex;
    char pad12[6];
    void *matrix;
    Vec3i pos1;
    Vec3i pos2;
    s16 pitch;
    s16 yaw;
    s32 scaleX;
    s32 scaleY;
    s32 scaleZ;
    s32 displayList;
} RaceCourseTriggerEffect;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s16 angle;
    s16 unkE;
} SoundParams;

typedef struct {
    s16 angle;
    char pad2[0xE];
} SoundParamAngle;

typedef struct {
    char pad0[0x14];
    s16 courseVtxHandle;
    s16 courseTextureHandle;
    char pad18[0x2A];
    s16 markerMatrixHandle;
    s16 courseRenderBufferHandle;
} CourseAssetHandles;

typedef struct {
    char pad0[0x44];
    Vec3i transformOffset;
    char pad50[0x60];
} RaceCamera;

typedef struct RaceCourseBackdropEffect {
    char pad0[0x44];
    void *matrix;
} RaceCourseBackdropEffect;

extern void addRenderCallback(void *, void *, void *);
extern void enqueueSoundEffect(s32, s32);
extern void enqueuePositionalSoundEffect(s32, void *, s32, s32);
extern void osWritebackDCache(void *, s32);
extern void *allocMenuRenderScratch(s32);
extern void func_800486BC(void *, void *);
extern void *allocFixedTransformMatrix(CourseEffectMatrixSource *);
extern void setPackedMatrixTranslation(CourseRenderCommand *, Vec3i *);
extern s32 func_80048E60(Vec3i *);
extern void getAssetTableImageAndPalette(s32, s32, void *, void *);
extern void *resolveAssetTableRelativePointer(void *, u32);
extern void func_80047174(s32, s32, s32, s32, s32);
extern void func_80045A78(s32, s32, s32, s32);
extern void osWritebackDCache(void *, s32);
extern s16 gRaceCommonSpriteAssetHandle;
extern s16 D_80112140;
extern s16 D_80112142;
extern s32 gMenuFlowState;
extern u8 gCurrentViewportIndex;
extern u8 gRenderMatricesDirty;
extern void waitForCourseGateTrigger(CourseGateObjectEffect *);
void renderCourseGateObject(CourseGateObjectEffect *);
void renderCourseTriggerVolume(RaceCourseTriggerEffect *);
void collidePlayerWithCourseTriggerVolume(CourseEffectPlayer *, RaceCourseTriggerEffect *);
void updateCourseTriggerVolume(RaceCourseTriggerEffect *);
void drawRaceCountdownReadyPrompt(RaceCountdownEffect *);
void drawRaceCountdownGoPrompt(RaceCountdownEffect *);
void updateRaceCountdownGoPromptOut(RaceCountdownEffect *);
void updateRaceCountdownGoPromptHold(RaceCountdownEffect *);
void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *);
void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *);
void updateRaceCountdownInitialDelay(RaceCountdownEffect *);
extern void renderCourseBillboardMarker(RaceCourseMarkerEffect *);
void updatePatrolCourseObject(PatrolCourseObjectEffect *);
void initPatrolCourseObject(PatrolCourseObjectEffect *);
void renderRaceCourseSceneryObjects(RaceCourseRenderEffect *);
void renderSpiralCourseObject(RaceMovingEffect *);
extern u8 gRaceUpdatePaused;
extern s16 gRaceCourseIndex;
extern CourseAssetHandles gAssetHandles;
extern s16 D_80112144;
extern s16 D_80112146;
extern RaceCamera D_801121E0[];
extern CourseMarkerSpawnEntry *gCourseTextureMarkerSpawnEntriesByCourse[];
extern CourseRenderEntry *gRaceCourseSceneryEntriesByCourse[];
extern void *gRaceCourseSceneryDisplayLists[];
extern SoundParams gCourseGateSoundParams[];
extern CourseSpawnEntry gRaceCourseStartEntries[];
extern CourseAngleEntry gSpiralCourseObjectAngles[];
extern CourseAngleEntry gLaunchRampCourseObjectAngles[];
extern CourseMarkerEntry gCourseBillboardMarkerEntries[];
extern CourseMarkerVertexResource gCourseBillboardMarkerVertexResources[];
extern CourseMarkerTextureResource gCourseBillboardMarkerTextureResources[];
extern CourseTriggerEntry gCourseTriggerEntries[];
extern Gfx D_2001D00[];
extern SoundParamAngle gCourseGateAngles[];
extern CourseEffectPlayer D_80121D80[];
extern CourseEffectPlayer D_8012238C[];
extern CourseEffectPlayer D_80122998[];
extern CourseEffectPlayer D_80122FA4[];
extern CourseRenderCommand gIdentityMatrix[];
extern s32 gMenuRenderCallbackList;
extern s32 D_80124878;
extern s32 D_801248D4;
extern s32 D_801248EC;
extern s32 D_801248D4;
extern s32 D_801248B0;
extern s32 gRaceObjectRenderCallbackList;
extern s32 D_801248F8;
extern Gfx *gRegionAllocPtr;
extern Gfx D_2001678[];
extern Gfx D_2001730[];
extern Gfx D_2001810[];
extern Gfx D_20018E8[];
extern Gfx D_2000910[];
extern Gfx D_2003218[];
extern Gfx D_20057D8[];
extern Gfx D_2006430[];
extern Gfx D_20067B0[];
extern Gfx D_2008628[];
extern Gfx D_2008900[];
extern Gfx D_20089E0[];
extern Gfx D_2008D50[];
extern Gfx D_2008E30[];
extern Gfx D_2008F80[];
extern Gfx D_200B400[];
extern Gfx D_200B4E0[];
extern Gfx D_200B7B8[];
extern Gfx D_200B8C8[];
extern Gfx D_200BD48[];
extern Gfx D_200BF40[];
extern Gfx D_200C060[];
extern Gfx D_200C238[];
extern Gfx D_200C7A8[];
extern Gfx D_2006548[];
extern Gfx D_2006880[];
extern Gfx D_20058A8[];
void drawRaceCountdownReadyPrompt(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        func_80047174(-0x34, -0xC, getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle), 0x3F, arg0->step);
    } else {
        func_80045A78(-0x34, -0xC, getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle), 0x3F);
    }
}

void drawRaceCountdownGoPrompt(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        func_80047174(-0x20, -0xC, getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle), 0x40, arg0->step);
    } else {
        func_80045A78(-0x20, -0xC, getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle), 0x40);
    }
}

void updateRaceCountdownGoPromptOut(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCountdownGoPrompt, arg0);
}

void updateRaceCountdownGoPromptHold(RaceCountdownEffect *arg0) {
    RaceCountdownEffect *temp_a2 = arg0;

    if (arg0->step != 0) {
        arg0->step--;
    }
    temp_a2->timer--;
    if (temp_a2->timer == 0) {
        gMenuFlowState &= ~1;
        setCallbackTaskCallback(temp_a2, updateRaceCountdownGoPromptOut);
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCountdownGoPrompt, temp_a2);
}

void updateRaceCountdownReadyPromptIn(RaceCountdownEffect *arg0) {
    arg0->step++;
    if (arg0->step == 4) {
        enqueueSoundEffect(0x4C, 0x5A);
        arg0->timer = 0x14;
        setCallbackTaskCallback(arg0, updateRaceCountdownGoPromptHold);
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCountdownReadyPrompt, arg0);
}

void updateRaceCountdownReadyPromptHold(RaceCountdownEffect *arg0) {
    if (arg0->step != 0) {
        arg0->step--;
    }
    arg0->timer--;
    if (arg0->timer == 0) {
        setCallbackTaskCallback(arg0, updateRaceCountdownReadyPromptIn);
    }
    addRenderCallback(&gMenuRenderCallbackList, drawRaceCountdownReadyPrompt, arg0);
}

void updateRaceCountdownInitialDelay(RaceCountdownEffect *arg0) {
    if ((--arg0->timer) == 0) {
        enqueueSoundEffect(0x4B, 0x5A);
        arg0->step = 4;
        arg0->timer = 0x3C;
        setCallbackTaskCallback(arg0, updateRaceCountdownReadyPromptHold);
    }
}

void initRaceCountdownPrompt(RaceCountdownEffect *arg0) {
    arg0->timer = 0x14;
    setCallbackTaskCallback(arg0, updateRaceCountdownInitialDelay);
}

void renderRaceCourseModel(void *arg0) {
    volatile u8 pad[0x30];

    gDPPipeSync(gRegionAllocPtr++);

    gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112140));
    gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112142));

    gSPMatrix(gRegionAllocPtr++, gIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (*(u16 *)&gRaceCourseIndex) {
        case 0:
            gSPDisplayList(gRegionAllocPtr++, D_2008900);
            break;
        case 1:
            gSPDisplayList(gRegionAllocPtr++, D_2008E30);
            break;
        case 2:
            gSPDisplayList(gRegionAllocPtr++, D_2008628);
            break;
        case 3:
            gSPDisplayList(gRegionAllocPtr++, D_200B400);
            break;
        case 4:
            gSPDisplayList(gRegionAllocPtr++, D_200BD48);
            break;
        case 5:
            gSPDisplayList(gRegionAllocPtr++, D_200C060);
            break;
        case 6:
            gSPDisplayList(gRegionAllocPtr++, D_200B7B8);
            break;
        case 7:
            gSPDisplayList(gRegionAllocPtr++, D_2006430);
            break;
        case 8:
            gSPDisplayList(gRegionAllocPtr++, D_20067B0);
            break;
        case 9:
            gSPDisplayList(gRegionAllocPtr++, D_20057D8);
            break;
    }
}

// renderRaceCourseBackdrop best match: 52.371% (base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/renderRaceCourseBackdrop.s")

#ifdef NON_MATCHING
#define EMIT_COURSE_BACKDROP(list)                             \
    do {                                                       \
        Gfx **region = &gRegionAllocPtr;                       \
        CourseAssetHandles *handles = &gAssetHandles;             \
        gfx = *region;                                         \
        *region = gfx + 1;                                     \
        gfx->words.w0 = 0xE7000000;                            \
        gfx->words.w1 = 0;                                     \
        gfx = *region;                                         \
        *region = gfx + 1;                                     \
        gfx->words.w0 = 0xBC000806;                            \
        gfx->words.w1 = (u32)getRelocatableHeapBlockBase(handles->courseVtxHandle);    \
        gfx = *region;                                         \
        *region = gfx + 1;                                     \
        gfx->words.w0 = 0xBC000C06;                            \
        gfx->words.w1 = (u32)getRelocatableHeapBlockBase(handles->courseTextureHandle); \
        gfx = *region;                                         \
        *region = gfx + 1;                                     \
        gfx->words.w0 = 0x01020040;                            \
        gfx->words.w1 = (u32)temp_s3->matrix;                  \
        gfx = *region;                                         \
        *region = gfx + 1;                                     \
        gfx->words.w1 = (u32)(list);                           \
        gfx->words.w0 = 0x06000000;                            \
    } while (0)

void renderRaceCourseBackdrop(RaceCourseBackdropEffect *arg0) {
    CourseEffectMatrixSource sp100;
    RaceCamera *camera;
    RaceCourseBackdropEffect *temp_s3;
    Gfx *gfx;
    volatile u8 pad[0xD0];

    temp_s3 = arg0;
    sp100 = gIdentityFixedTransform;
    camera = &D_801121E0[gCurrentViewportIndex];
    sp100.basePos.x = -camera->transformOffset.x;
    sp100.basePos.y = -camera->transformOffset.y;
    sp100.basePos.z = -camera->transformOffset.z;

    temp_s3->matrix = allocFixedTransformMatrix(&sp100);
    if (temp_s3->matrix != NULL) {
        switch ((u16)gRaceCourseIndex) {
            case 0:
                EMIT_COURSE_BACKDROP(D_20089E0);
                gfx = gRegionAllocPtr++;
                gfx->words.w1 = (u32)D_2008D50;
                gfx->words.w0 = 0x06000000;
                break;
            case 1:
                EMIT_COURSE_BACKDROP(D_2008F80);
                break;
            case 2:
                EMIT_COURSE_BACKDROP(D_2008790);
                break;
            case 3:
                EMIT_COURSE_BACKDROP(D_200B4E0);
                break;
            case 4:
                EMIT_COURSE_BACKDROP(D_200BF40);
                break;
            case 5:
                EMIT_COURSE_BACKDROP(D_200C238);
                gfx = gRegionAllocPtr++;
                gfx->words.w1 = (u32)D_200C7A8;
                gfx->words.w0 = 0x06000000;
                break;
            case 6:
                EMIT_COURSE_BACKDROP(D_200B8C8);
                break;
            case 7:
                EMIT_COURSE_BACKDROP(D_2006548);
                break;
            case 8:
                EMIT_COURSE_BACKDROP(D_2006880);
                break;
            case 9:
                EMIT_COURSE_BACKDROP(D_20058A8);
                break;
        }
    }
}
#endif

void updateRaceCourseModelRenderTask(void *arg0) {
    addRenderCallback(&gRaceObjectRenderCallbackList, renderRaceCourseModel, arg0);
    addRenderCallback(&D_801248F8, renderRaceCourseBackdrop, arg0);
}

void initRaceCourseModelRenderTask(void *arg0) {
    setCallbackTaskCallback(arg0, updateRaceCourseModelRenderTask);
}

void drawFinalLapPromptForViewport(RacePlayerEffect *arg0) {
    if (gCurrentViewportIndex == arg0->playerIndex) {
        func_80045A78(-0x30, -0xC, getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle), 0x41);
    }
}

void updateFinalLapPrompt(void *arg0) {
    if (gMenuFlowState & 8) {
        removeCallbackTask(arg0);
        return;
    }
    addRenderCallback(&D_80124878, drawFinalLapPromptForViewport, arg0);
}

void initFinalLapPrompt(void *arg0) {
    enqueueSoundEffect(0x52, 0x5A);
    setCallbackTaskCallback(arg0, updateFinalLapPrompt);
}

// renderCourseTextureMarkers best match: 99.693% (nonmatchings/renderCourseTextureMarkers-7892263622508053986/base_5.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/renderCourseTextureMarkers.s")

#ifdef NON_MATCHING
extern void getAssetTableImagePaletteAndSize(u8 *, u16, void **, void **, s16 *, s16 *);
extern Vtx D_800D9C40[];
extern Gfx gEffectRenderModeSetupDl[];
extern Gfx gEffectRenderModeCleanupDl[];
extern u32 gViewportMatrix;

void renderCourseTextureMarkers(RaceCourseRenderEffect *arg0) {
    volatile u8 pad[8];
    void *image;
    void *palette;
    s16 width;
    s16 height;
    CourseMarkerSpawnEntry *entry;
    s16 textureIndex;
    s32 i;
    s8 nextType;

    textureIndex = -1;
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeSetupDl);
    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex];
    i = 0;
    if (entry->type != -1) {
        do {
            if (isPositionNearCurrentViewport(&entry->pos) != 0) {
                if (entry->type != textureIndex) {
                    textureIndex = entry->type;
                    getAssetTableImagePaletteAndSize((u8 *)getRelocatableHeapBlockBase((s32)gRaceCommonSpriteAssetHandle), textureIndex & 0xFFFF,
                                  &image, &palette, &width, &height);
                    gDPLoadTextureBlock_4b(gRegionAllocPtr++, image, G_IM_FMT_CI, width, height, 0,
                                            G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                            G_TX_NOLOD, G_TX_NOLOD);
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, palette);
                }
                gSPMatrix(gRegionAllocPtr++, &arg0->vertices[i], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPMatrix(gRegionAllocPtr++, gViewportMatrix, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                {
                    Gfx *_g = gRegionAllocPtr++;
                    _g->words.w0 = 0x0400103F;
                    _g->words.w1 = (u32)&D_800D9C40[entry->type * 4];
                }
                {
                    Gfx *_g = gRegionAllocPtr++;
                    _g->words.w1 = 0x60200;
                    _g->words.w0 = 0xB1060402;
                }
            }
            nextType = entry[1].type;
            entry++;
            i++;
        } while (-1 != nextType);
    }
    gSPDisplayList(gRegionAllocPtr++, gEffectRenderModeCleanupDl);
}
#endif

void updateCourseTextureMarkers(void *arg0) {
    CourseMarkerSpawnEntry *entry;
    s8 type;

    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex];
    if (entry->type != -1) {
        do {
            switch (entry->type) {
                case 0:
                case 1:
                    func_80088294(&entry->pos, 0xC0000, 0x600000, 2);
                    break;
                case 2:
                    func_80088A1C(&entry->pos, 0x20000, 0x40000, 0x30000, 4);
                    break;
            }
            type = entry[1].type;
            entry++;
        } while (entry->type != -1);
    }

    addRenderCallback(&D_801248D4, renderCourseTextureMarkers, arg0);
}

void initCourseTextureMarkers(RaceCourseRenderEffect *arg0) {
    CourseMarkerSpawnEntry *entry;
    s32 count;
    s32 allocSize;
    s32 i;

    entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex];
    count = 0;
    if (entry->type != -1) {
        do {
            count++;
            entry++;
        } while (entry->type != -1);
    }

    if (count != 0) {
        entry = gCourseTextureMarkerSpawnEntriesByCourse[gRaceCourseIndex];
        allocSize = count * sizeof(CourseRenderCommand);
        gAssetHandles.markerMatrixHandle = allocRelocatableHeapBlock(allocSize);
        arg0->vertices = getRelocatableHeapBlockBase(gAssetHandles.markerMatrixHandle);

        i = 0;
        if (count > 0) {
            do {
                arg0->vertices[i] = gIdentityMatrix[0];
                setPackedMatrixTranslation(&arg0->vertices[i], &entry->pos);
                i++;
                entry++;
            } while (i != count);
        }
        osWritebackDCache(arg0->vertices, allocSize);
    }

    setCallbackTaskCallback(arg0, updateCourseTextureMarkers);
}

void renderRaceCourseSceneryObjects(RaceCourseRenderEffect *arg0) {
    CourseRenderEntry *var_s4;
    s32 var_s5;
    s32 var_s7;
    Gfx *temp_s0;
    Gfx *temp_s2;
    Gfx *temp_s3;

    var_s4 = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex];
    var_s7 = TRUE;
    var_s5 = 0;
    if (var_s4->displayListIndex != -1) {
        do {
            if (isPositionNearCurrentViewport(&var_s4->position) != 0) {
                if (var_s7 != 0) {
                    gDPPipeSync(gRegionAllocPtr++);
                    temp_s2 = gRegionAllocPtr++;
                    var_s7 = FALSE;
                    gSPSegment(temp_s2, 0x02, getRelocatableHeapBlockBase(gAssetHandles.courseVtxHandle));

                    temp_s3 = gRegionAllocPtr++;
                    gSPSegment(temp_s3, 0x03, getRelocatableHeapBlockBase(gAssetHandles.courseTextureHandle));
                }

                temp_s0 = gRegionAllocPtr++;
                gDma1p(temp_s0, 1, (u32)arg0->vertices + (var_s5 << 6), 0x40, 2);

                temp_s0 = gRegionAllocPtr++;
                gSPDisplayList(temp_s0, gRaceCourseSceneryDisplayLists[var_s4->displayListIndex]);
            }
            var_s4++;
            var_s5++;
        } while (var_s4->displayListIndex != -1);
    }
}

void updateRaceCourseSceneryObjects(void *arg0) {
    addRenderCallback(&D_801248B0, renderRaceCourseSceneryObjects, arg0);
}

void initRaceCourseSceneryObjects(RaceCourseRenderEffect *arg0) {
    s32 size;
    CourseRenderEntry *base;
    CourseRenderEntry *entry;
    s32 i;
    CourseEffectMatrixSource transform;
    s32 count;

    base = gRaceCourseSceneryEntriesByCourse[gRaceCourseIndex];
    count = 0;
    entry = base;
    if (base->displayListIndex != -1) {
        do {
            count++;
            entry++;
        } while (entry->displayListIndex != -1);
    }

    if (count != 0) {
        entry = base;
        size = count << 6;
        gAssetHandles.courseRenderBufferHandle = allocRelocatableHeapBlock(size);
        arg0->vertices = (void *)getRelocatableHeapBlockBase(gAssetHandles.courseRenderBufferHandle);

        for (i = 0; i < count; i++) {
            makeFixedRotationY(&transform, entry->rotation);
            transform.basePos.x = entry->position.x;
            transform.basePos.y = entry->position.y;
            transform.basePos.z = entry->position.z;
            func_800486BC(&transform, (void *)((u32)arg0->vertices + (i << 6)));
            entry++;
        }

        osWritebackDCache(arg0->vertices, size);
    }
    setCallbackTaskCallback(arg0, updateRaceCourseSceneryObjects);
}

// renderPatrolCourseObject best match: 99.182% at nonmatchings/renderPatrolCourseObject-731940616440357983/angle_5.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/renderPatrolCourseObject.s")

#ifdef NON_MATCHING
void renderPatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    s32 sine;
    s32 doubleSine;
    CourseEffectMatrixSource transform;
    volatile s32 pad0[18];

    if (gRenderMatricesDirty != 0) {
        arg0->displayListValid = 0;
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->displayListValid == 0) {
            arg0->displayListValid = 1;
            sine = fixedSine(arg0->unk40);
            if (1) {
                doubleSine = fixedSine((s16)(arg0->unk40 * 2));
                makeFixedRotationY(transform.rotation, (sine >> 4) + (0x800 + arg0->unk3E));
                transform.basePos.x = arg0->pos.x;
                transform.basePos.y = (arg0->pos.y + ((doubleSine + 0x1000) << 4)) + 0xA4000;
            }
            transform.basePos.z = arg0->pos.z;
            scaleFixedMatrix3sByQuarter(&transform);
            arg0->displayList = allocFixedTransformMatrix(&transform);
        }

        if (((&transform) && (&transform)) && (&transform)) {
        }

        if (arg0->displayList != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112146));
            {
                Gfx *_g = (Gfx *) (gRegionAllocPtr++);

                _g->words.w0 = (unsigned int) ((((unsigned int) ((0x00 | 0x02) | 0x00)) & ((0x01 << 8) - 1)) << 16);
                _g->words.w0 = (((unsigned int) ((((unsigned int) 1) & ((0x01 << 8) - 1)) << 24)) | _g->words.w0) | ((unsigned int) ((((unsigned int) (sizeof(Mtx))) & ((0x01 << 16) - 1)) << 0));
                _g->words.w1 = (unsigned int) arg0->displayList;
            }
            gSPDisplayList(gRegionAllocPtr++, D_2001D00);
        }
    }
}
#endif

// updatePatrolCourseObject best match: 97.016% at nonmatchings/updatePatrolCourseObject-3836525038718587862/base_6.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/updatePatrolCourseObject.s")

#ifdef NON_MATCHING
void updatePatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    Vec3i *pos;
    s16 temp_a1;
    s16 temp_t2;
    s16 temp_v0;
    s16 rand;
    s32 targetAngle;
    s32 var_v1;
    s32 dx;
    s32 dz;

    if (gRaceUpdatePaused == 0) {
        pos = &arg0->pos;
        if (func_80048E60(pos) != 0) {
            if (arg0->pad42 != 0) {
                targetAngle = calculateAngleBetweenXZPoints(arg0->pos.x, arg0->pos.z, arg0->unk24, arg0->unk2C);
            } else {
                targetAngle = calculateAngleBetweenXZPoints(arg0->pos.x, arg0->pos.z, arg0->unk30, arg0->unk38);
            }
            temp_a1 = arg0->unk3E;
            temp_t2 = (targetAngle - temp_a1) & 0xFFF;
            var_v1 = temp_t2;
            if (temp_t2 >= 0x801) {
                var_v1 = (s16)(temp_t2 - 0x1000);
            }
            temp_v0 = arg0->unk4C;
            if (temp_v0 < var_v1) {
                var_v1 = (s16)(s32)temp_v0;
            }
            if (var_v1 < -temp_v0) {
                var_v1 = (s16)-temp_v0;
            }
            arg0->unk3E = temp_a1 + var_v1;
            arg0->pos.x += fixedSine(arg0->unk3E) * ((s32)-arg0->unk50 / 4096);
            dz = arg0->pos.z + (fixedCosine(arg0->unk3E) * ((s32)-arg0->unk50 / 4096));
            arg0->pos.z = dz;
            arg0->unk3C = findRaceCourseSurfaceFromPoint(arg0->unk3C, arg0->pos.x, dz);
            arg0->pos.y = getRaceCourseSurfaceHeight(arg0->unk3C, arg0->pos.x, arg0->pos.z);
            if (arg0->pad42 != 0) {
                dx = arg0->pos.x - arg0->unk24;
                dz = arg0->pos.z - arg0->unk2C;
                if (dx < 0) {
                    dx = -dx;
                }
                if (dz < 0) {
                    dz = -dz;
                }
                if (dx < 0x30000) {
                    if (dz < 0x30000) {
                        arg0->pad42 = 0;
                    }
                }
            } else {
                dx = arg0->pos.x - arg0->unk30;
                dz = arg0->pos.z - arg0->unk38;
                if (dx < 0) {
                    dx = -dx;
                }
                if (dz < 0) {
                    dz = -dz;
                }
                if ((dx < 0x30000) && (dz < 0x30000)) {
                    arg0->pad42 = 1;
                }
            }
            func_80088A1C(pos, 0x40000, 0x50000, 0x30000, 4);
            arg0->unk40 += arg0->unk4E;
            if (arg0->unk40 == 0) {
                rand = randomNextSecondary() & 3;
                if (rand == 1) {
                    arg0->unk50 = 0x10000;
                    arg0->unk4E = 0x80;
                    arg0->unk4C = 0x10;
                }
                if (rand == 2) {
                    arg0->unk50 = 0x20000;
                    arg0->unk4E = 0x100;
                    arg0->unk4C = 0x20;
                }
                if (rand == 3) {
                    arg0->unk50 = 0x40000;
                    arg0->unk4E = 0x200;
                    arg0->unk4C = 0x40;
                }
            }
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderPatrolCourseObject, arg0);
}
#endif

// initPatrolCourseObject best match: 98.684% at nonmatchings/initPatrolCourseObject-5821324921387846781/base.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/initPatrolCourseObject.s")

#ifdef NON_MATCHING
void initPatrolCourseObject(PatrolCourseObjectEffect *arg0) {
    s32 temp24;
    s32 temp28;
    s32 temp2C;
    s16 angle;
    s16 temp4C;
    s32 temp50;
    s16 temp4E;

    if (gRaceUpdatePaused == 0) {
        angle = calculateAngleBetweenXZPoints(arg0->unk24, arg0->unk2C, arg0->unk30, arg0->unk38);
        temp24 = arg0->unk24;
        temp28 = arg0->unk28;
        temp2C = arg0->unk2C;
        arg0->pos.x = temp24;
        temp4C = 0x10;
        temp50 = 0x10000;
        temp4E = 0x80;
        arg0->unk3E = angle;
        arg0->unk4C = temp4C;
        arg0->unk50 = temp50;
        arg0->unk4E = temp4E;
        arg0->pos.y = temp28;
        arg0->pos.z = temp2C;
        arg0->pos.y = getRaceCourseSurfaceHeight(arg0->unk3C, arg0->pos.x, arg0->pos.z);
        setCallbackTaskCallback(arg0, updatePatrolCourseObject);
    }
}
#endif

void spawnPatrolCourseObject(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    PatrolCourseObjectEffect *p = createCallbackTask(initPatrolCourseObject, 0, 0x64);
    if (p != 0) {
        p->unk24 = arg1;
        p->unk2C = arg2;
        p->unk30 = arg3;
        p->unk38 = arg4;
        p->unk3C = arg0;
    }
}

void renderLaunchRampCourseObject(RaceMovingEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    volatile s32 pad[1];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(&transform, gLaunchRampCourseObjectAngles[gRaceCourseIndex].angle + 0x400);
        transform.basePos.x = arg0->pos.x;
        transform.basePos.y = arg0->pos.y;
        transform.basePos.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (arg0->matrix != NULL) {
        if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
            Gfx *_g;

            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (gRaceCourseIndex != 8) {
                gSPDisplayList(gRegionAllocPtr++, D_2000910);
            } else {
                gSPDisplayList(gRegionAllocPtr++, D_2003218);
            }
        }
    }
}

void updateLaunchRampCourseObjectExit(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    s32 temp_v1;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            temp_v1 = arg0->velocity.z;
            arg0->timer = temp_v0 - 1;
            if (temp_v1 >= (s32)0xFFF60001) {
                arg0->velocity.z = temp_v1 - 0x2000;
            }
            transformVec3iByFixedMatrix(&arg0->unk30, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, renderLaunchRampCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateLaunchRampCourseObjectArc(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    void *mtx;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        if (arg0->timer < 0x13) {
            arg0->velocity.z = 0xFFFC0000;
        }

        mtx = arg0->unk30;
        transformVec3iByFixedMatrix(mtx, &arg0->velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, updateLaunchRampCourseObjectExit);
            makeFixedRotationXY(mtx, 0x100, gLaunchRampCourseObjectAngles[gRaceCourseIndex].angle + 0x400);
            arg0->timer = 0x64;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, renderLaunchRampCourseObject, arg0);
}

void initLaunchRampCourseObject(RaceMovingEffect *arg0) {
    void *mtx;

    arg0->timer = 0x46;
    arg0->velocity.z = 0x680000;
    mtx = arg0->unk30;
    makeFixedRotationY(mtx, gLaunchRampCourseObjectAngles[gRaceCourseIndex].angle + 0x400);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.z = 0xFFFE0000;
    arg0->pos.x += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.x;
    arg0->pos.y += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.y;
    arg0->pos.z += gRaceCourseStartEntries[COURSE_INDEX_RELOAD].pos.z;
    setCallbackTaskCallback(arg0, updateLaunchRampCourseObjectArc);
}

void renderSpiralCourseObject(RaceMovingEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    volatile s32 pad[2];

    if (gRenderMatricesDirty != 0) {
        makeFixedRotationY(&transform, arg0->unk52);
        transform.basePos.x = arg0->pos.x;
        transform.basePos.y = arg0->pos.y;
        transform.basePos.z = arg0->pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (isPositionNearCurrentViewport(&arg0->pos) != 0) {
        if (arg0->matrix != NULL) {
            gDPPipeSync(gRegionAllocPtr++);
            gSPSegment(gRegionAllocPtr++, 0x02, getRelocatableHeapBlockBase(D_80112144));
            gSPSegment(gRegionAllocPtr++, 0x03, getRelocatableHeapBlockBase(D_80112146));
            gSPMatrix(gRegionAllocPtr++, arg0->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            if (gRaceCourseIndex != 8) {
                gSPDisplayList(gRegionAllocPtr++, D_2000910);
            } else {
                gSPDisplayList(gRegionAllocPtr++, D_2003218);
            }
        }
    }
}

void updateSpiralCourseObjectExit(RaceMovingEffect *arg0) {
    Vec3i sp24;
    s16 temp_v0;
    RaceMovingEffect *temp_a3 = arg0;

    temp_v0 = arg0->timer;
    if (temp_v0 != 0) {
        if (gRaceUpdatePaused == 0) {
            arg0->timer = temp_v0 - 1;
            transformVec3iByFixedMatrix(&arg0->unk30, &temp_a3->velocity, &sp24);
            temp_a3->pos.x += sp24.x;
            temp_a3->pos.y += sp24.y;
            temp_a3->pos.z += sp24.z;
        }
        addRenderCallback(&gRaceObjectRenderCallbackList, renderSpiralCourseObject, temp_a3);
        return;
    }
    removeCallbackTask(temp_a3);
}

void updateSpiralCourseObjectTurn(RaceMovingEffect *arg0) {
    Vec3i sp2C;
    void *velocity;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        arg0->unk52 -= 0x2A;
        makeFixedRotationY(arg0->unk30, arg0->unk52);

        velocity = &arg0->velocity;
        transformVec3iByFixedMatrix(arg0->unk30, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        arg0->unk52 -= 0x2A;
        makeFixedRotationY(arg0->unk30, arg0->unk52);
        transformVec3iByFixedMatrix(arg0->unk30, velocity, &sp2C);
        arg0->pos.x += sp2C.x;
        arg0->pos.y += sp2C.y;
        arg0->pos.z += sp2C.z;

        if (arg0->timer == 0) {
            setCallbackTaskCallback(arg0, updateSpiralCourseObjectExit);
            arg0->timer = 0x38;
        }
    }

    addRenderCallback(&gRaceObjectRenderCallbackList, renderSpiralCourseObject, arg0);
}

void updateSpiralCourseObjectLaunch(RaceMovingEffect *arg0) {
    Vec3i sp1C;
    RaceMovingEffect *temp_a3 = arg0;

    if (gRaceUpdatePaused == 0) {
        arg0->timer--;
        transformVec3iByFixedMatrix(&arg0->unk30, &temp_a3->velocity, &sp1C);
        temp_a3->pos.x += sp1C.x * 2;
        temp_a3->pos.y += sp1C.y * 2;
        temp_a3->pos.z += sp1C.z * 2;
        if (temp_a3->timer == 0) {
            setCallbackTaskCallback(temp_a3, updateSpiralCourseObjectTurn);
            temp_a3->timer = 0x18;
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderSpiralCourseObject, temp_a3);
}

void initSpiralCourseObject(RaceMovingEffect *arg0) {
    void *mtx;

    arg0->timer = 0x28;
    arg0->unk52 = gSpiralCourseObjectAngles[gRaceCourseIndex].angle;
    arg0->velocity.x = -0x200000;
    arg0->velocity.z = 0x400000;
    mtx = arg0->unk30;
    makeFixedRotationY(mtx, arg0->unk52);
    transformVec3iByFixedMatrix(mtx, &arg0->velocity, &arg0->pos);
    arg0->velocity.x = 0;
    arg0->velocity.z = -0x20000;
    arg0->pos.x += gRaceCourseStartEntries[gRaceCourseIndex].unk8.x;
    arg0->pos.y += gRaceCourseStartEntries[gRaceCourseIndex].unk8.y + 0x40000;
    arg0->pos.z += gRaceCourseStartEntries[gRaceCourseIndex].unk8.z;
    setCallbackTaskCallback(arg0, updateSpiralCourseObjectLaunch);
    updateSpiralCourseObjectLaunch(arg0);
}

// renderCourseGateObject best match: 99.414% (nonmatchings/renderCourseGateObject-6182772958467082306/base_6.c)
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/renderCourseGateObject.s")

#ifdef NON_MATCHING
void renderCourseGateObject(CourseGateObjectEffect *arg0) {
    CourseEffectMatrixSource scratch;
    volatile s32 pad[2];
    void *matrix;
    Gfx *segment1;
    Gfx *segment2;

    if (gRenderMatricesDirty != 0) {
        arg0->sourceMatrix = NULL;
        arg0->pos1Matrix = NULL;
        arg0->pos2Matrix = NULL;
    }

    if (isPositionNearCurrentViewport((Vec3i *) &gCourseGateSoundParams[gRaceCourseIndex]) == 0) {
        return;
    }

    matrix = arg0->sourceMatrix;
    if (matrix == NULL) {
        arg0->sourceMatrix = allocFixedTransformMatrix(&arg0->source);
        matrix = arg0->sourceMatrix;
    }

    if (matrix != NULL) {
        gDPPipeSync(gRegionAllocPtr++);
        segment1 = gRegionAllocPtr++;
        segment1->words.w0 = 0xBC000806;
        segment1->words.w1 = getRelocatableHeapBlockBase(D_80112144);
        segment2 = gRegionAllocPtr++;
        segment2->words.w0 = 0xBC000C06;
        segment2->words.w1 = getRelocatableHeapBlockBase(D_80112146);
        arg0++;
        arg0--;
        gSPMatrix(gRegionAllocPtr++, arg0->sourceMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_2001678);
    }

    matrix = arg0->pos1Matrix;
    if (matrix == NULL) {
        makeFixedRotationZY(scratch.rotation, gCourseGateAngles[gRaceCourseIndex].angle, arg0->unk50);
        scratch.basePos.x = arg0->pos1.x;
        scratch.basePos.y = arg0->pos1.y;
        scratch.basePos.z = arg0->pos1.z;
        arg0->pos1Matrix = allocFixedTransformMatrix(&scratch);
        matrix = arg0->pos1Matrix;
    }

    if (matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, arg0->pos1Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gRegionAllocPtr++, D_2001730);
    }

    matrix = arg0->pos2Matrix;
    if (matrix == NULL) {
        scratch = arg0->source;
        scratch.basePos.x = arg0->pos2.x;
        scratch.basePos.y = arg0->pos2.y;
        scratch.basePos.z = arg0->pos2.z;
        arg0->pos2Matrix = allocFixedTransformMatrix(&scratch);
        matrix = arg0->pos2Matrix;
    }

    if (matrix != NULL) {
        gSPMatrix(gRegionAllocPtr++, arg0->pos2Matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (arg0->unk56 == 0) {
            gSPDisplayList(gRegionAllocPtr++, D_2001810);
        } else {
            gSPDisplayList(gRegionAllocPtr++, D_20018E8);
        }
    }
}
#endif

void updateCourseGateClosing(CourseGateObjectEffect *arg0) {
    if (gRaceUpdatePaused == 0) {
        if (arg0->unk50 != 0) {
            arg0->unk50 += 0x80;
        } else {
            arg0->unk56 = 0;
            setCallbackTaskCallback(arg0, waitForCourseGateTrigger);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderCourseGateObject, arg0);
}

void updateCourseGateOpening(CourseGateObjectEffect *arg0) {
    CourseGateObjectEffect *temp_s0 = arg0;
    s16 temp_v0;

    if (gRaceUpdatePaused == 0) {
        temp_v0 = arg0->unk50;
        if (temp_v0 != -0x400) {
            arg0->unk50 = temp_v0 - 0x40;
        }
        temp_s0->unk54--;
        if (temp_s0->unk54 == 0) {
            enqueuePositionalSoundEffect(0x1C, &gCourseGateSoundParams[gRaceCourseIndex], 0x7F, 0x32);
            setCallbackTaskCallback(temp_s0, updateCourseGateClosing);
        }
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderCourseGateObject, temp_s0);
}

void waitForCourseGateTrigger(CourseGateObjectEffect *arg0) {
    if ((gRaceUpdatePaused == 0) && (gMenuFlowState & 4)) {
        arg0->unk54 = 0x2D;
        gMenuFlowState &= ~4;
        setCallbackTaskCallback(arg0, updateCourseGateOpening);
        arg0->unk56 = 1;
        enqueuePositionalSoundEffect(0x16, &gCourseGateSoundParams[gRaceCourseIndex], 0x7F, 0x32);
        enqueuePositionalSoundEffect(0x1B, &gCourseGateSoundParams[gRaceCourseIndex], 0x7F, 0x32);
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderCourseGateObject, arg0);
}

void initCourseGateObject(CourseGateObjectEffect *arg0) {
    void *mtx;
    Vec3i sp28;

    mtx = arg0->source.rotation;
    makeFixedRotationY(mtx, gCourseGateSoundParams[gRaceCourseIndex].angle);
    arg0->source.basePos.x = gCourseGateSoundParams[gRaceCourseIndex].x;
    arg0->source.basePos.y = gCourseGateSoundParams[gRaceCourseIndex].y;
    arg0->source.basePos.z = gCourseGateSoundParams[gRaceCourseIndex].z;
    sp28.x = 0x18000;
    sp28.y = 0x120000;
    sp28.z = -0x80000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->pos1);
    arg0->pos1.x += arg0->source.basePos.x;
    arg0->pos1.y += arg0->source.basePos.y;
    arg0->pos1.z += arg0->source.basePos.z;
    sp28.x = 0;
    sp28.y = 0x120000;
    sp28.z = 0x50000;
    transformVec3iByFixedMatrix(mtx, &sp28, &arg0->pos2);
    arg0->pos2.x += arg0->source.basePos.x;
    arg0->pos2.y += arg0->source.basePos.y;
    arg0->pos2.z += arg0->source.basePos.z;
    arg0->unk52 = 0;
    setCallbackTaskCallback(arg0, waitForCourseGateTrigger);
}

// renderCourseBillboardMarker best match: 99.603% at nonmatchings/renderCourseBillboardMarker-2/output-129-1/source.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/renderCourseBillboardMarker.s")

#ifdef NON_MATCHING
void renderCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    Gfx *gfx;
    s32 i;
    s16 vertexCount;
    volatile s32 pad[4];

    if (gRenderMatricesDirty != 0) {
        arg0->vertices = allocMenuRenderScratch(arg0->vertexCount * sizeof(Vtx));
        if (arg0->vertices != NULL) {
            i = 0;
            if (arg0->vertexCount > 0) {
                do {
                    arg0->vertices[i] = arg0->baseVertices[i];
                    arg0->vertices[i].v.tc[1] += arg0->rotation;
                    arg0->vertices[i].v.tc[1] = arg0->vertices[i].v.tc[1];
                    i++;
                } while ((i < arg0->vertexCount) != 0);
            }
        }
    }

    do { if (arg0->vertices != NULL) { gfx = gRegionAllocPtr++; gfx->words.w0 = 0xE7000000; vertexCount = (unsigned int) ((((unsigned int) ((((1 << 11) + MAX(1, 0x20 / 16)) - 1) / MAX(1, 0x20 / 16))) & ((0x01 << 12) - 1)) << 0); gfx->words.w1 = 0; gfx = gRegionAllocPtr++; gfx->words.w0 = 0xBC000806; gfx->words.w1 = getRelocatableHeapBlockBase(D_80112140); gfx = gRegionAllocPtr++; gfx->words.w0 = 0x01020040; gfx->words.w1 = (u32) gIdentityMatrix; gfx = gRegionAllocPtr++; gfx->words.w0 = 0x06000000; gfx->words.w1 = (u32) arg0->texturePtr; { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = ((((unsigned int) ((((unsigned int) 0xfd) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (1 - 1)) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (unsigned int) arg0->texture; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 6) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 5) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe6) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) 0xf3) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = ((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) MIN((((0x20 * 0x40) + 3) >> 2) - 1, 2047)) & ((0x01 << 12) - 1)) << 12)); _g->words.w1 = _g->words.w1 | ((unsigned int) ((((unsigned int) ((((1 << 11) + MAX(1, 0x20 / 16)) - 1) / MAX(1, 0x20 / 16))) & ((0x01 << 12) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); dummy_label_166469: ; ; ; _g->words.w0 = (unsigned int) ((((unsigned int) 0xe7) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (((0x20 >> 1) + 7) >> 3)) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 6) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 5) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((unsigned int) ((((unsigned int) 0xf2) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) ((0x20 - 1) << 2)) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) ((0x40 - 1) << 2)) & ((0x01 << 12) - 1)) << 0)); } } ; { { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = ((((unsigned int) ((((unsigned int) 0xfd) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 2) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (1 - 1)) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (unsigned int) arg0->palette; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe8) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (((((unsigned int) ((((unsigned int) 0xf5) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 21))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 19))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 9) - 1)) << 9))) | ((unsigned int) ((((unsigned int) (256 + ((0 & 0xf) * 16))) & ((0x01 << 9) - 1)) << 0)); _g->words.w1 = ((((((((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 20))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 18))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 14))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 10))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 2) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 4))) | ((unsigned int) ((((unsigned int) 0) & ((0x01 << 4) - 1)) << 0)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe6) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xf0) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = ((unsigned int) ((((unsigned int) 7) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) 15) & ((0x01 << 10) - 1)) << 14)); } ; { Gfx *_g = (Gfx *) (gRegionAllocPtr++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe7) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = 0; } } ; gfx = gRegionAllocPtr++; vertexCount = arg0->vertexCount; gfx->words.w0 = (((vertexCount << 0xA) | ((vertexCount << 4) - 1)) & 0xFFFF) | 0x04000000; gfx->words.w1 = (u32) arg0->vertices; gfx = gRegionAllocPtr++; gfx->words.w0 = 0x06000000; gfx->words.w1 = (u32) arg0->palettePtr; } } while (0);
}
#endif

void updateCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    arg0->rotation -= 0x40;
    arg0->rotation &= 0x7FF;
    if (arg0->useAltQueue != 0) {
        addRenderCallback(&D_801248EC, renderCourseBillboardMarker, arg0);
    } else {
        addRenderCallback(&gRaceObjectRenderCallbackList, renderCourseBillboardMarker, arg0);
    }
}

void initCourseBillboardMarker(RaceCourseMarkerEffect *arg0) {
    getAssetTableImageAndPalette(getRelocatableHeapBlockBase(gRaceCommonSpriteAssetHandle),
                  gCourseBillboardMarkerTextureResources[arg0->entryIndex].textureIndex,
                  &arg0->texture, &arg0->palette);
    arg0->baseVertices =
        (Vtx *) resolveAssetTableRelativePointer(getRelocatableHeapBlockBase(D_80112140), (s32) gCourseBillboardMarkerVertexResources[arg0->entryIndex].baseVerticesInput);

    {
        CourseMarkerEntry *entry = &gCourseBillboardMarkerEntries[arg0->entryIndex];

        arg0->vertexCount = entry->vertexCount;
        arg0->texturePtr = entry->texturePtr;
        arg0->palettePtr = entry->palettePtr;
        arg0->vertexCount = entry->vertexCount;
        arg0->useAltQueue = entry->flags & 1;
        arg0->unk3C = entry->flags & 2;
    }
    setCallbackTaskCallback(arg0, updateCourseBillboardMarker);
}

void renderCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    volatile s32 unused;
    CourseEffectMatrixSource transform;
    Gfx *gfx;

    if (gRenderMatricesDirty != 0) {
        CourseTriggerEntry *entry = &gCourseTriggerEntries[((volatile RaceCourseTriggerEffect *) arg0)->entryIndex];

        makeFixedRotationXY(&transform, entry->pitch, entry->yaw);
        transform.basePos.x = gCourseTriggerEntries[arg0->entryIndex].pos.x;
        transform.basePos.y = gCourseTriggerEntries[arg0->entryIndex].pos.y;
        transform.basePos.z = gCourseTriggerEntries[arg0->entryIndex].pos.z;
        arg0->matrix = allocFixedTransformMatrix(&transform);
    }

    if (arg0->matrix != NULL) {
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xE7000000;
            _g->words.w1 = 0;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0xBC000806;
            _g->words.w1 = getRelocatableHeapBlockBase(D_80112140);
        }
        {
            Gfx *_g = gRegionAllocPtr++;
            volatile s32 pad[2];

            _g->words.w0 = 0xBC000C06;
            _g->words.w1 = getRelocatableHeapBlockBase(D_80112142);
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x01020040;
            _g->words.w1 = (u32) arg0->matrix;
        }
        {
            Gfx *_g = gRegionAllocPtr++;

            _g->words.w0 = 0x06000000;
            _g->words.w1 = (u32) arg0->displayList;
        }
    }
}

// collidePlayerWithCourseTriggerVolume best match: 71.621% at nonmatchings/collidePlayerWithCourseTriggerVolume-731940616440357983/base_1.c.
#pragma GLOBAL_ASM("asm/nonmatchings/race_course_effects/collidePlayerWithCourseTriggerVolume.s")

#ifdef NON_MATCHING
void collidePlayerWithCourseTriggerVolume(CourseEffectPlayer *player, RaceCourseTriggerEffect *trigger) {
    Vec3i transformed;
    Vec3i delta;
    FixedMatrix3s matrix;
    s32 savedPush;
    CourseTriggerEntry *entry;
    s32 scaleX;
    s32 collisionRadius;
    s32 limit;
    s32 positiveLimit;
    s32 negativeLimit;
    s32 push;

    if ((gRaceUpdatePaused == 0) && (player->isActive != 0)) {
        entry = &gCourseTriggerEntries[trigger->entryIndex];
        makeFixedRotationYX(matrix, -entry->pitch, -entry->yaw);

        if ((player->flags & 0x2000) == 0) {
            delta.x = player->posX - gCourseTriggerEntries[trigger->entryIndex].pos.x;
            delta.y = player->posY - gCourseTriggerEntries[trigger->entryIndex].pos.y;
            delta.z = player->posZ - gCourseTriggerEntries[trigger->entryIndex].pos.z;
            transformVec3iByFixedMatrix(matrix, &delta, &transformed);

            if ((transformed.z >= -trigger->scaleZ) && (trigger->scaleZ >= transformed.z) &&
                (transformed.x >= (-trigger->scaleX - 0x30000)) &&
                ((trigger->scaleX + 0x30000) >= transformed.x) && (transformed.y >= -0xFFFFF) &&
                (trigger->scaleY >= transformed.y)) {
                delta.y = trigger->scaleY - transformed.y;
                delta.x = -transformed.x;
                delta.z = 0;

                entry = &gCourseTriggerEntries[trigger->entryIndex];
                makeFixedRotationXY(matrix, entry->pitch, entry->yaw);
                transformVec3iByFixedMatrix(matrix, &delta, &transformed);

                player->posX += transformed.x;
                player->posY += transformed.y;
                player->posZ += transformed.z;
                player->flags |= 0x02000000;
                player->yaw = gCourseTriggerEntries[trigger->entryIndex].yaw;
                player->pitch = gCourseTriggerEntries[trigger->entryIndex].pitch;
                return;
            }
        }

        if (player->unk578 == 0) {
            func_80088C80(&trigger->pos1, trigger->scaleX + 0x30000, 0x120000, player->unk0);
            func_80088C80(&trigger->pos2, trigger->scaleX + 0x30000, 0x100000, player->unk0);
        }

        delta.x = player->posX - gCourseTriggerEntries[trigger->entryIndex].pos.x;
        delta.y = player->posY - gCourseTriggerEntries[trigger->entryIndex].pos.y;
        delta.z = player->posZ - gCourseTriggerEntries[trigger->entryIndex].pos.z;
        transformVec3iByFixedMatrix(matrix, &delta, &transformed);

        if (transformed.y <= 0) {
            if (transformed.y >= -0x160000) {
                if (transformed.z >= -trigger->scaleZ) {
                    if (trigger->scaleZ >= transformed.z) {
                        scaleX = trigger->scaleX;
                        collisionRadius = player->collisionRadius;
                        limit = scaleX + collisionRadius;
                        if (transformed.x >= ((-scaleX - collisionRadius) - 0x30000)) {
                            positiveLimit = limit + 0x30000;
                            if (positiveLimit >= transformed.x) {
                                push = 0;
                                negativeLimit = -limit;
                                if (transformed.x >= 0) {
                                    if (transformed.x < positiveLimit) {
                                        push = (limit - transformed.x) + 0x30000;
                                    }
                                } else if ((negativeLimit - 0x30000) < transformed.x) {
                                    push = (negativeLimit - transformed.x) - 0x30000;
                                }

                                if (push != 0) {
                                    savedPush = push;
                                    makeFixedRotationY(matrix, gCourseTriggerEntries[trigger->entryIndex].yaw);
                                    delta.y = 0;
                                    delta.z = 0;
                                    delta.x = savedPush;
                                    transformVec3iByFixedMatrix(matrix, &delta, &transformed);
                                    player->posX += transformed.x;
                                    player->posZ += transformed.z;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

void updateCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    if (D_80121D80[0].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(D_80121D80, arg0);
    }
    if (D_80121D80[1].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(D_8012238C, arg0);
    }
    if (D_80121D80[2].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(D_80122998, arg0);
    }
    if (D_80121D80[3].isActive != 0) {
        collidePlayerWithCourseTriggerVolume(D_80122FA4, arg0);
    }
    addRenderCallback(&gRaceObjectRenderCallbackList, renderCourseTriggerVolume, arg0);
}

void initCourseTriggerVolume(RaceCourseTriggerEffect *arg0) {
    CourseTriggerScratch scratch;
    CourseTriggerEntry *entry;

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pitch = entry->pitch;
    arg0->yaw = entry->yaw;
    arg0->scaleX = entry->scaleX << 0x10;
    arg0->scaleY = entry->scaleY << 0x10;
    arg0->scaleZ = entry->scaleZ << 0x10;
    arg0->displayList = entry->displayList;

    makeFixedRotationXY(scratch.mtx, arg0->pitch, arg0->yaw);

    scratch.source.y = 0;
    scratch.source.x = 0;
    scratch.source.z = arg0->scaleZ;
    transformVec3iByFixedMatrix(scratch.mtx, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pos1.x = entry->pos.x + scratch.dest.x;
    arg0->pos1.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos1.z = entry->pos.z + scratch.dest.z;

    scratch.source.x = 0;
    scratch.source.y = 0;
    scratch.source.z = -arg0->scaleZ;
    transformVec3iByFixedMatrix(scratch.mtx, &scratch.source, &scratch.dest);

    entry = &gCourseTriggerEntries[arg0->entryIndex];
    arg0->pos2.x = entry->pos.x + scratch.dest.x;
    arg0->pos2.y = entry->pos.y + scratch.dest.y - 0x100000;
    arg0->pos2.z = entry->pos.z + scratch.dest.z;

    setCallbackTaskCallback(arg0, updateCourseTriggerVolume);
}
