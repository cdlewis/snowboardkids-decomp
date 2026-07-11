#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "common.h"

void func_80000450(void);
void func_800005E4(void *arg0);
void func_80000960(void);
void func_800009B0(void);
void func_80000A40(u16 arg0);
void func_80000A8C(u16 arg0);
void requestRumbleMotorStart(u16 arg0);
void func_80000C48(u16 arg0);
void func_80000C94(u16 arg0);
void func_80000DB4(u16 arg0);
void func_80000E00(u16 arg0);
void func_80001010(u16 arg0);
void func_8000105C(u16 arg0);
void func_800012CC(u16 arg0);
void func_80001318(u16 arg0);
void func_80001538(u16 arg0);
void func_80001584(u16 arg0);
void requestControllerPakFileList(void);
void readControllerPakFileStates(void);
void requestControllerPakDeleteFile(u16 arg0);
void deleteControllerPakFile(u16 arg0);
void requestControllerPakFreeSpaceUpdate(void);
void updateControllerPakFreeSpaceInfo(void);
u16 func_80001904(s32 arg0);
s32 func_80001994(s32 arg0);
void func_80001C30(void);
void func_80001C80(void);
void func_80002024(void);
void func_800022B8(void);
void func_800024A8(void);
void func_8000262C(void);
void func_80002794(void);
void func_80002810(void);
void func_800028B4(void);
void func_80002A1C(void);
void func_80002CE4(void);
void func_80002D50(void);
s32 func_80002DA0(void);

#endif
