#ifndef PLAYER_SETUP_MENU_H
#define PLAYER_SETUP_MENU_H

#include "common.h"

void initPlayerSetupMenu(void);
void updatePlayerSetupPlayerCountMenu(void);
void initPlayerSaveSetupMenu(void);
void updatePlayerSaveSetupMenu(void);
void playerSetupMenuNoop(void);
void updatePlayerSetupRumblePrompt(void);
void initPlayerSaveData(s32 arg0);

#endif
