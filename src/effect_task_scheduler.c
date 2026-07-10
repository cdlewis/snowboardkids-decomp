#include "common.h"
#include "effect_task_scheduler.h"

extern EffectTask *D_80121848;
extern EffectTask *D_801214D8[];
extern u16 D_80121838;
extern u16 D_8012183A;
extern u16 D_8012183C;
extern u16 D_8012183E;
extern u16 D_80121840;
extern u16 D_80121842;
extern u16 D_80121844;
extern u16 D_80121846;
extern EffectTask D_80112780;
extern EffectTask *D_80112784;

#pragma GLOBAL_ASM("asm/nonmatchings/effect_task_scheduler/func_80070EC0.s")

// Mirrors the linked list head D_80112784 into the global cursor D_80121848,
// then for each node clears callbackTimer and invokes its callback with a
// pointer to itself, advancing via next. Assigning through the captured store
// (`s0 = (D_80121848 = D_80112784)`) and clearing callbackTimer via a temp
// steers IDO's register allocator to match the target's temp-register choice.
void func_8007105C(void) {
    EffectTask *s0;
    s32 new_var;

    s0 = (D_80121848 = D_80112784);
    if (D_80121848 != NULL) {
        do {
            new_var = 0;
            D_80121848->callbackTimer = new_var;
            D_80121848->callback(D_80121848);
            s0 = D_80121848->next;
            D_80121848 = s0;
        } while (D_80121848 != NULL);
    }
}

// Like func_8007105C, mirrors the linked list head D_80112784 into the global
// cursor D_80121848, then iterates: clearing each node's callbackTimer and
// invoking its callback, advancing via next. Stops early when a node's priority
// is less than arg0. The compound assignment `s0 = (D_80121848 = D_80112784)`
// steers IDO's register allocator to use a separate temp (t6) for the high-half
// load, matching the target.
void func_800710CC(s32 arg0) {
    EffectTask *s0 = (D_80121848 = D_80112784);

    if (s0 != NULL) {
        do {
            s0->callbackTimer = 0;
            s0 = D_80121848;
            if ((u16)s0->priority < arg0) {
                break;
            }
            s0->callback(s0);
            s0 = D_80121848->next;
            D_80121848 = s0;
        } while (s0 != NULL);
    }
}

// Drains the global cursor list (D_80121848): for each node, clears
// callbackTimer and invokes its callback with a pointer to itself, then advances
// via next.
// The empty `if ((s0 && s0) && s0) {}` is a dead condition that IDO 5.3 folds to
// zero instructions, but which steers the allocator into keeping the cursor in
// $s0 and emitting `move $a0, $s0` for the callback argument (matching the
// target). Without it IDO folds the reload straight into $a0.
void func_8007115C(void) {
    EffectTask *s0 = D_80121848;

    if (s0 != NULL) {
        do {
            s0->callbackTimer = 0;
            s0 = D_80121848;
            if ((s0 && s0) && s0) {}
            s0->callback(s0);
            s0 = D_80121848->next;
            D_80121848 = s0;
        } while (s0 != NULL);
    }
}

void func_800711C8(void) {
}

// func_800711D0 best match: 92.071%
#pragma GLOBAL_ASM("asm/nonmatchings/effect_task_scheduler/func_800711D0.s")

#ifdef NON_MATCHING
EffectTask *func_800711D0(void (*callback)(EffectTask *), s32 type, s32 priority) {
    EffectTask *task;
    EffectTask *prev;
    EffectTask *next;
    s32 index;

    type &= 0xFFFF;
    switch (type & 0xFF) {
    case 0:
        if (D_8012183A == 0) {
            return NULL;
        }
        D_8012183A--;
        break;
    case 1:
        if (D_80121840 == 0) {
            return NULL;
        }
        D_80121840--;
        break;
    case 2:
        if (D_80121842 == 0) {
            return NULL;
        }
        D_80121842--;
        break;
    case 3:
        if (D_80121844 == 0) {
            return NULL;
        }
        D_80121844--;
        break;
    case 4:
        if (D_80121846 == 0) {
            return NULL;
        }
        D_80121846--;
        break;
    case 5:
        if (D_8012183E == 0) {
            return NULL;
        }
        D_8012183E--;
        break;
    case 6:
        if (D_8012183C == 0) {
            return NULL;
        }
        D_8012183C--;
        break;
    default:
        return NULL;
    }

    if (D_80121838 == 0) {
        return NULL;
    }
    index = D_80121838 - 1;
    D_80121838 = index;

    task = D_801214D8[index];
    prev = &D_80112780;
    if (D_80112780.next != NULL) {
        next = D_80112780.next;
        do {
            if ((u16)next->priority < priority) {
                break;
            }
            prev = next;
            next = next->next;
        } while (next != NULL);
    }

    task->prev = prev;
    task->next = prev->next;
    next = prev->next;
    if (next != NULL) {
        next->prev = task;
    }
    prev->next = task;
    task->isActive = 1;
    task->callback = callback;
    task->type = type;
    task->priority = priority;
    return task;
}
#endif

