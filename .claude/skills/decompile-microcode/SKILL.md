---
name: decompile-microcode
description: Use this skill if you need to decompile a function that uses F3DEX. The presence of the Gfx type or gRegionAllocPtr are strongly indicative of an F3DEX or S2DEX microcode function.
---

# Decompiling F3DEX code

`gRegionAllocPtr` is indicative of a dynamically constructed F3DEX display list. F3DEX display list commands are microcode that are passed to the RSP for rendering. They are all 8 bytes long.

Special care needs to be taken with functions constructing display lists as the original developer likely used specific macros from `include/PR/gbi.h` (compiled with `-DF3DEX_GBI`). These macro calls need to be replicated in order for the function to match.

The first step is to identify the commands in question, for example a naive decompilation might produce:

```
    temp_v1 = gRegionAllocPtr;
    temp_a1 = (s32)arg0->unk30;
    temp_v1->words.w0 = 0x01020040;
    temp_v1->words.w1 = temp_a1;
```

Here there is a single F3DEX command. Its `w0` starts with `0x01` — the F3DEX `G_MTX` opcode (F3DEX2's `G_MTX` is `0xDA`; do not confuse the two). We can use `gfxdis.f3dex` to identify it:

```
gfxdis.f3dex -d 0102004000000000
```

Which gives us:

```
gsSPMatrix(0x00000000, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
```

This is close to what we want. But it needs a few improvements:

1. We need the variant of this macro that also accepts a display list pointer: `gSPMatrix`.
2. We need to replace 0x00000000 with our actual variable: arg0->unk30.

This ultimately will give us:

```
gSPMatrix(gRegionAllocPtr++, (Mtx*)arg0->unk30, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
```

Note the inline increment of gRegionAllocPtr, this is very common in commands dealing with F3DEX microcode.

The mapping may not always be this clear, however. You can consult @f3dex-microcode-reference.md for a full list of F3DEX commands and `include/PR/gbi.h` for suitable macros to use.

## Aggregate macros

Be aware that some macros in `gbi.h` aggregate a number of regular F3DEX commands. You should always look for patterns of commands that could translate into one of these macros. The use of which is very likely to increase your match percentage. Common examples of this include:

* gDPLoadTLUT (and its variations gDPLoadTLUT*)
* gDPLoadTextureBlock (and its variations gDPLoadTextureBlock*)

But there are many others.

## Permuter

The permuter may sometimes be helpful but it is not aware of the macros in gbi.h and will often try to replace them with their underlying commands. This is not what you want. Do not use its results if it requires you to remove gbi.h macros.

You can use the --preserve-f3dex argument when calling the permuter to explicitly prevent it from expanding maros. This can be useful for fixing register assignment issues on very close matches.