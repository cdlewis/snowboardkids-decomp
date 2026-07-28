void updateControllerPakRaceRecordSaveFlow(void) {
    RaceRecordSaveCompletion flowComplete;
    s16 statusCode;
    void *saveStatusTask = D_8010ADE4;
    s32 transitionStep;
    s32 valueThree;
    s32 retryLimit;
    s16 valueFour;

    flowComplete.value = 0;
    if (gCurrentGameTask->fade != 0) {
        gCurrentGameTask->fade = stepMenuFadeAlpha((s16)gCurrentGameTask->fade, 0x24, 0);
        if (gCurrentGameTask->fade == 0) {
            gControllerPakMenuState.state = 3;
            gControllerPakMenuState.confirmChoice = 1;
            createCallbackTask(initControllerPakDeleteConfirmPrompt, 0, 0x64);
            setCurrentGameTaskCallback(updateControllerPakRaceRecordSaveOverwritePrompt, 0);
        }
    } else {
        retryLimit = 3;
        transitionStep = gControllerPakRaceRecordSaveStatusTransition.step;
        if (gMenuSelectionConfirmTimer == 0) {
            if (transitionStep == 1) {
            if (gMenuChoicePromptState[0] != 0) {
                statusCode = 6;
            } else {
                statusCode = gControllerPakStatusCodes[0];
            }
            valueFour = 4;

            switch (statusCode) {
                case 0:
                    gRumblePakConnectedByController[0] = 0;
                    requestRumbleMotorInit(0);
                    if ((gRumbleMotorStatuses[0] != 1) && (gRumbleMotorStatuses[0] != 0xB) &&
                        (gRumbleMotorStatuses[0] != valueFour)) {
                        gRumblePakConnectedByController[0] = 1;
                    } else {
                        gRumblePakConnectedByController[0] = 0;
                    }
                    requestControllerPakProbe(0);
                    if (gControllerPakStatusCodes[0] == 0x10) {
                        gControllerPakStatusCodes[0] = 0x11;
                    }
                    break;

                case 1:
                    requestControllerPakSaveStatus(0);
                    if ((gControllerPakStatusCodes[0] == 9) || (gControllerPakStatusCodes[0] == 2)) {
                        gControllerPakStatusCodes[0] = 8;
                        gMenuChoicePromptState[0] = 1;
                    }
                    break;

                case 2:
                    if (transitionStep != (valueThree = retryLimit)) {
                        requestControllerPakSaveWrite(0);
                        if (gControllerPakRetryCounts[0] == 0) {
                            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 5;
                            gControllerPakRaceRecordSaveStatusTransition.step = 3;
                            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                        } else if (gControllerPakRetryCounts[0] == valueThree) {
                            if (saveStatusTask != 0) {
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
                    requestControllerPakRepair(0);
                    if (gControllerPakRetryCounts[0] == 0) {
                        if (saveStatusTask != 0) {
                            gControllerPakRaceRecordSaveStatusTransition.step = 3;
                            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x10;
                        } else {
                            gControllerPakStatusCodes[0] = 0x10;
                        }
                    } else if (gControllerPakRetryCounts[0] == retryLimit) {
                        if (saveStatusTask != 0) {
                            gControllerPakRaceRecordSaveStatusTransition.step = 3;
                            gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0xE;
                        } else {
                            gControllerPakStatusCodes[0] = 0xE;
                        }
                        gControllerPakRetryCounts[0] = 0;
                    }
                    break;

                case 4:
                    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                        enqueueSoundEffect(1, 0x32);
                        if (saveStatusTask != 0) {
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
                    if ((gMenuChoicePromptState[0] == retryLimit) ||
                        (gMenuChoicePromptState[0] == valueFour)) {
                        valueThree = 3;
                        if ((gPlayerInputPressed & 0x10800) && (gMenuChoicePromptState[0] != valueThree)) {
                            gMenuChoicePromptState[0]--;
                            enqueueSoundEffect(0x19, 0x32);
                        }
                        if (gPlayerInputPressed & 0x20400) {
                            if (gMenuChoicePromptState[0] != 4) {
                                gMenuChoicePromptState[0]++;
                                enqueueSoundEffect(0x19, 0x32);
                            }
                        }
                        if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
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
                            } else {
                                if (gControllerPakStatusCodes[0] == 8) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 2;
                                } else if ((gControllerPakStatusCodes[0] == 0xA) ||
                                           (gControllerPakStatusCodes[0] == 0x11)) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 0xF;
                                } else if (gControllerPakStatusCodes[0] == 7) {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 4;
                                } else {
                                    gControllerPakRaceRecordSaveStatusTransition.nextStatus = 5;
                                }
                            }
                            gMenuChoicePromptState[0] += retryLimit - 1;
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
                    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                        enqueueSoundEffect(1, 0x32);
                        gMenuChoicePromptState[0] =
                            gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                    }
                    break;

                case 15:
                    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                        enqueueSoundEffect(1, 0x32);
                        gMenuChoicePromptState[0] =
                            gControllerPakRaceRecordSaveStatusChoicePromptStates[gControllerPakStatusCodes[0]];
                    }
                    break;

                case 14:
                case 16:
                    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                        enqueueSoundEffect(1, 0x32);
                        if (gControllerPakStatusCodes[0] == 0xE) {
                            if (saveStatusTask != 0) {
                                gControllerPakRaceRecordSaveStatusTransition.step = 3;
                                gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0x12;
                            } else {
                                gControllerPakStatusCodes[0] = 7;
                            }
                        } else if (saveStatusTask != 0) {
                            gControllerPakRaceRecordSaveStatusTransition.step = 1;
                            gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                            gControllerPakStatusCodes[0] = 0;
                        } else {
                            gControllerPakStatusCodes[0] = 0;
                        }
                    }
                    break;

                case 18:
                    if ((gPlayerInputPressed & 0x8000) || (gPlayerInputPressed & 0x1000)) {
                        enqueueSoundEffect(1, 0x32);
                        gControllerPakRaceRecordSaveStatusTransition.step = 3;
                        gControllerPakRaceRecordSaveStatusTransition.alpha = 0x100;
                        gControllerPakRaceRecordSaveStatusTransition.targetStatus = 0;
                    }
                    break;
            }
        }
        flowComplete.value = (u8)gRacePlayers.status & 1;
        } else {
            gMenuSelectionConfirmTimer = gMenuSelectionConfirmTimer + 1;
        }
    }

    if (flowComplete.value != 0) {
        gMenuSelectionConfirmTimer = 1;
    }
    if (gMenuSelectionConfirmTimer == 0x23) {
        setCurrentGameTaskCallback(fadeOutControllerPakRaceRecordSaveFlow, 0);
    }
    updateCallbackTasks();
}
