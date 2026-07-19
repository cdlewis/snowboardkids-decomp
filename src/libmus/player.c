#include "game/audio/audio_engine_internal.h"

#include "player_commands.c"

s32 Fgoto(PlayerCommandState *arg0, u8 *arg1) {
    s32 offset;
    s32 sequenceOffset;

    sequenceOffset = *arg1++ << 8;
    sequenceOffset += *arg1++;

    offset = *arg1++ << 8;
    offset += *arg1++;
    arg0->unk60 = arg0->unk64 + offset;
    arg0->unkC8 = 1;

    offset = *arg1++ << 8;
    offset += *arg1++;
    arg0->unk68 = arg0->unk6C + offset;
    arg0->unkCA = 1;

    return arg0->restartPos + sequenceOffset;
}

#include "player_commands_tail.c"
#include "player_api.c"

// __MusIntMain best match: 99.025% (nonmatchings/__MusIntMain-8075865578671233833/base_17.c)

#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntMain.s")

#ifdef NON_MATCHING
ALMicroTime __MusIntMain(void *arg0) {
    PlayerCommandState *var_s0;
    s32 temp_t0;
    s32 temp_v0;
    s32 var_s1;
    u32 temp_t9;
    u32 temp_t9_2;

    var_s0 = mus_channels;
    var_s1 = 0;
    if (max_channels > 0) {
        do {
            if (var_s0->sequencePos != 0) {
                temp_t9 = (var_s0->unk0 += (u16)var_s0->unkB8);

                if ((var_s0->unkBC != 0x7FFF) && ((u32)var_s0->unkC < temp_t9) && (var_s0->sequencePos != 0)) {
loop_6:
                    __MusIntGetNewNote(var_s0, var_s1);
                    if ((int)((u32)var_s0->unkC < (u32)var_s0->unk0)) {
                        if (var_s0->sequencePos != 0) {
                            goto loop_6;
                        }
                    }
                }

                if (0 == var_s0->sequencePos) {

                } else {
                    if (var_s0->unk60 != 0) {
                        __MusIntProcessContinuousVolume(var_s0);
                    }
                    if (var_s0) {
                    }
                    if (var_s0->unk68 != 0) {
                        __MusIntProcessContinuousPitchBend(var_s0);
                    }

                    temp_v0 = var_s0->fadeTarget;
                    temp_t0 = temp_v0 - (1 & 0xFFFFFFFFFFFFFFFF);
                    if (var_s0->fadeTarget != -1) {
                        var_s0->fadeTarget = temp_t0;
                        if ((temp_t0 ^ 0) == -1) {
                            var_s0->sequencePos = Fstop(var_s0, 0);
                            temp_t9_2 = var_s0->unkE4;
                            if (temp_t9_2 != 0) {
                                var_s0->unkE4 = 0;
                                alSynStopVoice(&gAudioSynthesizer,
                                               (ALVoice *)(mus_voices + (7 * (((0, var_s1)) * 4))));
                            }
                        }
                    }

                    if (var_s0->unkE4 != 0) {
                        __MusIntProcessEnvelope(var_s0);
                        __MusIntProcessVibrato(var_s0);
                        __MusIntProcessWobble(var_s0);
                        __MusIntSetPitch(var_s0, var_s1);
                        __MusIntSetVolumeAndPan(var_s0, var_s1);
                    }

                    temp_t9_2 = (u32)(var_s0->unk0 - var_s0->unk10) >> 8;
                    var_s0->noteAgeTicks = (u16)temp_t9_2;
                    var_s0->noteAgeTicksF = (f32)(temp_t9_2 & 0xFFFF);
                }
            }
            var_s1 += 1;
            var_s0 += 1;
        } while (var_s1 < max_channels);
    }

    mus_next_frame_time += 1;
    return 0xF4240 / (s32)mus_vsyncs_per_second;
}
#endif

// __MusIntGetNewNote best match: 93.328% (nonmatchings/__MusIntGetNewNote-2694253543240320626/base_11.c)

#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntGetNewNote.s")

