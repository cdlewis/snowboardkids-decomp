#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80023880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80023A68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024050.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024380.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024968.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80024E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800257F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80025AA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80025BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80025E6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80025FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800260E8.s")

extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, void *);
extern void func_800260E8(void);
extern void func_80028FF0(void);
extern u8 D_80121B55;
extern s32 D_80124868;

typedef struct Obj2784C {
    char pad0[0x18];
    s16 unk18;
    char pad1A[0x42];
    u8 unk5C;
} Obj2784C;

typedef struct Struct801235B8_24480 {
    char pad0[0x20];
    s32 unk20;
} Struct801235B8_24480;

extern Struct801235B8_24480 *D_801235B8;
extern void func_800275E0(void);
extern void func_80027AF8(void);

void func_800263A4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x108;
    *(s16 *)((u8 *)arg0 + 0x1A) = 8;
    func_80071824(arg0, func_800260E8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800263D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80026A54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80026B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80026C4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800271CC.s")

extern void func_800271CC(void);

void func_800273C4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x84;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0xC;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s8 *)((u8 *)arg0 + 0x1E) = 0;
    *(s8 *)((u8 *)arg0 + 0x20) = 0;
    *(s8 *)((u8 *)arg0 + 0x1F) = 0;
    func_80071824(arg0, func_800271CC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80027408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80027498.s")

extern void func_80027498(void);

void func_800275A4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x18) = -0x8;
    *(s16 *)((u8 *)arg0 + 0x1A) = -0x5C;
    *(s16 *)((u8 *)arg0 + 0x1C) = 0;
    *(s8 *)((u8 *)arg0 + 0x1E) = 0;
    func_80071824(arg0, func_80027498);
}

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800275E0.s")

void func_8002784C(Obj2784C *arg0) {
    s32 var_v1;
    Obj2784C *var_v0;
    Obj2784C *temp_a2 = arg0;
    Obj2784C *temp_a1;

    temp_a1 = arg0;
    if (arg0->unk18 < -0x74) {
        var_v1 = 0;
        if ((s32)arg0->unk5C > 0) {
            var_v0 = arg0;
            do {
                var_v0->unk18 += 0x20;
                if (var_v0->unk18 >= -0x74) {
                    var_v0->unk18 = -0x74;
                }
                var_v1++;
                var_v0 = (Obj2784C *)((u8 *)var_v0 + 2);
            } while (var_v1 < (s32)temp_a1->unk5C);
        }
    } else if (D_801235B8->unk20 == 4) {
        func_80071824(temp_a2, func_80027AF8);
    }
    func_800483FC(&D_80124868, func_800275E0, temp_a2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80027914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80027A08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80027AF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80028194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80028354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_800287EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80028B0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80028C68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/24480/func_80028FF0.s")

void func_8002916C(void *arg0) {
    s32 var_v0;
    u8 *var_v1;

    if (D_80121B55 == 2) {
        *(s16 *)((u8 *)arg0 + 0x18) = -0x6E;
        *(s16 *)((u8 *)arg0 + 0x1A) = -0x4C;
    } else {
        *(s16 *)((u8 *)arg0 + 0x18) = -0x8C;
        *(s16 *)((u8 *)arg0 + 0x1A) = -0x46;
    }
    var_v0 = 0;
    var_v1 = arg0;
    if ((s32)D_80121B55 > 0) {
        do {
            *(s16 *)(var_v1 + 0x1C) = 0;
            var_v0++;
            var_v1 += 2;
        } while (var_v0 < (s32)D_80121B55);
    }
    func_80071824(arg0, func_80028FF0);
}

void func_800291F0(s32 arg0) {

}
