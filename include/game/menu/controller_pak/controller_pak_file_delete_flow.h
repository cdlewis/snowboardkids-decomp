#ifndef CONTROLLER_PAK_FILE_DELETE_FLOW_H
#define CONTROLLER_PAK_FILE_DELETE_FLOW_H

#include "common.h"

#if defined(VERSION_JP)
extern u8 gControllerPakDeletePromptState;
extern u8 gControllerPakMenuConfirmChoice;
extern u8 gControllerPakMenuCursorState;
#endif

void initControllerPakFileDeleteFlow(void);
void updateControllerPakFileDeleteMainOptions(void);
void updateControllerPakFileDeleteFileList(void);
void updateControllerPakFileDeleteConfirm(void);
void updateControllerPakFileDeletePrompt(void);
void updateControllerPakFileDeleteErrorPrompt(void);
void fadeOutControllerPakFileDeleteFlow(void);

#endif
