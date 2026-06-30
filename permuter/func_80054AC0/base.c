typedef int s32;
typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;

extern void func_80071824(void *, void *);
extern void func_800483FC(void *, void *, s32);
extern u8 D_80124878[];

void func_8005499C(void *arg0);
void func_80054A64(void *arg0);

void func_80054AC0(void *arg0) {
    s32 temp_v0 = *(s32 *)((u8 *)arg0 + 0x24);
    s32 temp_t8 = *(s32 *)((u8 *)arg0 + 0x24) - 4;

    *(s32 *)((u8 *)arg0 + 0x18) += temp_v0;
    *(s32 *)((u8 *)arg0 + 0x24) = temp_t8;
    if (temp_t8 == 0) {
        *(s16 *)((u8 *)arg0 + 0x28) = 0x1E;
        func_80071824(arg0, func_80054A64);
    }
    func_800483FC(D_80124878, func_8005499C, (s32)arg0);
}
