#ifndef SYSTEM_BOOT_H
#define SYSTEM_BOOT_H

#include "common.h"

void main(void *arg);
void bootThreadMain(void *arg);
void gameThreadMain(void *arg);
void initGameSystems(void);

#endif
