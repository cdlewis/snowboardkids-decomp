#ifndef EFFECT_TASK_SCHEDULER_H
#define EFFECT_TASK_SCHEDULER_H

#include "common.h"

typedef struct EffectTask {
    struct EffectTask *prev;
    struct EffectTask *next;
    void (*callback)(struct EffectTask *);
    u16 type;
    u16 priority;
    s16 unk10;
    s16 unk12;
    s16 callbackTimer;
    s16 isActive;
    void *args[0x40];
} EffectTask; // size = 0x118

void func_80070EC0(s32 arg0);
void updateEffectTasks(void);
void func_800710CC(s32 arg0);
void func_8007115C(void);
void func_800711C8(void);
#ifdef NON_MATCHING
EffectTask *func_800711D0(void (*callback)(EffectTask *), u16 type, s32 priority);
#else
EffectTask *func_800711D0(void (*callback)(EffectTask *), s32 type, s32 priority);
#endif
#ifdef NON_MATCHING
EffectTask *createEffectTask(void (*callback)(EffectTask *), u16 type, s32 priority);
#else
EffectTask *createEffectTask(void (*callback)(EffectTask *), s32 type, s32 priority);
#endif
EffectTask *func_80071664(void (*callback)(EffectTask *), s32 arg1, s32 arg2, s32 arg3);
/*
 * func_800716A4 is intentionally not declared here: its callers disagree on the
 * arity (race_item_effects.c passes 4 args, race_ui_effects.c passes 3), and
 * both call shapes are baked into matched ROM code. A single shared prototype
 * cannot satisfy both, so each caller keeps its own local declaration.
 */
void func_800716E4(EffectTask *task);
void func_80071824(void *task, void (*callback)());

#endif