#ifdef NON_MATCHING
typedef u8 *(*PlayerCommandHandler)(PlayerCommandState *, u8 *, s32);

extern PlayerCommandHandler jumptable[];
extern u8 mus_default_velocities[];

void __MusIntGetNewNote(PlayerCommandState *arg0, s32 arg1) {
    u8 *seq;
    u8 *durationPos;
    u8 cmd;
    u8 fxMix;
    u32 duration;
    s32 soundIndex;

    seq = arg0->sequencePos;
    if (seq != NULL) {
        cmd = seq[0];
        while (cmd >= 0x80) {
            seq = jumptable[cmd & 0x7F](arg0, seq + 1, cmd);
            if (seq == NULL) {
                break;
            }
            cmd = seq[0];
        }
    }
    arg0->sequencePos = (s32)seq;

    if (seq != NULL) {
        arg0->portamentoStartPitch = arg0->currentNotePitch;
        cmd = *seq;
        seq++;
        arg0->sequencePos = (s32)seq;
        arg0->unkFE = cmd;

        if (arg0->unkED != 0) {
            arg0->unk108 = mus_default_velocities[*seq];
            seq++;
            arg0->sequencePos = (s32)seq;
        } else {
            arg0->unk108 = mus_default_velocities[arg0->unkEE];
        }

        duration = (u16)arg0->unkC0;
        if (duration != 0) {
            arg0->unkBC = duration;
            arg0->unk28 = (f32)(duration & 0xFFFF);
        }

        if ((arg0->flagE6 != 0) || (duration == 0)) {
            arg0->flagE6 = 0;
            durationPos = (u8 *)arg0->sequencePos;
            cmd = *durationPos;
            durationPos++;
            arg0->sequencePos = (s32)durationPos;
            if (cmd < 0x80) {
                arg0->unkBC = cmd;
                arg0->unk28 = (f32)cmd;
            } else {
                duration = (cmd & 0x7F) << 8;
                arg0->unkBC = duration;
                duration += *durationPos;
                arg0->unkBC = duration;
                arg0->unk28 = (f32)(duration & 0xFFFF);
                arg0->sequencePos = (s32)(durationPos + 1);
            }
        }

        duration = arg0->unkC;
        arg0->unkC += arg0->unkBC << 8;
        arg0->noteAgeTicks = 0;
        arg0->pitchPulseOffset = 0;
        arg0->unk10 = duration;
        arg0->noteAgeTicksF = 0.0f;
        arg0->unk107 = arg0->unk106;

        if (arg0->unkFE != 0) {
            if (arg0->jumpTarget != 0) {
                u8 *entry = (u8 *)arg0->jumpTarget + (arg0->unkFE * 4);

                arg0->instrumentIndex = entry[-0x30];
                arg0->unkF2 = entry[-0x2E] / 2;
                Fdefa(arg0, arg0->data->commands + (entry[-0x2F] * 7));
                arg0->unkFE = ((u8 *)arg0->jumpTarget + (arg0->unkFE * 4))[-0x2D];
            }

            soundIndex = arg0->instrumentIndex;
            if (gSoundBankEntryCount <= soundIndex) {
                soundIndex = 0;
            }

            if (arg0->flagE5 == 0) {
                if (arg0->unkE4 != 0) {
                    alSynStopVoice(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)));
                }
                arg0->unkE4 = 1;
                arg0->unkB6 = 0xFFFF;
                arg0->unkE3 = 0xFF;
                alSynStartVoice(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)),
                                gSoundWaveTable[soundIndex]);
            }

            arg0->notePitch = gSoundPlayerTuningTable[soundIndex] + arg0->unkFE - 5;
            if (arg0->flagE8 == 0) {
                arg0->padF4[4] = 0;
                arg0->padF4[5] = arg0->padF4[1];
                __MusIntInitEnvelope(arg0);
            }
            __MusIntSetPitch(arg0, arg1);
            __MusIntSetVolumeAndPan(arg0, arg1);
            fxMix = arg0->unkF3;
            if (arg0->unkE2 != fxMix) {
                arg0->unkE2 = fxMix;
                alSynSetFXMix(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)), fxMix);
            }
        } else if (arg0->padF4[4] < 4) {
            arg0->padF4[4] = 4;
            arg0->padF4[6] = 1;
            arg0->unk100 = arg0->unk0;
            arg0->unk104 = arg0->padF4[5];
        }
    } else if (arg0->unkE4 != 0) {
        arg0->unkE4 = 0;
        alSynStopVoice(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)));
    }
}
#endif

