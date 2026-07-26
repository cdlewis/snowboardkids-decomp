s32 MusInitialize(PlayerCommandInit *arg0) {
    ALVoiceConfig vc;
    ALSynConfig synConfig;
    AudioSynthInitConfig synthConfig;
    s32 i;

    max_channels = arg0->count;
    gSoundPlayerTuningTable = arg0->tuningTable;
    gSoundPlayerPitchOffsets = arg0->pitchOffsetTable;
    libmus_fxheader_current = arg0->fxHeader;
    gSoundPriorityTable = arg0->unk28;

    if (osTvType == 0) {
        mus_vsyncs_per_second = 50;
    } else {
        mus_vsyncs_per_second = 60;
    }

    __MusIntMemSet(arg0->heapBase, 0, arg0->heapLen);
    alHeapInit(&gSoundPlayerHeap, arg0->heapBase, arg0->heapLen);

    mus_voices = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1, max_channels * sizeof(ALVoice));
    mus_channels = alHeapDBAlloc(0, 0, &gSoundPlayerHeap, 1, max_channels * sizeof(PlayerCommandState));

    __MusIntMemSet((u8 *)mus_voices, 0, max_channels * sizeof(ALVoice));
    __MusIntMemSet(mus_channels, 0, max_channels * sizeof(PlayerCommandState));

    synConfig.maxVVoices = max_channels;
    synConfig.maxPVoices = max_channels;
    synConfig.maxUpdates = arg0->maxUpdates;
    synConfig.dmaproc = 0;
    synConfig.fxType = 2;
    synConfig.outputRate = 0;
    synConfig.heap = &gSoundPlayerHeap;

    synthConfig.outputRate = arg0->maxFXBusses;
    synthConfig.frameRate = arg0->unk38;
    synthConfig.commandListSize = arg0->unk34;

    initAudioSynthesizer(arg0->unk4, &synConfig, arg0->outputRate, &synthConfig,
                  arg0->unk3C, arg0->unk40, mus_vsyncs_per_second);
    __MusIntRemapPtrBank(arg0->soundBank, arg0->sampleBaseOffset);
    MusSetMasterVolume(3, 0x7FFF);

    mus_next_frame_time = 0;
    mus_current_handle = 1;
    mus_random_seed = 0x12345678;

    plr_player.next = 0;
    plr_player.handler = __MusIntMain;
    plr_player.clientData = &plr_player;
    alSynAddPlayer(&gAudioSynthesizer, &plr_player);

    for (i = 0; i < max_channels; i++) {
        mus_channels[i].unkE4 = 0;
        __MusIntInitialiseChannel(&mus_channels[i]);
        vc.unityPitch = 0;
        vc.priority = arg0->outputRate;
        vc.fxBus = 0;
        alSynAllocVoice(&gAudioSynthesizer, &mus_voices[i], &vc);
    }

    return gSoundPlayerHeapEnd - gSoundPlayerHeap.base;
}

void MusSetMasterVolume(s32 arg0, s32 arg1) {
    s32 temp_t7 = arg0 & 2;

    if (arg0 & 1) {
        mus_master_volume_effects = arg1;
    }
    if (temp_t7) {
        mus_master_volume_songs = arg1;
    }
}

s32 MusStartSong(PlayerCommandData *arg0) {
    s32 i;
    int handle;
    s32 needed;
    s32 value;
    s32 index;
    s32 *ptr;
    PlayerCommandState *state;
    u8 *position;
    s32 free;
    s32 trackCount;

    ptr = (s32 *)arg0++;
    arg0 = (PlayerCommandData *)ptr;
    trackCount = arg0->trackCount;
    if ((u32)arg0->sequenceOffsets < 0x400U) {
        ptr = (s32 *)&arg0->sequenceOffsets;
        for (i = 0; i < (trackCount * 3) + 5; i++) {
            value = ptr[i];
            if (value != 0) {
                ptr[i] = value;
                ptr[i] = ptr[i] + (s32)arg0;
            }
        }
    }

    needed = 0;
    for (i = 0; i < trackCount; i++) {
        if (arg0->sequenceOffsets[i] != 0) {
            needed++;
        }
    }

    free = 0;
    state = mus_channels;
    for (i = 0; i < max_channels; i++) {
        if (state->sequencePos == 0) {
            free++;
        }
        state++;
    }

    if (free < needed) {
        return 0;
    }

    value = mus_current_handle;
    handle = value;
    mus_current_handle = handle + 1;
    for (i = 0; i < trackCount; i++) {
        value = (s32)arg0;
        if (arg0->sequenceOffsets[i] != 0) {
            index = __MusIntFindChannel(value, i);
            if (index == -1) {
                rmonPrintf(D_800E1A60);
            }
            state = &mus_channels[index];
            __MusIntInitialiseChannel(state);
            state->data = arg0;
            position = arg0->unk8[i];
            state->unk64 = position;
            state->unk60 = position;
            position = arg0->unkC[i];
            state->unk6C = position;
            state->unk68 = position;
            position = arg0->sequenceOffsets[i];
            state->restartPos = position;
            state->sequencePos = position;
            state->id = handle;
        }
    }

    return handle;
}

