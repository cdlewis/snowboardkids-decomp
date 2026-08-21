BASENAME  = snowboardkids

ifndef QUIET
  PRINTF = @printf
else
  PRINTF = @true
endif

VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

WERROR ?= 0

# Colours

NO_COL  := \033[0m
RED     := \033[0;31m
RED2    := \033[1;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
PINK    := \033[0;35m
CYAN    := \033[0;36m

# Directories

BUILD_DIR = build
ASM_DIRS  = asm asm/data
BIN_DIRS  = assets
SRC_DIRS  = src
TOOLS_DIR = tools

# Tools

find-command = $(shell which $(1) 2>/dev/null)

ifneq      ($(call find-command,mips-linux-gnu-ld),)
  CROSS := mips-linux-gnu-
else ifneq ($(call find-command,mips64-linux-gnu-ld),)
  CROSS := mips64-linux-gnu-
else ifneq ($(call find-command,mips64-elf-ld),)
  CROSS := mips64-elf-
else
  $(error Unable to detect a suitable MIPS cross toolchain installed.)
endif

ifneq (,$(call find-command,$(CROSS)clang))
  CPP      := $(CROSS)clang
  CPPFLAGS := -E -P -x c
else ifneq (,$(call find-command,clang))
  CPP      := clang
  CPPFLAGS := -E -P -x c
else
  CPP      := cpp
  CPPFLAGS := -P
endif

AS      = $(CROSS)as
CC      = $(TOOLS_DIR)/ido-recomp/linux/cc
CC_CHECK = clang
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
LLVM_MAJOR_VERSION := 20
LD      = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON  = python3
SPLAT   = $(PYTHON) -m splat split
N64CRC  = $(TOOLS_DIR)/n64crc.py
ASM_PROC = $(TOOLS_DIR)/asm-processor/build.py

IDO_ASMPROC = $(PYTHON) $(ASM_PROC) $(CC) -- $(AS) $(ASFLAGS) --
IDO_DIRECT  = $(CC)
IDO_CC      = $(IDO_ASMPROC)

# Flags

ASFLAGS      = -G 0 -I include -mips3 -mabi=32
ULTRA_AS_CPPFLAGS = -D_MIPS_SZLONG=32 -DCOMPILING_LIBULTRA \
                    -DBUILD_VERSION=VERSION_I -DBUILD_VERSION_STRING=\"2.0I\" \
                    -D_FINALROM -DNDEBUG -non_shared
ULTRA_AS_INCLUDES = -I$(abspath include) -I$(abspath include/compiler/ido) -I$(abspath include/PR)
ULTRA_AS_ISA = mips2
ULTRA_AS_OPT = -O1
ULTRA_AS_POST =
C_DEFINES    = -DLANGUAGE_C -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DNDEBUG \
               -DCOMPILING_LIBULTRA -DBUILD_VERSION=VERSION_I -DF3DEX_GBI
CC_CHECK_MIPS_DEFINES = -DMIPSEB -D_MIPS_FPSET=16 -D_MIPS_ISA_MIPS2=2 \
                        -D_MIPS_ISA=_MIPS_ISA_MIPS2 -D_ABIO32=1 \
                        -D_MIPS_SIM=_ABIO32 -D_MIPS_SZINT=32 \
                        -D_MIPS_SZPTR=32 -D__sgi=1
CC_CHECK_INCLUDES = -I. -Iinclude -Iinclude/PR -Isrc/ultra/audio -Isrc/ultra/libc
CC_CHECK_FLAGS = -fsyntax-only -ffreestanding -fno-builtin -funsigned-char \
                 -fdiagnostics-color -std=gnu89 -m32 \
                 '-D__builtin_classof(type)=0' \
                 '-D__builtin_alignof(type)=__alignof__(type)'
CC_CHECK_WARNINGS = -Wall -Wextra -Wno-unknown-pragmas \
                    -Wno-unused-parameter -Wno-unused-variable \
                    -Wno-unused-but-set-variable -Wno-unused-value \
                    -Wno-unused-label -Wno-missing-braces -Wno-sign-compare \
                    -Wno-tautological-compare -Wno-pointer-bool-conversion \
                    -Wno-array-bounds -Wno-unsequenced -Wno-pointer-sign \
                    -Wno-constant-conversion -Wno-multichar \
                    -Werror=implicit-int \
                    -Werror=implicit-function-declaration \
                    -Werror=int-conversion \
                    -Werror=incompatible-pointer-types \
                    -Werror=incompatible-function-pointer-types \
                    -Werror=strict-prototypes \
                    -Werror=deprecated-non-prototype \
                    -Werror=return-type
ifneq ($(WERROR),0)
  CC_CHECK_WARNINGS += -Werror
endif
C_MIPS       = -mips1
C_OPT        = -O2
CFLAGS       = -c $(C_MIPS) -G 0 -non_shared -fullwarn -Xcpluscomm \
               -nostdinc -Wab,-r4300_mul -woff 649,838,712,516 \
               -I. -Iinclude -Iinclude/PR -Isrc/ultra/audio -Isrc/ultra/libc \
               $(C_DEFINES)
OBJCOPYFLAGS = -O binary
RM_MDEBUG    = $(OBJCOPY) --remove-section .mdebug $@
C_OBJ_POSTPROCESS = :

$(BUILD_DIR)/src/ultra/io/%.o: C_OPT = -O1
$(BUILD_DIR)/src/ultra/io/%.o: C_MIPS = -mips2
$(BUILD_DIR)/src/ultra/audio/%.o: C_OPT = -O2
$(BUILD_DIR)/src/ultra/audio/%.o: C_MIPS = -mips2
$(BUILD_DIR)/src/ultra/audio/env.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/env.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/drvrnew.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/drvrnew.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/reverb.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/reverb.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/resample.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/resample.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/auxbus.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/auxbus.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/load.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/load.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/mainbus.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/mainbus.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/synallocfx.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/synallocfx.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/synallocvoice.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/synallocvoice.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/audio/synthesizer.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/audio/synthesizer.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/os/%.o: C_OPT = -O1
$(BUILD_DIR)/src/ultra/os/%.o: C_MIPS = -mips2
$(BUILD_DIR)/src/ultra/os/timerintr.o: CFLAGS += -D_FINALROM
$(BUILD_DIR)/src/ultra/gu/%.o: C_OPT = -O2
$(BUILD_DIR)/src/ultra/gu/%.o: C_MIPS = -mips2
$(BUILD_DIR)/src/ultra/gu/perspective.o: C_OPT = -O2
$(BUILD_DIR)/src/ultra/io/sptask.o: C_OPT = -O2
$(BUILD_DIR)/src/ultra/io/sptask.o: CFLAGS += -DF3DEX_GBI
$(BUILD_DIR)/src/ultra/libc/%.o: C_MIPS = -mips2
$(BUILD_DIR)/src/ultra/libc/%.o: ULTRA_AS_OPT = -O3
$(BUILD_DIR)/src/ultra/libc/ll.o: C_OPT = -O1
$(BUILD_DIR)/src/ultra/libc/ll.o: C_MIPS = -mips3 -32
$(BUILD_DIR)/src/ultra/libc/xprintf.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/libc/xprintf.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/libc/xldtob.o: C_OPT = -O3
$(BUILD_DIR)/src/ultra/libc/xldtob.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/src/ultra/os/exceptasm.o: ULTRA_AS_ISA = mips3
$(BUILD_DIR)/src/ultra/os/exceptasm.o: ULTRA_AS_POST = $(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@
# These course-select BSS objects end at intentional non-16-byte boundaries.
# Preserve their logical section sizes instead of adding assembler tail padding.
COURSE_SELECT_BSS_C_O_FILES := \
	$(BUILD_DIR)/src/menu/course_select/course_select_selection_state.o \
	$(BUILD_DIR)/src/menu/course_select/course_select_transition_state.o \
	$(BUILD_DIR)/src/menu/course_select/course_select_course_tables.o \
	$(BUILD_DIR)/src/menu/course_select/course_select_status.o
COURSE_SELECT_UNPADDED_BSS_O_FILES := \
	$(COURSE_SELECT_BSS_C_O_FILES) \
	$(BUILD_DIR)/asm/data/bss_chunk_a_2_course_select_selection_tail.bss.o \
	$(BUILD_DIR)/asm/data/bss_chunk_a_2_course_select_transition_tail.bss.o \
	$(BUILD_DIR)/asm/data/bss_chunk_a_2_course_select_tables_tail.bss.o \
	$(BUILD_DIR)/asm/data/bss_chunk_a_2_course_select_status_tail.bss.o
$(COURSE_SELECT_UNPADDED_BSS_O_FILES): ASFLAGS += -no-pad-sections
# The weak, zero-initialized prompt state preserves IDO's original codegen;
# raw BSS owns its storage. Remove only that verified trailing data block.
$(BUILD_DIR)/src/menu/race_setup/race_setup_menu.o: C_OBJ_POSTPROCESS = \
	$(PYTHON) $(TOOLS_DIR)/trim_elf_section_tail.py $@ .data 16 \
		--undefine-symbol gControllerPakRumbleCheckPromptState

LD_SCRIPT      = $(BASENAME).ld
LINKER_SCRIPTS = linker_scripts/hardware_regs.ld linker_scripts/libultra_syms.ld \
                 linker_scripts/game_syms.ld \
                 linker_scripts/race_setup_menu_syms.ld \
                 linker_scripts/controller_subsystem_syms.ld \
                 linker_scripts/race_motion_syms.ld \
                 linker_scripts/training_course_ui_syms.ld \
                 linker_scripts/course_select_menu_player_syms.ld
LDFLAGS        = -T $(LD_SCRIPT) -Map $(TARGET).map \
                 $(foreach ld,$(LINKER_SCRIPTS),-T $(ld)) \
                 --no-check-sections

# Files

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ASM_S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
ASM_O_FILES := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_S_FILES))

C_FILES     := $(foreach dir,$(SRC_DIRS),$(call rwildcard,$(dir),*.c))
C_O_FILES   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_FILES))

# Formatting and tidy cover all tracked C code, including libultra and libmus.
# Local match-sensitive functions and textconv tables use clang-format guards.
PROJECT_C_FILES := $(shell git ls-files 'src/**/*.c' 'src/*.c')
FORMAT_C_FILES := $(PROJECT_C_FILES)
PROJECT_H_FILES := $(shell git ls-files 'include/**/*.h' 'include/*.h' 'src/**/*.h')
FORMAT_H_FILES := $(filter-out include/hasm.h include/include_asm.h include/sys/%,$(PROJECT_H_FILES))
FORMAT_FILES := $(FORMAT_C_FILES) $(FORMAT_H_FILES)
TIDY_C_FRAGMENT_FILES := \
	src/libmus/player_api.c \
	src/libmus/player_commands.c \
	src/libmus/player_commands_tail.c
TIDY_C_FILES := $(filter-out %.inc.c $(TIDY_C_FRAGMENT_FILES),$(PROJECT_C_FILES))

TEXTCONV = $(PYTHON) $(TOOLS_DIR)/textconv.py
CHARMAP = $(TOOLS_DIR)/charmap.txt
TEXTCONV_DIR = $(BUILD_DIR)/textconv
COURSE_SURFACE_DATA_PACK = $(PYTHON) $(TOOLS_DIR)/course_surface_data_pack.py
COURSE_MODEL_RESOURCES_PACK = $(PYTHON) $(TOOLS_DIR)/course_model_resources_pack.py
COURSE_SPRITE_TABLE_PACK = $(PYTHON) $(TOOLS_DIR)/course_sprite_table_pack.py

BIN_FILES   := $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))
BIN_O_FILES := $(patsubst %.bin,$(BUILD_DIR)/%.o,$(BIN_FILES))

