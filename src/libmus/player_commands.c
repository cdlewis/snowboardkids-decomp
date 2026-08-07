const char D_800E1A60[16] = "NG Channel\n";

u8 *Fstop(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unk60 = 0;
    arg0->unk68 = 0;
    arg0->song_addr = 0;
    arg0->soundId = 0;
    arg0->id = 0;
    return 0;
}
void *Fwave(PlayerCommandState *arg0, u8 *arg1) {
    u16 v;
    u8 *new_var;

    v = *arg1;
    arg1++;
    if (v & 0x80) {
        v = ((v & 0x7F) << 1) << 7;
        v |= *(new_var = arg1);
        arg1++;
    }
    arg0->wave = v;
    return arg1;
}

s32 Fport(PlayerCommandState *arg0, u8 *arg1) {
    arg0->portamentoTime = *arg1;
    return (s32)(arg1 + 1);
}

s32 Fportoff(PlayerCommandState *arg0, s32 arg1) {
    arg0->portamentoTime = 0;
    return arg1;
}

s32 Fdefa(PlayerCommandState *arg0, u8 *arg1) {
    u32 value;

    value = *arg1++;
    if (value == 0) {
        value = 1;
    }
    arg0->unkF4 = value;
    arg0->env_init_vol = arg1[0];
    value = arg1[1];
    arg1 += 2;
    if (value == 0) {
        value = 1;
    }
    arg0->unkFB = value;
    arg0->unk30 = (f32)(1.0 / (f64)(f32)value);

    arg1 += 2;
    arg0->unkF6 = arg1[-2];
    value = arg1[-1];
    if (value == 0) {
        value = 1;
    }
    arg0->unkFC = value;
    arg0->unk34 = (f32)(1.0 / (f64)(f32)value);

    arg1 += 2;
    arg0->unkF7 = arg1[-2];
    value = arg1[-1];
    if (value == 0) {
        value = 1;
    }
    arg0->unkFD = value;
    arg0->unk38 = (f32)(1.0 / (f64)(f32)value);

    return (s32)arg1;
}

s32 Ftempo(PlayerCommandState *arg0, u8 *arg1) {
    s32 i;
    PlayerCommandState *entry;
    s32 value;
    s32 scaled;

    value = (*arg1 * 0x6000) / 120 / mus_vsyncs_per_second;
    scaled = 7;
    scaled = (arg0->unkB4 * value) >> scaled;
    arg1++;
    if (arg0->soundId != 0) {
        do {
            arg0->unkB8 = value;
        } while (0);
    } else {
        i = 0;
        entry = mus_channels;
        if (max_channels > 0) {
            do {
                i++;
                if (entry->song_addr == arg0->song_addr) {
                    entry->unkBA = value;
                    entry->unkB8 = scaled;
                }
                entry++;
            } while (i < max_channels);
        }
    }
    return (s32)arg1;
}

s32 Fendit(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;

    arg0->unkC4 = *arg1;
    ;
    ret = (temp = arg1) + 1;
    arg0->unkC2 = 0;
    arg1++;
    arg1--;
    return (s32)ret;
}

s32 Fcutoff(PlayerCommandState *arg0, u8 *arg1) {
    u8 *ret;
    u8 *temp;
    u32 v;
    short new_var;

    new_var = arg1[0] << 8;
    new_var = arg1[1] | ((0, new_var));
    v = new_var;
    ret = (temp = arg1) + 2;
    arg0->unkC2 = v;
    arg0->unkC4 = 0;
    arg1 += 2;
    arg1 -= 2;
    return (s32)ret;
}

