#ifndef CONTROLLER_PAK_CONTINUE_PROMPT_FLOW_H
#define CONTROLLER_PAK_CONTINUE_PROMPT_FLOW_H

#include "common.h"

typedef struct {
    /* 0x0 */ u16 x;
    /* 0x2 */ u8 state;
    /* 0x3 */ u8 pad3;
} ControllerPakContinuePromptTransition;

extern ControllerPakContinuePromptTransition gControllerPakContinuePromptTransition;

void initControllerPakContinuePromptFlow(void);
void updateControllerPakContinuePromptFlow(void);
void closeControllerPakContinuePromptFlow(void);

#endif