void __MusIntSetVolumeAndPan(PlayerCommandState *arg0, s32 arg1) {
    u32 volume;
    s32 stopping;
    int pan;
    u8 oldPan;

    volume = (u32)(arg0->unkF9 * (*arg0).unkEF * arg0->unk108 * arg0->unkB0) >> 13;
    if (volume >= 0x8000U) {
        volume = 0x7FFF;
    }

    if (arg0->soundId == 0) {
        volume *= mus_master_volume_songs;
    } else {
        volume *= mus_master_volume_effects;
    }

    stopping = arg0->fadeTarget;
    volume >>= 15;

    if (stopping != -1) {
        volume = (stopping * volume) / arg0->fadeTime;
    }

    if (volume != arg0->unkB6) {
        arg0->unkB6 = volume;
        alSynSetVol(&gAudioSynthesizer, (ALVoice *)(mus_voices + (14 * (2 * arg1))), (s16)volume, 0xF4240 / mus_vsyncs_per_second);
    }

    stopping = arg0->unkE3;
    oldPan = stopping;
    pan = (((*(u8 *)&arg0->unkF2) * arg0->unkB2) >> 7) & 0x7F;
    if (pan != oldPan) {
        arg0->unkE3 = pan;
        alSynSetPan(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)), pan & 0xFF);
    }
}

// __MusIntSetPitch best match: 97.179% (nonmatchings/__MusIntSetPitch-1225020319268080736/base_2.c)

#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntSetPitch.s")

#ifdef NON_MATCHING
void __MusIntSetPitch(PlayerCommandState *arg0, s32 arg1) {
    register f32 notePitch;
    f32 portamentoStartPitch;
    f32 portamentoPitchStep;
    f32 pitchRatio;
    f32 sequencePitchOffset;
    f64 pitchBendScale;
    s32 signedNotePitch;
    u8 portamentoTime;

    signedNotePitch = arg0->notePitch;
    if (signedNotePitch & 0x80) {
        notePitch = -0x100 - -(s32)signedNotePitch;
    } else {
        notePitch = signedNotePitch;
    }

    portamentoTime = arg0->portamentoTime;
    if ((portamentoTime != (0, 0)) && (portamentoTime >= arg0->noteAgeTicks)) {
        portamentoStartPitch = arg0->portamentoStartPitch;
        if (1) {
            portamentoPitchStep = (notePitch - portamentoStartPitch) / (f32)portamentoTime;
            portamentoPitchStep *= arg0->noteAgeTicksF;
            notePitch = portamentoStartPitch + portamentoPitchStep;
        }
    }

    sequencePitchOffset = (f32)arg0->pitchOffset * (f32)(1 - arg0->skipPitchOffsetOnce);
    arg0->currentNotePitch = notePitch;
    arg0->skipPitchOffsetOnce = 0;
    notePitch += arg0->vibratoPitchOffset + sequencePitchOffset + arg0->finePitchOffset + (f32)arg0->pitchPulseOffset;
    notePitch += gSoundPlayerPitchOffsets[arg0->instrumentIndex];
    notePitch = (f32)((f64)notePitch + ((pitchBendScale = (f64)(f32)arg0->pitchBendDepth * 0.015625) * ((f32)arg0->pitchBendValue - 64.0)));
    if (1) {}
    if (1) {}
    if (1) {}
    if (1) {}
    notePitch += arg0->handlePitchOffset;

    if (notePitch != arg0->cachedPitch) {
        arg0->cachedPitch = notePitch;
        pitchRatio = __MusIntPowerOf2((f32)((f64)notePitch * 0.083333333333333329));
        if (pitchRatio < 0.0f) {
            pitchRatio = 0.0f;
        }
        if (pitchRatio > 2.0) {
            pitchRatio = 2.0f;
            arg0->unk108 = 0;
        }
        alSynSetPitch(&gAudioSynthesizer, (ALVoice *)(mus_voices + (arg1 * 0x1C)), pitchRatio);
    }
}
#endif