ULTRA_IDO_AS_FILES := \
	src/ultra/libc/bcopy.s \
	src/ultra/libc/bzero.s \
	src/ultra/os/exceptasm.s \
	src/ultra/os/getcount.s \
	src/ultra/os/getsr.s \
	src/ultra/os/interrupt.s \
	src/ultra/os/invaldcache.s \
	src/ultra/os/invalicache.s \
	src/ultra/os/maptlbrdb.s \
	src/ultra/os/probetlb.s \
	src/ultra/os/setcompare.s \
	src/ultra/os/setfpccsr.s \
	src/ultra/os/setintmask.s \
	src/ultra/os/setsr.s \
	src/ultra/os/writebackdcache.s \
	src/ultra/os/writebackdcacheall.s
ULTRA_IDO_AS_O_FILES := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ULTRA_IDO_AS_FILES))

O_FILES := $(shell grep -E 'build/(asm|assets|src)/.+\.o' $(LD_SCRIPT) -o 2>/dev/null | sort | uniq)

TARGET = $(BUILD_DIR)/$(BASENAME)

### Targets

default: all

all: dirs $(TARGET).z64 verify

# Build the ROM without SHA1 verification (useful while a build is non-matching).
# The N64 CRC is still applied in the .z64 step, so the result is a bootable ROM.
nonmatching: dirs $(TARGET).z64
	$(PRINTF) "[$(YELLOW) nonmatch $(NO_COL)]  Built $(TARGET).z64 (SHA1 verification skipped)\n"

