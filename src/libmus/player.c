#include "game/audio/audio_engine_internal.h"

#define U8_TO_FLOAT(c) ((c) & 128) ? -(256 - (c)) : (c)

#include "player_commands.c"

u8 *Fgoto(PlayerCommandState *arg0, u8 *arg1) {
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

#pragma weak __MusIntRandomWithContext = __MusIntRandom
#include "player_commands_tail.c"
#include "player_api.c"

ALMicroTime __MusIntMain(void *arg0) {
    s32 i;
    PlayerCommandState *channel;

    for (i = 0, channel = mus_channels; i < max_channels; i++, channel++) {
        if (channel->sequencePos == 0) {
            continue;
        }

        channel->unk0 += (u16)channel->unkB8;
        if (channel->unkBC != 0x7FFF) {
            while ((u32)channel->unkC < (u32)channel->unk0 && channel->sequencePos != 0) {
                __MusIntGetNewNote(channel, i);
            }
        }
        if (channel->sequencePos == 0) {
            continue;
        }

        if (channel->unk60 != 0) {
            __MusIntProcessContinuousVolume(channel);
        }
        if (channel->unk68 != 0) {
            __MusIntProcessContinuousPitchBend(channel);
        }

        if (channel->fadeTarget != -1) {
            channel->fadeTarget--;
            if (channel->fadeTarget == -1) {
                channel->sequencePos = Fstop(channel, 0);
                if (channel->unkE4 != 0) {
                    channel->unkE4 = 0;
                    alSynStopVoice(&gAudioSynthesizer, &mus_voices[i]);
                }
            }
        }

        if (channel->unkE4 != 0) {
            __MusIntProcessEnvelope(channel);
            __MusIntProcessVibrato(channel);
            __MusIntProcessWobble(channel);
            __MusIntSetPitch(channel, i);
            __MusIntSetVolumeAndPan(channel, i);
        }

        channel->noteAgeTicks = (u32)(channel->unk0 - channel->unk10) >> 8;
        channel->noteAgeTicksF = channel->noteAgeTicks;
    }

    mus_next_frame_time++;
    return 1000000 / mus_vsyncs_per_second;
}

// __MusIntGetNewNote best match: 99.962% (nonmatchings/__MusIntGetNewNote-8498672362023432715/base_13.c)

#pragma GLOBAL_ASM("asm/nonmatchings/libmus/player/__MusIntGetNewNote.s")

#ifdef NON_MATCHING
typedef u8 *(*PlayerCommandHandler)(PlayerCommandState *, u8 *, s32);

extern PlayerCommandHandler jumptable[];
extern u8 mus_default_velocities[];

void __MusIntGetNewNote(PlayerCommandState *arg0, s32 arg1) {
    u8 *seq;
    u8 *durationPos;
    u8 velocity;
    u8 cmd;
    u8 fxMix;
    u32 durationValue;
    u16 baseDuration;
    int zero;
    u32 duration;
    int instrumentIndex;
    int velocityIndex;
    s32 soundIndex;

    seq = arg0->sequencePos;
    if (seq != NULL) {
        cmd = seq[0];
        while (cmd >= 0x80) {
            seq = jumptable[cmd & 0x7F](arg0, seq + 1, cmd);
            if (seq == NULL) {
                if (1) {
                }
                break;
            }
            cmd = seq[0];
        }
    }
    arg0->sequencePos = seq;

    if (seq != NULL) {
        arg0->portamentoStartPitch = arg0->currentNotePitch;
        cmd = (*seq) ^ 0;
        arg0->sequencePos = seq + 1;
        arg0->unkFE = cmd;

        if (arg0->unkED != 0) {
            arg0->unk108 = mus_default_velocities[*arg0->sequencePos++];
        } else {
            arg0->unk108 = mus_default_velocities[arg0->unkEE];
        }

        baseDuration = (u16)arg0->unkC0;
        if (baseDuration != 0) {
            durationValue = baseDuration;
            durationValue = durationValue & 0xFFFF;
            arg0->unkBC = durationValue;
            arg0->unk28 = (f32)durationValue;
        }

        zero = 0;
        if ((arg0->flagE6 != zero) || (baseDuration == 0)) {
            arg0->flagE6 = 0;
            cmd = *arg0->sequencePos++;
            if (cmd < 0x80) {
                arg0->unkBC = cmd;
                arg0->unk28 = (f32)cmd;
            } else {
                durationPos = (u8 *)arg0->sequencePos;
                duration = (((cmd & 0x7F) << 5) << 1) << 2;
                arg0->unkBC = duration;
                duration += *durationPos;
                arg0->unkBC = duration;
                arg0->unk28 = (duration & 0xFFFF) & 0xFFFFu;
                arg0->sequencePos = durationPos + 1;
            }
        }

        duration = arg0->unkC;
        arg0->unkC += arg0->unkBC << 8;
        arg0->noteAgeTicks = 0;
        arg0->pitchPulseOffset = 0;
        arg0->unk10 = duration;
        arg0->noteAgeTicksF = 0.f;
        arg0->unk107 = arg0->unk106;

        if (arg0->unkFE != 0) {
            if (arg0->jumpTarget != 0) {
                u8 *entry = (u8 *)arg0->jumpTarget + (arg0->unkFE * 4);

                arg0->instrumentIndex = entry[-0x30];
                arg0->unkF2 = entry[-0x2E] / 2;
                Fdefa(arg0, arg0->data->commands + (entry[-0x2F] * 7));
                arg0->unkFE = ((u8 *)arg0->jumpTarget + (arg0->unkFE * 4))[-0x2D];
            }

            if (gSoundBankEntryCount <= (instrumentIndex = arg0->instrumentIndex)) {
                soundIndex = 0;
            } else {
                soundIndex = instrumentIndex;
            }

            if (arg0->flagE5 == 0) {
                if (arg0->unkE4 != 0) {
                    alSynStopVoice(&gAudioSynthesizer, &mus_voices[arg1]);
                }
                arg0->unkE4 = 1;
                arg0->unkB6 = 0xFFFF;
                arg0->unkE3 = 0xFF;
                alSynStartVoice(&gAudioSynthesizer, &mus_voices[arg1],
                                gSoundWaveTable[soundIndex]);
            }

            arg0->notePitch = gSoundPlayerTuningTable[soundIndex] + arg0->unkFE - 5;
            if (arg0->flagE8 == 0) {
                arg0->unkF8 = 0;
                arg0->unkF9 = arg0->unkF5;
                __MusIntInitEnvelope(arg0);
            }
            __MusIntSetPitch(arg0, arg1);
            __MusIntSetVolumeAndPan(arg0, arg1);
            fxMix = arg0->unkF3;
            if (fxMix != arg0->unkE2) {
                arg0->unkE2 = fxMix;
                alSynSetFXMix(&gAudioSynthesizer, &mus_voices[arg1], fxMix);
            }
        } else if (arg0->unkF8 < 4) {
            arg0->unkF8 = 4;
            arg0->unk100 = arg0->unk0;
            arg0->unkFA = 1;
            arg0->unk104 = arg0->unkF9;
        }
    } else if (arg0->unkE4 != 0) {
        arg0->unkE4 = 0;
        alSynStopVoice(&gAudioSynthesizer, &mus_voices[arg1]);
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
        alSynSetVol(&gAudioSynthesizer, &mus_voices[arg1], (s16)volume, 0xF4240 / mus_vsyncs_per_second);
    }

    stopping = arg0->unkE3;
    oldPan = stopping;
    pan = (((*(u8 *)&arg0->unkF2) * arg0->unkB2) >> 7) & 0x7F;
    if (pan != oldPan) {
        arg0->unkE3 = pan;
        alSynSetPan(&gAudioSynthesizer, &mus_voices[arg1], pan & 0xFF);
    }
}

void __MusIntSetPitch(PlayerCommandState *arg0, s32 arg1) {
    f32 frequency;
    f32 temp;

    frequency = U8_TO_FLOAT(arg0->notePitch);
    if (arg0->portamentoTime != 0) {
        if (arg0->noteAgeTicks <= arg0->portamentoTime) {
            temp = (frequency - arg0->portamentoStartPitch) / (f32)arg0->portamentoTime;
            temp *= arg0->noteAgeTicksF;
            frequency = arg0->portamentoStartPitch + temp;
        }
    }
    arg0->currentNotePitch = frequency;
    frequency += arg0->vibratoPitchOffset +
                 (((f32)arg0->pitchOffset) * (1 - arg0->skipPitchOffsetOnce)) +
                 arg0->finePitchOffset +
                 ((f32)arg0->pitchPulseOffset);
    arg0->skipPitchOffsetOnce = 0;
    frequency += gSoundPlayerPitchOffsets[arg0->instrumentIndex];
    frequency += (f32)arg0->pitchBendDepth * (1.0 / 64.0) *
                 ((f32)arg0->pitchBendValue - 64.0);
    frequency += arg0->handlePitchOffset;
    if (frequency == arg0->cachedPitch) {
        return;
    }
    arg0->cachedPitch = frequency;
    frequency = __MusIntPowerOf2(frequency * (1.0 / 12.0));
    if (frequency < 0) {
        frequency = 0;
    }
    if (frequency > 2.0) {
        frequency = 2.0;
        arg0->unk108 = 0;
    }
    alSynSetPitch(&gAudioSynthesizer, &mus_voices[arg1], frequency);
}

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

void __MusIntProcessEnvelope(PlayerCommandState *arg0) {
    u32 env_phase_count;
    u8 phase;
    f32 initial_level;
    u8 rate;
    f32 envelope_value;
    u8 start_level;
    f32 release_amount;

    if (!arg0->unkF8) {
        return;
    }

    if ((arg0->unk0 >= arg0->unk100) && (arg0->unkF8 < 4)) {
        arg0->unkF8 = 4;
        arg0->unkFA = 1;
        arg0->unk104 = arg0->unkF9;
    }

    arg0->unkFA--;
    if (!arg0->unkFA) {
        rate = arg0->unkF4;
        phase = arg0->unkF8;
        arg0->unkFA = rate;
        switch (arg0->unkF8) {
            case 1:
                env_phase_count = (u32)(arg0->unk0 - arg0->unk10) >> 8;
                env_phase_count = env_phase_count / rate;
                if (env_phase_count < arg0->unkFB) {
                    start_level = arg0->unkF5;
                    envelope_value = arg0->unkF6 - start_level;
                    envelope_value *= arg0->unk30;
                    envelope_value *= (f32)env_phase_count;
                    initial_level = (f32)start_level;
                    arg0->unkF9 = (s32)(initial_level + envelope_value);
                    return;
                } else {
                    arg0->unkF8 = arg0->unkF8 + 1;
                    arg0->unkF9 = arg0->unkF6;
                    return;
                }
            case 2:
                env_phase_count = ((u32)(arg0->unk0 - arg0->unk10) >> 8) - arg0->unkFB;
                env_phase_count = env_phase_count / rate;
                if (env_phase_count < arg0->unkFC) {
                    envelope_value = arg0->unkF7 - arg0->unkF6;
                    envelope_value *= arg0->unk34;
                    envelope_value *= (f32)env_phase_count;
                    arg0->unkF9 = (s32)((f32)arg0->unkF6 + envelope_value);
                    return;
                } else {
                    arg0->unkF8 = arg0->unkF8 + 1;
                    arg0->unkF9 = arg0->unkF7;
                    return;
                }
            case 3:
                return;
            case 4:
                env_phase_count = (u32)(arg0->unk0 - arg0->unk100) >> 8;
                env_phase_count = env_phase_count / rate;
                if (env_phase_count < arg0->unkFD) {
                    release_amount = arg0->unk104;
                    release_amount *= arg0->unk38;
                    release_amount *= (f32)env_phase_count;
                    arg0->unkF9 = (s32)((f32)arg0->unk104 - release_amount);
                    return;
                } else {
                    arg0->unkF8 = arg0->unkF8 + 1;
                    arg0->unkF9 = 0;
                    return;
                }
        }
    }
}

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

void __MusIntProcessContinuousVolume(PlayerCommandState *arg0) {
    u16 temp_c8;
    s32 one = 1;
    int new_var;
    register u8 temp_v0;
    register u8 temp_t2;
    register u8 temp_t4;
    short temp_s16;
    register u16 temp_t6;
    register int temp_t9;
    register u16 temp_t0;

    if ((u32)arg0->unk4 < (u32)arg0->unk0) {
        do {
            temp_t9 = (short)(temp_c8 = arg0->unkC8) - one;
            arg0->unk4 = arg0->unk4 + 0x100;
            arg0->unkC8 = temp_t9;
            temp_t0 = (temp_t9 ^ (temp_v0 = 0)) & 0xFFFF;
            if (!temp_t0) {
                temp_v0 = *arg0->unk60++;
                if ((s32)temp_v0 >= 0x80) {
                    temp_t2 = temp_v0 & 0x7F;
                    arg0->unkEF = temp_t2;
                    temp_v0 = *arg0->unk60++;
                    temp_t0 = one;
                    if ((s32)temp_v0 >= 0x80) {
                        temp_t4 = temp_v0 & 0x7F;
                        temp_s16 = temp_t4 << 8;
                        temp_t6 = temp_s16;
                        arg0->unkC8 = (short)temp_t6;
                        arg0->unkC8 = temp_t6 + *arg0->unk60++ + (new_var = 2);
                    } else {
                        arg0->unkC8 = temp_v0 + 2;
                    }
                } else {
                    arg0->unkEF = temp_v0;
                    arg0->unkC8 = one;
                }
            }
        } while ((u32)arg0->unk4 < (u32)arg0->unk0);
    }
}

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

void __MusIntMemSet(void *p, unsigned char c, s32 n) {
    u8 *q = p;

    while (n--) {
        *(q++) = c;
    }
}

s32 __MusIntStartEffect(PlayerCommandState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *temp_v1;

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