void __MusIntInitEnvelope(PlayerCommandState *arg0) {
    u16 temp_v0;

    if (arg0->unkBC != 0x7FFF) {
        temp_v0 = arg0->unkC2;
        if (temp_v0 != 0) {
            arg0->unk100 = arg0->unk10 + (temp_v0 << 8);
        } else {
            arg0->unk100 = arg0->unkC - (arg0->unkC4 << 8);
        }
    } else {
        arg0->unk100 = 0x7FFFFFFF;
    }

    arg0->unkF9 = arg0->unkF5;
    arg0->unkFA = arg0->unkF4;
    arg0->unkF8 = 1;
}

// __MusIntProcessEnvelope best match: 94.856% (nonmatchings/__MusIntProcessEnvelope-8075865578671233833/base_11.c)

#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntProcessEnvelope.s")

#ifdef NON_MATCHING
void __MusIntProcessEnvelope(PlayerCommandState *arg0) {
    u8 state;
    s32 tick;
    PlayerCommandState *new_var;
    u8 new_var4;
    u8 rate;
    PlayerCommandState *new_var2;
    float new_var3;
    u32 step;
    u8 value;
    f32 temp;

    new_var2 = arg0;
    state = new_var2->unkF8;
    if (state) {
        if (((u32)arg0->unk0 >= (u32)arg0->unk100) && (state < 4)) {
            step = 4;
            arg0->unkF8 = step;
            arg0->unkFA = 1;
            arg0->unk104 = arg0->unkF9;
        }
        tick = arg0->unkFA - 1;
        arg0->unkFA = tick;
        if ((tick & 0xFF) != 0) {
            return;
        }
        rate = arg0->unkF4;
        state = arg0->unkF8;
        arg0->unkFA = rate;
        switch (state) {
            case 1:
                step = ((u32)(arg0->unk0 - arg0->unk10)) >> 8;
                step = step / rate;
                if (step < arg0->unkFB) {
                    value = arg0->unkF5 ^ 0;
                    arg0->unkF9 = (s32)((f32)value + ((arg0->unkF6 - value) * arg0->unk30 * (f32)step));
                    return;
                }
                arg0->unkF8 = state;
                arg0->unkF8 = arg0->unkF8 + 1;
                arg0->unkF9 = arg0->unkF6;
                return;
            case 2:
                step = (((u32)(arg0->unk0 - arg0->unk10) >> 8) - arg0->unkFB) / rate;
                new_var4 = arg0->unkFC;
                if (step < new_var4) {
                    value = arg0->unkF6;
                    arg0->unkF9 = (s32)((f32)value + ((arg0->unkF7 - value) * arg0->unk34 * (f32)step));
                    return;
                }
                arg0->unkF8 = state + 1;
                arg0->unkF9 = arg0->unkF7;
                return;
            case 3:
                return;
            case 4:
                step = ((u32)(arg0->unk0 - arg0->unk100) >> 8) / rate;
                new_var = arg0;
                if (step < arg0->unkFD) {
                    value = new_var->unk104;
                    temp = value;
                    new_var3 = temp * new_var->unk38;
                    new_var->unkF9 = (s32)(temp - (new_var3 * (f32)step));
                    return;
                }
                new_var->unkF8 = state + 1;
                arg0->unkF9 = (((((u32)(arg0->unk0 - arg0->unk10)) >> 8) - new_var->unkFB) / rate) * 0;
                break;
        }
    }
}
#endif

