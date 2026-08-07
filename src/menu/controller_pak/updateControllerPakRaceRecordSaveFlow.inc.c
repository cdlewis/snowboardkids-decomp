void updateControllerPakRaceRecordSaveFlow(void) {
    RaceRecordSaveCompletion sp24;
    RaceRecordSaveCompletion *completion;
    CallbackTask *temp_t0;
    s16 statusCode;
    s32 fade;

    completion = &sp24;
    completion->value = 0;
    fade = gCurrentGameTask->fade;
    temp_t0 = D_8010ADE4;
    if (fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s32)(s16)fade, 0x24, 0U);
        if (gCurrentGameTask->fade == 0) {
            gControllerPakMenuState.state = 3;
            gControllerPakMenuState.confirmChoice = 1;
            createCallbackTask((CallbackTaskCallback)initControllerPakDeleteConfirmPrompt, 0, 0x64);
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveOverwritePrompt, 0);
        }
    } else {
        if (gMenuSelectionConfirmTimer == 0) {
            if (gControllerPakRaceRecordSaveStatusTransition.step == 1) {
                if (gMenuChoicePromptState[0] != 0) {
                    statusCode = 6;
                } else {
                    statusCode = gControllerPakStatusCodes[0];
                }
                switch (statusCode) {
                    case 0:
                        gRumblePakConnectedByController[0] = 0;
                        requestRumbleMotorInit(0U);
                        if ((gRumbleMotorStatuses[0] != 1) && (gRumbleMotorStatuses[0] != 0xB) &&
                            (gRumbleMotorStatuses[0] != 4)) {
                            gRumblePakConnectedByController[0] = 1;
                        } else {
                            gRumblePakConnectedByController[0] = 0;
                        }
                        requestControllerPakProbe(0U);
                        if (gControllerPakStatusCodes[0] == 0x10) {
                            gControllerPakStatusCodes[0] = 0x11;
                        }
                        break;
                    case 1:
                        requestControllerPakSaveStatus(0U);
                        if ((gControllerPakStatusCodes[0] == 9) || (gControllerPakStatusCodes[0] == 2)) {
                            gControllerPakStatusCodes[0] = 8;
                            gMenuChoicePromptState[0] = 1;
                        }
                        break;
                    case 2:
                        if (gControllerPakRaceRecordSaveStatusTransition.step != 3) {
                            requestControllerPakSaveWrite(0U);
                            if (gControllerPakRetryCounts[0] == 0) {
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 5;
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                            } else if (gControllerPakRetryCounts[0] == 3) {
                                if (temp_t0 != NULL) {
                                    gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xD;
                                    gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                } else {
                                    gControllerPakStatusCodes[0] = 0xD;
                                }
                                gControllerPakRetryCounts[0] = 0;
                            }
                        }
                        break;
                    case 3:
                        requestControllerPakRepair(0U);
                        if (gControllerPakRetryCounts[0] == 0) {
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x10;
                            } else {
                                gControllerPakStatusCodes[0] = 0x10;
                            }
                        } else if (gControllerPakRetryCounts[0] == 3) {
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xE;
                            } else {
                                gControllerPakStatusCodes[0] = 0xE;
                            }
                            gControllerPakRetryCounts[0] = 0;
                        }
                        break;
                    case 4:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 1;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                gControllerPakStatusCodes[0] = 0;
                            } else {
                                gControllerPakStatusCodes[0] = 0;
                            }
                        }
                        break;
                    case 5:
                        if (gRacePlayers[0].menuState == 0) {
                            gRacePlayers[0].menuState = 1;
                        }
                        break;
                    case 6:
                        if ((gMenuChoicePromptState[0] == 3) || (gMenuChoicePromptState[0] == 4)) {
                            if (gPlayerInputPressed[0] & (STICK_UP | U_JPAD)) {
                                if (gMenuChoicePromptState[0] != 3) {
                                    gMenuChoicePromptState[0]--;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            }
                            if (gPlayerInputPressed[0] & (STICK_DOWN | D_JPAD)) {
                                if (gMenuChoicePromptState[0] != 4) {
                                    gMenuChoicePromptState[0]++;
                                    enqueueSoundEffect(0x19, 0x32);
                                }
                            }
                            if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                                enqueueSoundEffect(1, 0x32);
                                if (gMenuChoicePromptState[0] == 4) {
                                    if (gControllerPakStatusCodes[0] == 8) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
                                    } else if (gControllerPakStatusCodes[0] == 7) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 3;
                                    } else if (gControllerPakStatusCodes[0] == 0xF) {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0;
                                    } else {
                                        gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
                                    }
                                } else if (gControllerPakStatusCodes[0] == 8) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 2;
                                } else if ((gControllerPakStatusCodes[0] == 0xA) ||
                                           (gControllerPakStatusCodes[0] == 0x11)) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
                                } else if (gControllerPakStatusCodes[0] == 7) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
                                } else {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 5;
                                }
                                gMenuChoicePromptState[0] += 2;
                            }
                        }
                        break;
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 17:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gMenuChoicePromptState[0] =
                                (s16)gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                        }
                        break;
                    case 15:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gMenuChoicePromptState[0] =
                                (s16)gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                        }
                        break;
                    case 14:
                    case 16:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            if (gControllerPakStatusCodes[0] == 0xE) {
                                if (temp_t0 != NULL) {
                                    gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                    gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x12;
                                } else {
                                    gControllerPakStatusCodes[0] = 7;
                                }
                            } else if (temp_t0 != NULL) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 1;
                                gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                                gControllerPakStatusCodes[0] = 0;
                            } else {
                                gControllerPakStatusCodes[0] = 0;
                            }
                        }
                        break;
                    case 18:
                        if ((gPlayerInputPressed[0] & A_BUTTON) || (gPlayerInputPressed[0] & START_BUTTON)) {
                            enqueueSoundEffect(1, 0x32);
                            gControllerPakRaceRecordSaveStatusTransition.step = 3;
                            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
                        }
                        break;
                }
            }
            completion->value = gRacePlayers[0].menuState & 1;
        } else {
            gMenuSelectionConfirmTimer++;
        }
    }
    if (completion->value != 0) {
        gMenuSelectionConfirmTimer = 1;
    }
    if (gMenuSelectionConfirmTimer == 0x23) {
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}
