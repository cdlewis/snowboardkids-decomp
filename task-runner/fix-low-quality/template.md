# Cleanup Function

## Instructions

Look at `$INPUT[1]` in `$INPUT[0]`. Think about what its purpose is in the broader project and how it relates to other functions as a caller or callee.

Think about what the original developers would have written. This means proper structs, proper struct and array accesses, proper variable names, and clear function-local intent.

Make a list of the problems with the current function. Try practical strategies to fix these problems while preserving the 100% match.

It may require several iterations to get this balance right. This is expected.

It may require refactoring nearby project code. This is expected when shared structs or declarations are wrong or incomplete. Do not give up just because the necessary changes cross file boundaries.

Once you have cleaned up the function, run the following command to verify you have not broken the build:

```bash
./tools/build-and-verify.sh
```

If the build fails, fix it while preserving your cleanup. This may require several iterations.

Commit your changes when you are done.

## Unable To Clean Up The Function

Make several attempts to clean up the function without breaking the build. If you are unable to make progress after 30 attempts, revert all of your changes so the build remains clean, then exit.

## Safe Refactoring

### Identify Structs And Arrays

There are many kinds of refactoring that are safe to perform without breaking the match.

The most common example is swapping pointer arithmetic for proper struct or array types. For example:

```c
*(s16 *)((u8 *)arg0 + 0x28) -= 1;
```

Could often be rewritten as:

```c
arg0->unk28 -= 1;
```

You need to provide a suitable type definition for `arg0`.

Similarly:

```c
temp_a0 = (s8 *)(offset + (s32)arg0->entries);
```

Could be rewritten as an array access:

```c
temp_a0 = arg0->entries[i];
```

In the above example, if `offset` is computed as `0x10 * i`, that indicates the array elements are likely 0x10-byte structs.

### Local Variable Names

Local variables within a function can be safely renamed without affecting the match. Use this to communicate the intent of the function once you understand it.

### Global Variable Names

This is safe as long as you use the rename-global skill.

### Creating New Structs

Refactoring might be difficult because the file reuses a shared struct from elsewhere in the project. This reuse may be appropriate, but it may also be incorrect and can be what prevents proper cleanup.

When you create or modify a struct, verify nearby functions that access the same data still compile and match. Do not replace real types with `void *` just to make the compiler accept the code.