void __MusIntProcessWobble(PlayerCommandState *arg0) {
    u8 temp_t7;
    u8 temp_v0;

    if (1) {
        temp_t7 = (arg0->unk107 & 0xFFFFU) - 1;
    }
    arg0->unk107 = temp_t7;
    if ((temp_t7 & 0xFF) == 0) {
        temp_v0 = arg0->unkEB;
        if (temp_v0 != 0) {
            if (arg0->pitchPulseOffset == 0) {
                arg0->unk107 = temp_v0;
                arg0->pitchPulseOffset = arg0->unk119;
                return;
            }
            arg0->pitchPulseOffset = 0;
            arg0->unk107 = arg0->unk106;
        }
    }
}

void __MusIntProcessVibrato(PlayerCommandState *arg0) {
    s32 temp_v1;
    register f32 temp_fv1;
    u8 temp_v0;

    temp_v0 = arg0->unkE9;
    if (temp_v0 != 0) {
        temp_v1 = arg0->noteAgeTicks - arg0->unk105;
        if (temp_v1 > 0) {
            temp_fv1 = sinf((f32)((f64)((temp_v1 / (f32)temp_v0) * ((float)2.0)) * 3.1415926000000001)) * arg0->vibratoDepth;
            arg0->vibratoPitchOffset = temp_fv1;
        }
    }
}

// __MusIntProcessContinuousVolume best match: 99.608% (nonmatchings/__MusIntProcessContinuousVolume-6934502587000073416/base_13.c)
#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntProcessContinuousVolume.s")

#ifdef NON_MATCHING
void __MusIntProcessContinuousVolume(PlayerCommandState *arg0) {
    short high_mask;
    u16 temp_c8;
    s32 one = 1;
    register u8 *temp_v1;
    int new_var;
    register u8 temp_v0;
    register u8 *temp_t1;
    short temp_s16;
    register u8 temp_v0_2;
    register u8 *temp_t3;
    register u16 temp_t6;
    u16 new_var2;
    register unsigned int temp_t9;
    register u16 temp_t0;

    if ((u32)arg0->unk4 < (u32)arg0->unk0) {
        do {
            temp_s16 = (temp_c8 = arg0->unkC8);
            temp_t9 = temp_s16 - one;
            arg0->unk4 = arg0->unk4 + 0x100;
            arg0->unkC8 = temp_t9;
            temp_t0 = (temp_t9 ^ (temp_v0 = 0)) & 0xFFFF;
            if (temp_t0 == 0) {
                temp_t1 = (u8 *)arg0->unk60;
                temp_v1 = temp_t1;
                temp_v0 = *temp_v1;
                arg0->unk60 = (s32)(temp_v1 + one);
                if ((s32)temp_v0 >= 0x80) {
                    temp_t6 = (arg0->unkEF = temp_v0 & 0x7F);
                    temp_t1 = (u8 *)arg0->unk60;
                    temp_v0_2 = *temp_t1;
                    do {
                        ;
                        temp_t3 = (temp_t1 += one);
                        arg0->unk60 = (s32)temp_t3;
                    } while (0);
                    high_mask = temp_v0_2 & 0x7F;
                    temp_t0 = one;
                    if ((s32)temp_v0_2 >= 0x80) {
                        new_var2 = (temp_t6 = high_mask << 8);
                        temp_s16 = new_var2;
                        arg0->unkC8 = temp_t6;
                        arg0->unkC8 = temp_t6 + *temp_t3 + (new_var = 2);
                        arg0->unk60 = (s32)(temp_t3 + temp_t0);
                    } else {
                        arg0->unkC8 = temp_v0_2 + 2;
                    }
                } else {
                    arg0->unkEF = temp_v0;
                    arg0->unkC8 = one;
                }
            }
        } while ((u32)arg0->unk4 < (u32)arg0->unk0);
    }
}
#endif

