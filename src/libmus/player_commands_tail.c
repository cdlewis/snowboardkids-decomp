s32 Freverb(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkF3 = *arg1;
    return (s32)(arg1 + 1);
}
s32 FrandNote(PlayerCommandState *arg0, u8 *arg1) {
    u8 range;

    range = *arg1;
    arg1++;
    arg0->pitchOffset = __MusIntRandom(range, arg1, arg0);
    arg0->pitchOffset += *arg1;
    return (s32)(arg1 + 1);
}

s32 FrandVolume(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s32 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = __MusIntRandom(temp_a0, arg1, arg0);
    arg0->unkEF = temp_v0;
    arg0->unkEF = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 FrandPan(PlayerCommandState *arg0, u8 *arg1) {
    u8 temp_a0;
    s8 temp_v0;

    temp_a0 = *arg1;
    arg1++;
    temp_v0 = __MusIntRandom(temp_a0, arg1, arg0);
    arg0->unkF2 = temp_v0;
    arg0->unkF2 = *arg1 + (temp_a0 = temp_v0);
    return (s32)(arg1 + 1);
}

s32 Fvolume(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unkEF = *arg1;
    return (s32)(arg1 + 1);
}

s32 Fstartfx(PlayerCommandState *arg0, u8 *arg1) {
    PlayerCommandState *entry;
    s32 i;
    s32 newId;
    s32 soundId;

    soundId = *arg1;
    arg1++;
    if (soundId >= 0x80) {
        soundId = *arg1 + ((soundId & 0x7F) << 8);
        arg1++;
    }

    newId = MusStartEffect2(soundId, arg0->unkEF, (u8)arg0->unkF2, 0, arg0->unk20++);
    arg0->unk20--;
    if (newId == 0) {
        return (s32)arg1;
    }

    i = 0;
    entry = mus_channels;
    if (max_channels > 0) {
        do {
            i++;
            if (newId == entry->id) {
                entry->id = arg0->id;
            }
            entry++;
        } while (i < max_channels);
    }
    return (s32)arg1;
}

s32 Fbendrange(PlayerCommandState *arg0, u8 *arg1) {
    arg0->pitchBendDepth = *arg1;
    return (s32)(arg1 + 1);
}