s32 Fvibup(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->vibratoDepth = (f32)(((double)var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 Fvibdown(PlayerCommandState *arg0, u8 *arg1) {
    u32 temp_t8;
    f32 var_ft1;

    arg0->unk105 = arg1[0];
    arg0->unkE9 = arg1[1];
    arg1 += 2;
    temp_t8 = arg1[0];
    var_ft1 = (f32)(temp_t8 & 0xFFFF);
    arg0->vibratoDepth = (f32)(((double)-var_ft1) / 50.0);
    return (s32)(arg1 + 1);
}

s32 Fviboff(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkE9 = 0;
    arg0->vibratoPitchOffset = 0.0f;
    return arg1;
}

s32 Flength(PlayerCommandState *arg0, u8 *arg1) {
    u8 b;
    unsigned int new_var;
    s16 value;

    b = arg1[0];
    arg1++;
    if (b < 0x80) {
        arg0->fixed_length = b;
    } else {
        value = ((b & 0x7F) << 6) << 2;
        arg0->fixed_length = value;
        new_var = arg1[0];
        b = new_var;
        arg0->fixed_length = value + b;
        arg1++;
    }
    return (s32)arg1;
}

s32 Fignore(PlayerCommandState *arg0, s32 arg1) {
    arg0->ignore = 1;
    return arg1;
}

s32 Ftrans(PlayerCommandState *arg0, u8 *arg1) {
    arg0->pitchOffset = *arg1;
    return (s32)(arg1 + 1);
}

s32 Fignore_trans(PlayerCommandState *arg0, s32 arg1) {
    arg0->skipPitchOffsetOnce = 1;
    return arg1;
}

s32 Fdistort(PlayerCommandState *arg0, u8 *arg1) {
    s32 b;
    f32 f;
    b = *arg1;
    arg1++;
    if (b & 0x80) {
        b |= 0xFFFFFF00;
    }
    f = (f32)b;
    arg0->finePitchOffset = f / 100.0;
    return (s32)arg1;
}

s32 Fenvelope(PlayerCommandState *arg0, u8 *arg1) {
    s32 var_v0;

    var_v0 = arg1[0];
    arg1 += 1;
    if (var_v0 & 0x80) {
        var_v0 = arg1[0] | ((0, (var_v0 & 0x7F) << 8));
        arg1 += 1;
    }
    Fdefa(arg0, arg0->song_addr->commands + (var_v0 * 7));
    return (s32)arg1;
}

s32 Fenvoff(PlayerCommandState *arg0, s32 arg1) {
    arg0->env_trigger_off = 1;
    return arg1;
}

s32 Fenvon(PlayerCommandState *arg0, s32 arg1) {
    arg0->env_trigger_off = 0;
    return arg1;
}

s32 Ftroff(PlayerCommandState *arg0, s32 arg1) {
    arg0->trigger_off = 1;
    return arg1;
}

s32 Ftron(PlayerCommandState *arg0, s32 arg1) {
    arg0->trigger_off = 0;
    return arg1;
}

s32 Ffor(PlayerCommandState *arg0, u8 *arg1) {
    u8 depth;

    depth = arg0->returnDepth;
    arg0->returnUnk109[depth] = *arg1++;
    arg0->returnScripts[depth] = arg1;
    arg0->returnUnk60[depth] = arg0->unk60;
    arg0->returnUnk68[depth] = arg0->unk68;
    arg0->returnUnk10E[depth] = arg0->unkEF;
    arg0->returnUnk113[depth] = arg0->pitchBendValue;
    arg0->returnUnkC8[depth] = arg0->unkC8;
    arg0->returnUnkCA[depth] = arg0->unkCA;
    arg0->returnDepth++;
    return (s32)arg1;
}

s32 Fnext(PlayerCommandState *arg0, s32 arg1) {
    s32 depth;
    u8 *countPtr;
    u8 count;
    u8 nextCount;

    depth = arg0->returnDepth - 1;
    countPtr = &arg0->returnUnk109[depth];
    count = *countPtr;
    nextCount = count - 1;
    if (count != 0xFF) {
        *countPtr = nextCount;
        if ((nextCount & 0xFF) == 0) {
            arg0->returnDepth = depth;
            depth = -1;
        }
    }
    if (depth >= 0) {
        arg1 = (s32)arg0->returnScripts[depth];
        arg0->unk60 = arg0->returnUnk60[depth];
        arg0->unk68 = arg0->returnUnk68[depth];
        arg0->unkEF = arg0->returnUnk10E[depth];
        arg0->pitchBendValue = arg0->returnUnk113[depth];
        arg0->unkC8 = arg0->returnUnkC8[depth];
        arg0->unkCA = arg0->returnUnkCA[depth];
    }
    return arg1;
}

s32 Fwobble(PlayerCommandState *arg0, u8 *arg1) {
    arg0->unk119 = arg1[0];
    arg0->unkEB = arg1[1];
    arg0->wobble_off_speed = arg1[2];
    return (s32)(arg1 + 3);
}

s32 Fwobbleoff(PlayerCommandState *arg0, s32 arg1) {
    arg0->unkEB = 0;
    return arg1;
}

s32 Fvelon(PlayerCommandState *arg0, s32 arg1) {
    arg0->velocity_on = 1;
    return arg1;
}

s32 Fveloff(PlayerCommandState *arg0, s32 arg1) {
    arg0->velocity_on = 0;
    return arg1;
}

s32 Fvelocity(PlayerCommandState *arg0, u8 *arg1) {
    s32 ret;

    ret = (s32)(arg1 + 1);
    arg0->default_velocity = arg1[0];
    if (ret) {}
    arg0->velocity_on = 0;
    return ret;
}

s32 Fpan(PlayerCommandState *arg0, u8 *arg1) {
    arg0->pan = (s32)(u8)*arg1 / 2;
    return (s32)(arg1 + 1);
}

s32 Fstereo(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

void *Fdrums(void *arg0, u8 *arg1) {
    u8 *temp_a2;
    s32 phi;

    temp_a2 = *(u8 **)((s32)arg0 + 0x54);
    phi = (s32)temp_a2 + (*(s32 **)((s32)(*(u8 **)((s32)arg0 + 0x54)) + 0x14))[*arg1];
    *(s32 *)((s32)arg0 + 0x70) = phi;
    return arg1 + 1;
}

s32 Fdrumsoff(PlayerCommandState *arg0, s32 arg1) {
    arg0->pdrums = 0;
    return arg1;
}

s32 Fprint(s32 arg0, s32 arg1) {
    return arg1;
}