void __MusIntProcessContinuousPitchBend(PlayerCommandState *arg0) {
    u16 temp_ca;
    s32 one = 1;
    int new_var;
    register u8 temp_v0;
    register u8 temp_t2;
    register u8 temp_t4;
    short temp_s16;
    register u16 temp_t6;
    register int temp_t9;
    register u16 temp_t0;

    if ((u32)arg0->unk8 < (u32)arg0->unk0) {
        do {
            temp_t9 = (short)(temp_ca = arg0->unkCA) - one;
            arg0->unk8 = arg0->unk8 + 0x100;
            arg0->unkCA = temp_t9;
            temp_t0 = (temp_t9 ^ (temp_v0 = 0)) & 0xFFFF;
            if (!temp_t0) {
                temp_v0 = *arg0->unk68++;
                if ((s32)temp_v0 >= 0x80) {
                    temp_t2 = temp_v0 & 0x7F;
                    arg0->pitchBendValue = temp_t2;
                    temp_v0 = *arg0->unk68++;
                    temp_t0 = one;
                    if ((s32)temp_v0 >= 0x80) {
                        temp_t4 = temp_v0 & 0x7F;
                        temp_s16 = temp_t4 << 8;
                        temp_t6 = temp_s16;
                        arg0->unkCA = (short)temp_t6;
                        arg0->unkCA = temp_t6 + *arg0->unk68++ + (new_var = 2);
                    } else {
                        arg0->unkCA = temp_v0 + 2;
                    }
                } else {
                    arg0->pitchBendValue = temp_v0;
                    arg0->unkCA = one;
                }
            }
        } while ((u32)arg0->unk8 < (u32)arg0->unk0);
    }
}

f32 __MusIntPowerOf2(f32 arg0) {
    f32 square;
    f32 fourth;

    if (arg0 == 0.0f) {
        return 1.0f;
    }

    if (arg0 > 0.0f) {
        square = arg0 * arg0;
        fourth = square * square;
        return (f32)(((f64)arg0 * 0.69314718055994495) + 1.0 +
            ((f64)square * 0.240226506959101) +
            ((f64)(square * arg0) * 0.055504108664821597) +
            ((f64)fourth * 0.0096181291076284803) +
            ((f64)(fourth * arg0) * 0.00133335581464284) +
            ((f64)(fourth * square) * 0.00015403530393381601));
    }

    arg0 = -arg0;
    square = arg0 * arg0;
    fourth = square * square;
    return (f32)(1.0 / (((f64)arg0 * 0.69314718055994495) + 1.0 +
        ((f64)square * 0.240226506959101) +
        ((f64)(square * arg0) * 0.055504108664821597) +
        ((f64)fourth * 0.0096181291076284803) +
        ((f64)(fourth * arg0) * 0.00133335581464284) +
        ((f64)(fourth * square) * 0.00015403530393381601)));
}

void __MusIntRemapPtrBank(PlayerCommandBank *bank, s32 sampleBaseOffset) {
    s32 i;
    ALWaveTable *wave;

    gSoundBankEntryCount = (gCurrentSoundBank = bank)->waveCount;
    gSoundWaveTable = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1,
                                   gSoundBankEntryCount * sizeof(*gSoundWaveTable));

    i = 0;
    if (gSoundBankEntryCount > 0) {
        do {
            gSoundWaveTable[i] =
                (ALWaveTable *)(gCurrentSoundBank->waveTableOffsets[i] + (s32)gCurrentSoundBank);
            wave = gSoundWaveTable[i];
            if (wave->flags == 0) {
                wave->base += sampleBaseOffset;
                gSoundWaveTable[i]->flags = 1;
                wave = gSoundWaveTable[i];
                if (wave->waveInfo.adpcmWave.loop != 0) {
                    wave->waveInfo.adpcmWave.loop =
                        (ALADPCMloop *)((s32)wave->waveInfo.adpcmWave.loop + (s32)gCurrentSoundBank);
                    wave = gSoundWaveTable[i];
                }
                if (wave->type == AL_ADPCM_WAVE) {
                    wave->waveInfo.adpcmWave.book =
                        (ALADPCMBook *)((s32)wave->waveInfo.adpcmWave.book + (s32)bank);
                }
            }
            i++;
        } while (i < gSoundBankEntryCount);
    }
    osWritebackDCacheAll();
}

