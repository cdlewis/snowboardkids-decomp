#ifndef COMPILER_DIAGNOSTICS_H
#define COMPILER_DIAGNOSTICS_H

#ifdef __clang__
#define CLANG_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#define CLANG_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#define CLANG_DIAGNOSTIC_IGNORE_RETURN_TYPE _Pragma("clang diagnostic ignored \"-Wreturn-type\"")
#define CLANG_DIAGNOSTIC_IGNORE_SELF_ASSIGN _Pragma("clang diagnostic ignored \"-Wself-assign\"")
#define CLANG_DIAGNOSTIC_IGNORE_UNINITIALIZED _Pragma("clang diagnostic ignored \"-Wuninitialized\"")
#define CLANG_DIAGNOSTIC_IGNORE_DEPRECATED_NON_PROTOTYPE _Pragma("clang diagnostic ignored \"-Wdeprecated-non-prototype\"")
#else
#define CLANG_DIAGNOSTIC_PUSH
#define CLANG_DIAGNOSTIC_POP
#define CLANG_DIAGNOSTIC_IGNORE_RETURN_TYPE
#define CLANG_DIAGNOSTIC_IGNORE_SELF_ASSIGN
#define CLANG_DIAGNOSTIC_IGNORE_UNINITIALIZED
#define CLANG_DIAGNOSTIC_IGNORE_DEPRECATED_NON_PROTOTYPE
#endif

#endif
