#ifndef CONTROLLER_PAK_REPLAY_SAVE_MESSAGE_FLOW_H
#define CONTROLLER_PAK_REPLAY_SAVE_MESSAGE_FLOW_H

/* Legacy names: startup logo callbacks, owned by task 4 while task 0 is suspended. */
void initControllerPakReplaySaveMessageFlow(void);
void updateControllerPakReplaySaveMessageFirstPageFadeIn(void);
void updateControllerPakReplaySaveMessageFirstPageFadeOut(void);
void waitForControllerPakReplaySaveMessageSecondPage(void);
void updateControllerPakReplaySaveMessageSecondPageFadeIn(void);
void fadeOutControllerPakReplaySaveMessageFlow(void);

#endif