s32 __MusIntRandom(s32 arg0) {
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_v0;
    f32 temp_f0;
    f32 temp_f10;

    for (var_v0 = 0; var_v0 != 8;) {
        var_v0 += 4;
        temp_v1 = mus_random_seed & 0x48000000;
        mus_random_seed *= 2;
        if ((temp_v1 == 0x48000000) || (temp_v1 == 0x08000000)) {
            mus_random_seed |= 1;
        }
        temp_v1_2 = mus_random_seed & 0x48000000;
        mus_random_seed *= 2;
        if ((temp_v1_2 == 0x48000000) || (temp_v1_2 == 0x08000000)) {
            mus_random_seed |= 1;
        }
        temp_v1_3 = mus_random_seed & 0x48000000;
        mus_random_seed *= 2;
        if ((temp_v1_3 == 0x48000000) || (temp_v1_3 == 0x08000000)) {
            mus_random_seed |= 1;
        }
        temp_v1_4 = mus_random_seed & 0x48000000;
        mus_random_seed *= 2;
        if ((temp_v1_4 == 0x48000000) || (temp_v1_4 == 0x08000000)) {
            mus_random_seed |= 1;
        }
    }

    temp_f0 = (f32)mus_random_seed / 65536.0f;
    temp_f10 = (f32)arg0;
    temp_f0 = temp_f0 / 65536.0f;
    return (s32)(temp_f10 * temp_f0);
}

void __MusIntInitialiseChannel(PlayerCommandState *arg0) {
    u8 temp_v0;
    PlayerCommandClearBlock *var_v1;
    s32 var_a1;
    u16 temp_t9;

    temp_v0 = arg0->unkE4;
    arg0->sequencePos = 0;

    var_v1 = (PlayerCommandClearBlock *)arg0;
    var_a1 = 0;
loop:
    var_a1 += 4;
    var_v1->unk1 = 0;
    var_v1->unk2 = 0;
    var_v1->unk3 = 0;
    var_v1++;
    var_v1[-1].unk0 = 0;
    if (var_a1 != sizeof(PlayerCommandState)) {
        goto loop;
    }

    arg0->unkB6 = 0xFFFF;
    arg0->unkE2 = 0xFF;
    arg0->unkE3 = 0xFF;
    arg0->cachedPitch = 99.9f;
    temp_t9 = 0x6000 / mus_vsyncs_per_second;
    arg0->unkBC = 1;
    arg0->unkED = 0;
    arg0->unkEE = 0x7F;
    arg0->unkEF = 0x7F;
    arg0->pitchBendValue = 0x40;
    arg0->pitchBendDepth = 2;
    *(u8 *)&arg0->unkF2 = 0x40;
    arg0->unkC8 = 1;
    arg0->unkCA = 1;
    arg0->fadeTarget = -1;
    arg0->unkB0 = 0x80;
    arg0->unkBA = temp_t9;
    arg0->unkB8 = temp_t9;
    arg0->unkB2 = 0x80;
    arg0->unkB4 = 0x80;
    arg0->unkE4 = temp_v0;
    arg0->soundId = 0;
}

s32 __MusIntFindChannel(s32 arg0, s32 arg1) {
    s32 i;
    PlayerCommandState *entry;

    entry = mus_channels;
    i = 0;
    if (max_channels > 0) {
        do {
            if (entry->sequencePos == 0) {
                return i;
            }
            i++;
            entry++;
        } while (i < max_channels);
    }
    return -1;
}

void __MusIntMemSet(u8 *p, unsigned char c, s32 n) {
    u8 *q = p;

    while (n--) {
        *(q++) = c;
    }
}

s32 __MusIntStartEffect(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v1;

    __MusIntInitialiseChannel(arg0);
    arg0->soundId = arg1;
    arg0->unkB0 = arg2;
    arg0->unkB2 = arg3;
    arg0->id = mus_current_handle;
    mus_current_handle += 1;
    arg0->unk20 = arg4;
    temp_v1 = libmus_fxheader_current[arg1];
    arg0->restartPos = temp_v1;
    arg0->sequencePos = temp_v1;
    return arg0->id;
}