dirs:
	@mkdir -p $(BUILD_DIR)/asm/data
	@mkdir -p $(BUILD_DIR)/assets
	@mkdir -p $(BUILD_DIR)/src

extract: check
	$(PRINTF) "[$(CYAN) splat  $(NO_COL)]  Extracting $(BASENAME).yaml\n"
	$(V)$(SPLAT) $(BASENAME).yaml

#################
## COMPILATION ##
#################

$(ULTRA_IDO_AS_O_FILES): $(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   as   $(NO_COL)]  $<\n"
	$(V)$(TOOLS_DIR)/ido-as.sh $< $@ $(ULTRA_AS_ISA) $(ULTRA_AS_OPT)
	$(V)$(ULTRA_AS_POST)
	$(V)$(RM_MDEBUG)

# *.s -> *.o (through C preprocessor)
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   as   $(NO_COL)]  $<\n"
	$(V)$(CPP) $(CPPFLAGS) -I include $< | $(AS) $(ASFLAGS) -o $@

$(TEXTCONV_DIR)/%.c: %.c $(TOOLS_DIR)/textconv.py $(CHARMAP)
	@mkdir -p $(dir $@)
	$(V)$(TEXTCONV) $(CHARMAP) $< $@

# IDO's object writer rounds BSS sections to 16 bytes. These data-only files
# are assembled from IDO's normal output with GNU as so -no-pad-sections can
# preserve their actual sizes.
$(COURSE_SELECT_BSS_C_O_FILES): $(BUILD_DIR)/%.o: $(TEXTCONV_DIR)/%.c
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   c    $(NO_COL)]  $*.c\n"
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) \
		$(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) -iquote $(dir $*) $<
	$(V)cd $(dir $@) && $(abspath $(CC)) \
		$(filter-out -c -I. -Iinclude -Iinclude/PR -Isrc/ultra/audio -Isrc/ultra/libc,$(CFLAGS)) $(C_OPT) \
		-I$(abspath .) -I$(abspath include) -I$(abspath include/PR) \
		-I$(abspath src/ultra/audio) -I$(abspath src/ultra/libc) -I$(abspath $(dir $*)) \
		-S $(abspath $<)
	$(V)$(AS) $(ASFLAGS) -o $@ $(dir $@)$(notdir $*).s
	$(V)$(RM) $(dir $@)$(notdir $*).s