// func_80071408 best match: 96.233%
#pragma GLOBAL_ASM("asm/nonmatchings/effect_task_scheduler/func_80071408.s")

#ifdef NON_MATCHING
EffectTask *func_80071408(void (*callback)(EffectTask *), u16 type, s32 priority) {
    EffectTask *task;
    EffectTask *prev;
    EffectTask *next;
    EffectTask *sentinel;
    s32 index;
    u32 i;

    switch (type & 0xFF) {
    case 0:
        if (D_8012183A == 0) {
            return NULL;
        }
        D_8012183A--;
        break;
    case 1:
        if (D_80121840 == 0) {
            return NULL;
        }
        D_80121840--;
        break;
    case 2:
        if (D_80121842 == 0) {
            return NULL;
        }
        D_80121842--;
        break;
    case 3:
        if (D_80121844 == 0) {
            return NULL;
        }
        D_80121844--;
        break;
    case 4:
        if (D_80121846 == 0) {
            return NULL;
        }
        D_80121846--;
        break;
    case 5:
        if (D_8012183E == 0) {
            return NULL;
        }
        D_8012183E--;
        break;
    case 6:
        if (D_8012183C == 0) {
            return NULL;
        }
        D_8012183C--;
        break;
    default:
        return NULL;
    }

    if ((D_80121838 & 0xFFFFu) == 0) {
        return NULL;
    }
    index = D_80121838 - 1;
    sentinel = &D_80112780;
    prev = sentinel;
    D_80121838 = index;
    task = D_801214D8[index & 0xFFFF];
    if (prev->next != NULL) {
        next = sentinel->next;
        do {
            if ((u16) next->priority < priority) {
                break;
            }
            prev = next;
            next = next->next;
        } while (next != NULL);
    }

    task->prev = prev;
    task->next = prev->next;
    next = prev->next;
    if (next != NULL) {
        next->prev = task;
    }
    prev->next = task;
    task->callback = callback;
    task->type = type;
    task->priority = priority;
    task->isActive = 1;

    i = 0;
    do {
        task->args[i] = NULL;
        i = (i + 1) & 0xFFFF;
    } while (i < 0x40U);

    return task;
}
#endif

EffectTask *func_80071664(void (*callback)(EffectTask *), s32 arg1, s32 arg2, s32 arg3){ EffectTask *t=func_80071408(callback,arg1&0xFFFF,arg2); if(t!=NULL){t->unk10=arg3;} return t;}

EffectTask *func_800716A4(void (*callback)(EffectTask *), s32 arg1, s32 arg2, s32 arg3){ EffectTask *t=func_800711D0(callback,arg1&0xFFFF,arg2); if(t!=NULL){t->unk10=arg3;} return t;}

void func_800716E4(EffectTask *task) {
    EffectTask *next;
    u16 *counter;
    unsigned char type;

    task->isActive = 0;
    task->prev->next = task->next;
    next = task->next;
    if (next != NULL) {
        next->prev = task->prev;
    }

    D_801214D8[D_80121838] = task;
    D_80121838++;

    type = task->type & 0xFF;
    switch (type) {
    case 0:
        counter = &D_8012183A;
        (*counter)++;
        return;
    case 1:
        counter = &D_80121840;
        (*counter)++;
        return;
    case 2:
        counter = &D_80121842;
        (*counter)++;
        return;
    case 3:
        counter = &D_80121844;
        (*counter)++;
        return;
    case 4:
        counter = &D_80121846;
        (*counter)++;
        return;
    case 5:
        counter = &D_8012183E;
        (*counter)++;
        return;
    case 6:
        counter = &D_8012183C;
        (*counter)++;
        return;
    }
}

void func_80071824(void *task, void (*callback)()) {
    ((EffectTask *)task)->callback = (void (*)(EffectTask *))callback;
}