s32 MusStartEffect(s32 arg0) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;

    value = gSoundPriorityTable[arg0];
    entry = mus_channels;
    i = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (entry->sequencePos == 0) {
                return __MusIntStartEffect(entry, arg0, 0x80, 0x80, value);
            }
            entry++;
        } while (i < max_channels);
    }
    return 0;
}

s32 MusStartEffect2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    PlayerCommandState *entry;

    if (arg4 == -1) {
        arg4 = gSoundPriorityTable[arg0];
    }

    if (arg3 != 0) {
        entry = mus_channels;
        i = 0;
        if (max_channels > 0) {
            do {
                i++;
                if (arg0 == entry->soundId) {
                    return __MusIntStartEffect(entry, arg0, arg1, arg2, arg4);
                }
                entry++;
            } while (i < max_channels);
        }
    }

    entry = mus_channels;
    i = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (entry->sequencePos == 0) {
                return __MusIntStartEffect(entry, arg0, arg1, arg2, arg4);
            }
            entry++;
        } while (i < max_channels);
    }
    return 0;
}

void MusStop(s32 arg0, s32 arg1) {
    s32 i;
    s32 value;
    PlayerCommandState *entry;

    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = mus_channels;
    i = 0;
    entry = mus_channels;
    if (max_channels > 0) {
        do {
            ;
            i++;
            if (((entry->soundId != 0) && (arg0 & 1)) ||
                ((entry->soundId == 0) && (arg0 & 2))) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
            }
            entry++;
        } while (i < max_channels);
    }
}

s32 MusAsk(s32 arg0) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;
    volatile char flags;
    u16 temp_a0;

    i = 0;
    entry = mus_channels;
    ;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (entry->sequencePos != 0) {
                temp_a0 = entry->soundId;
                if (((temp_a0 != 0) && (arg0 & 1)) || ((((unsigned long) temp_a0) == 0) && (arg0 & 2))) {
                    matches++;
                }
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}

s32 MusHandleStop(s32 arg0, s32 arg1) {
    s32 count;
    s32 i;
    s32 value;
    PlayerCommandState *entry;

    value = arg0 == 0;
    i = 0;
    if (value) {
        return 0;
    }
    if (arg1 != 0) {
        value = arg1;
    } else {
        value = 1;
    }
    entry = mus_channels;
    count = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->fadeTarget = arg1;
                entry->fadeTime = value;
                count++;
            }
            entry++;
        } while (i < max_channels);
    }
    return count;
}

s32 MusHandleAsk(s32 arg0) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = mus_channels;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                matches++;
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}

s32 MusHandleSetVolume(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = mus_channels;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB0 = arg1;
                matches++;
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}

s32 MusHandleSetPan(s32 arg0, s32 arg1) {
    s32 i;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    i = 0;
    entry = mus_channels;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB2 = arg1;
                matches++;
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}

s32 MusHandleSetFreqOffset(s32 arg0, f32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    entry = mus_channels;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->handlePitchOffset = arg1;
                matches++;
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}

s32 MusHandleSetTempo(s32 arg0, s32 arg1) {
    s32 i = 0;
    s32 matches;
    PlayerCommandState *entry;

    if (arg0 == 0) {
        return 0;
    }

    if (arg1 <= 0) {
        arg1 = 1;
    } else if (arg1 >= 0x101) {
        arg1 = 0x100;
    }

    entry = mus_channels;
    matches = 0;
    if (max_channels > 0) {
        do {
            i++;
            if (arg0 == entry->id) {
                entry->unkB4 = arg1;
                matches++;
                entry->unkB8 = (s32)(entry->unkBA * arg1) >> 7;
            }
            entry++;
        } while (i < max_channels);
    }
    return matches;
}