$(BUILD_DIR)/%.o: $(TEXTCONV_DIR)/%.c
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   c    $(NO_COL)]  $*.c\n"
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) \
		$(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) -iquote $(dir $*) $<
	$(V)$(IDO_CC) $(CFLAGS) $(C_OPT) -I$(dir $*) -o $@ $<
	$(V)$(RM_MDEBUG)
	$(V)$(C_OBJ_POSTPROCESS)

# Patch ll.o for modern binutils, matching the upstream libultra build.
$(BUILD_DIR)/src/ultra/libc/ll.o: src/ultra/libc/ll.c
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   c    $(NO_COL)]  $<\n"
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) \
		$(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) $<
	$(V)$(IDO_CC) $(CFLAGS) $(C_OPT) -o $@ $<
	$(V)$(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@
	$(V)$(RM_MDEBUG)


# *.bin -> *.o
$(BUILD_DIR)/%.o: %.bin
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(PINK) linker $(NO_COL)]  $<\n"
	$(V)$(LD) -r -b binary -o $@ $<

# Editable compressed course collision/path data -> *.o
$(BUILD_DIR)/assets/course_surface_data/%.o: assets/course_surface_data/%.yaml \
		$(TOOLS_DIR)/course_surface_data_pack.py \
		$(TOOLS_DIR)/course_surface_data_common.py \
		$(TOOLS_DIR)/huffman_asset.py
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN) course $(NO_COL)]  $<\n"
	$(V)$(COURSE_SURFACE_DATA_PACK) $< --out $(BUILD_DIR)/assets/course_surface_data/$*.bin
	$(V)$(LD) -r -b binary -o $@ $(BUILD_DIR)/assets/course_surface_data/$*.bin

# Editable compressed course vertices and remaining segment-3 resources.
$(BUILD_DIR)/assets/course_model_resources/%.o: assets/course_model_resources/%.yaml \
		$(TOOLS_DIR)/course_model_resources_pack.py \
		$(TOOLS_DIR)/course_graphics_common.py \
		$(TOOLS_DIR)/huffman_asset.py
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN) course $(NO_COL)]  $<\n"
	$(V)$(COURSE_MODEL_RESOURCES_PACK) $< --out $(BUILD_DIR)/assets/course_model_resources/$*.bin
	$(V)$(LD) -r -b binary -o $@ $(BUILD_DIR)/assets/course_model_resources/$*.bin

# Editable compressed course-specific sprite tables.
$(BUILD_DIR)/assets/course_sprite_tables/%.o: assets/course_sprite_tables/%.yaml \
		$(TOOLS_DIR)/course_sprite_table_pack.py \
		$(TOOLS_DIR)/course_sprite_table_common.py \
		$(TOOLS_DIR)/course_graphics_common.py \
		$(TOOLS_DIR)/huffman_asset.py
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN) course $(NO_COL)]  $<\n"
	$(V)$(COURSE_SPRITE_TABLE_PACK) $< --out $(BUILD_DIR)/assets/course_sprite_tables/$*.bin
	$(V)$(LD) -r -b binary -o $@ $(BUILD_DIR)/assets/course_sprite_tables/$*.bin

# Editable F3DEX command streams from the course segment-2 graphics bundles.
# GNU as preserves command streams whose lengths are not multiples of IDO's
# normal 16-byte data-section alignment.
$(BUILD_DIR)/assets/course_display_lists/%.o: assets/course_display_lists/%.c
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN) course $(NO_COL)]  $<\n"
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) \
		$(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) $<
	$(V)cd $(dir $@) && $(abspath $(CC)) \
		$(filter-out -c -I. -Iinclude -Iinclude/PR -Isrc/ultra/audio -Isrc/ultra/libc,$(CFLAGS)) $(C_OPT) \
		-I$(abspath .) -I$(abspath include) -I$(abspath include/PR) \
		-I$(abspath src/ultra/audio) -I$(abspath src/ultra/libc) \
		-S $(abspath $<)
	$(V)$(AS) $(ASFLAGS) -no-pad-sections -o $@ $(dir $@)$(notdir $*).s
	$(V)$(RM) $(dir $@)$(notdir $*).s

# *.o -> *.elf
$(TARGET).elf: $(LD_SCRIPT) $(LINKER_SCRIPTS) $(O_FILES)
	$(PRINTF) "[$(PINK) linker $(NO_COL)]  Linking $(TARGET).elf\n"
	$(V)$(LD) $(LDFLAGS) -o $@

# *.elf -> *.bin
$(TARGET).bin: $(TARGET).elf
	$(PRINTF) "[$(CYAN) objcpy $(NO_COL)]  $<\n"
	$(V)$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

# *.bin -> *.z64 (with N64 CRC)
$(TARGET).z64: $(TARGET).bin
	$(PRINTF) "[$(CYAN) n64crc $(NO_COL)]  $@\n"
	$(V)cp $< $@
	$(V)$(PYTHON) $(N64CRC) $@

# SHA1 verification
verify: $(TARGET).z64
	$(PRINTF) "[$(GREEN) verify $(NO_COL)]  Checking $(BASENAME).sha1\n"
	$(V)shasum --check $(BASENAME).sha1

# Check baserom exists
.PHONY: check
check:
	@if [ ! -f $(BASENAME).z64 ]; then echo "Error: $(BASENAME).z64 not found" >&2; exit 1; fi

check-clang-format-version:
	@command -v $(CLANG_FORMAT) >/dev/null 2>&1 || { echo "Error: $(CLANG_FORMAT) was not found" >&2; exit 1; }
	@version=`$(CLANG_FORMAT) --version | sed -n 's/.*version \([0-9][0-9]*\).*/\1/p' | head -n 1`; \
		if [ "$$version" != "$(LLVM_MAJOR_VERSION)" ]; then \
			echo "Error: clang-format $(LLVM_MAJOR_VERSION) is required (found major version $${version:-unknown})" >&2; \
			exit 1; \
		fi

check-clang-tidy-version:
	@command -v $(CLANG_TIDY) >/dev/null 2>&1 || { echo "Error: $(CLANG_TIDY) was not found" >&2; exit 1; }
	@version=`$(CLANG_TIDY) --version | sed -n 's/.*version \([0-9][0-9]*\).*/\1/p' | head -n 1`; \
		if [ "$$version" != "$(LLVM_MAJOR_VERSION)" ]; then \
			echo "Error: clang-tidy $(LLVM_MAJOR_VERSION) is required (found major version $${version:-unknown})" >&2; \
			exit 1; \
		fi

format: check-clang-format-version
	$(CLANG_FORMAT) -i -style=file $(FORMAT_FILES)

format-check: check-clang-format-version
	$(CLANG_FORMAT) --dry-run --Werror -style=file $(FORMAT_FILES)

decompilation-summary: all
	$(PYTHON) -m mapfile_parser objdiff_report --quiet
	$(PYTHON) $(TOOLS_DIR)/decompilation_summary.py

# Tidy is diagnostic-only: automatic fixes can alter matching code generation.
tidy: check-clang-tidy-version
	$(CLANG_TIDY) $(TIDY_C_FILES) -- $(CC_CHECK_FLAGS) $(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES)

###########
## CLEAN ##
###########

clean:
	$(PRINTF) "[$(YELLOW) clean  $(NO_COL)]  Removing generated files\n"
	$(V)rm -rf asm
	$(V)rm -rf assets
	$(V)rm -rf build
	$(V)rm -f *auto.txt

### Settings
.SECONDARY:
.PHONY: all clean default extract nonmatching verify check \
	check-clang-format-version check-clang-tidy-version format format-check tidy \
	decompilation-summary
