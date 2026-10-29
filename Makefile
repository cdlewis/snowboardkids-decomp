BASENAME  = snowboardkids
.DEFAULT_GOAL := all
VERSION ?= us
ifneq ($(VERSION),$(filter $(VERSION),us jp))
  $(error VERSION must be us or jp)
endif
ifeq ($(strip $(VERSION)),)
  $(error VERSION must be us or jp)
endif
ifneq ($(words $(VERSION)),1)
  $(error VERSION must be us or jp)
endif

# Keep the established US paths for asset tools and existing workflows.
VERSION_SUFFIX = $(if $(filter jp,$(VERSION)),.jp,)
CONFIG = $(BASENAME)$(VERSION_SUFFIX).yaml
BASE_ROM = $(BASENAME)$(VERSION_SUFFIX).z64
SHA1_FILE = $(BASENAME)$(VERSION_SUFFIX).sha1
BASE_LD_SCRIPT = $(BASENAME)$(VERSION_SUFFIX).ld
MATCHING_BUILD_DIR = build$(if $(filter jp,$(VERSION)),/jp,)
VERSION_DEFINE = -DVERSION_$(if $(filter jp,$(VERSION)),JP,US)=1

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

BUILD_MODE ?= matching
ifeq ($(filter $(BUILD_MODE),matching shiftable),)
  $(error BUILD_MODE must be matching or shiftable)
endif
ifeq ($(VERSION):$(BUILD_MODE),jp:shiftable)
  $(error BUILD_MODE=shiftable currently supports VERSION=us only)
endif
BUILD_DIR = $(if $(filter shiftable,$(BUILD_MODE)),build/shiftable,$(MATCHING_BUILD_DIR))
ASM_DIRS  = asm$(VERSION_SUFFIX) asm$(VERSION_SUFFIX)/data
BIN_DIRS  = assets$(VERSION_SUFFIX)
SRC_DIR = src.$(VERSION)
# Shared C sources are selected explicitly by both regional Splat configs.
SRC_DIRS = $(SRC_DIR) src
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
IDO_PLATFORM := $(if $(filter Darwin,$(shell uname -s)),macos,linux)
IDO_DIR ?= $(abspath $(TOOLS_DIR)/ido-recomp/$(IDO_PLATFORM))
export IDO_DIR
CC      = $(IDO_DIR)/cc
CC_CHECK = clang
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
LLVM_MAJOR_VERSION := 22
LD      = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON  = python3
SPLAT   = $(PYTHON) -m tools.split_rom split
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
C_DEFINES    = $(VERSION_DEFINE) -DLANGUAGE_C -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DNDEBUG \
               -DCOMPILING_LIBULTRA -DBUILD_VERSION=VERSION_I -DF3DEX_GBI
CC_CHECK_MIPS_DEFINES = -DMIPSEB -D_MIPS_FPSET=16 -D_MIPS_ISA_MIPS2=2 \
                        -D_MIPS_ISA=_MIPS_ISA_MIPS2 -D_ABIO32=1 \
                        -D_MIPS_SIM=_ABIO32 -D_MIPS_SZINT=32 \
                        -D_MIPS_SZPTR=32 -D__sgi=1
CC_CHECK_INCLUDES = -I. -Iinclude -Iinclude/PR -I$(SRC_DIR)/ultra/audio -I$(SRC_DIR)/ultra/libc
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
               -I. -Iinclude -Iinclude/PR -I$(SRC_DIR)/ultra/audio -I$(SRC_DIR)/ultra/libc \
               $(C_DEFINES)
OBJCOPYFLAGS = -O binary
RM_MDEBUG    = $(OBJCOPY) --remove-section .mdebug $@
C_OBJ_POSTPROCESS = :

$(BUILD_DIR)/$(SRC_DIR)/ultra/io/%.o: C_OPT = -O1
$(BUILD_DIR)/$(SRC_DIR)/ultra/io/%.o: C_MIPS = -mips2
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/%.o: C_OPT = -O2
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/%.o: C_MIPS = -mips2
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/env.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/env.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/drvrnew.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/drvrnew.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/reverb.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/reverb.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/resample.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/resample.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/auxbus.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/auxbus.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/load.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/load.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/mainbus.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/mainbus.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synallocfx.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synallocfx.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synallocvoice.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synallocvoice.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synthesizer.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/audio/synthesizer.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/os/%.o: C_OPT = -O1
$(BUILD_DIR)/$(SRC_DIR)/ultra/os/%.o: C_MIPS = -mips2
$(BUILD_DIR)/$(SRC_DIR)/ultra/os/timerintr.o: CFLAGS += -D_FINALROM
$(BUILD_DIR)/$(SRC_DIR)/ultra/gu/%.o: C_OPT = -O2
$(BUILD_DIR)/$(SRC_DIR)/ultra/gu/%.o: C_MIPS = -mips2
$(BUILD_DIR)/$(SRC_DIR)/ultra/gu/perspective.o: C_OPT = -O2
$(BUILD_DIR)/$(SRC_DIR)/ultra/io/sptask.o: C_OPT = -O2
$(BUILD_DIR)/$(SRC_DIR)/ultra/io/sptask.o: CFLAGS += -DF3DEX_GBI
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/%.o: C_MIPS = -mips2
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/%.o: ULTRA_AS_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/ll.o: C_OPT = -O1
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/ll.o: C_MIPS = -mips3 -32
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/xprintf.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/xprintf.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/xldtob.o: C_OPT = -O3
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/xldtob.o: IDO_CC = $(IDO_DIRECT)
$(BUILD_DIR)/$(SRC_DIR)/ultra/os/exceptasm.o: ULTRA_AS_ISA = mips3
$(BUILD_DIR)/$(SRC_DIR)/ultra/os/exceptasm.o: ULTRA_AS_POST = $(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@
# These course-select BSS objects end at intentional non-16-byte boundaries.
# Preserve their logical section sizes instead of adding assembler tail padding.
COURSE_SELECT_BSS_C_O_FILES := \
	$(BUILD_DIR)/$(SRC_DIR)/menu/course_select/course_select_selection_state.o \
	$(BUILD_DIR)/$(SRC_DIR)/menu/course_select/course_select_transition_state.o \
	$(BUILD_DIR)/$(SRC_DIR)/menu/course_select/course_select_course_tables.o \
	$(BUILD_DIR)/$(SRC_DIR)/menu/course_select/course_select_status.o
COURSE_SELECT_UNPADDED_BSS_O_FILES := \
	$(COURSE_SELECT_BSS_C_O_FILES) \
	$(BUILD_DIR)/asm/data/bss_chunk_a_2_course_select_transition_tail.bss.o
$(COURSE_SELECT_UNPADDED_BSS_O_FILES): ASFLAGS += -no-pad-sections

LD_SCRIPT      = $(if $(filter shiftable,$(BUILD_MODE)),$(BUILD_DIR)/$(BASENAME).ld,$(BASE_LD_SCRIPT))
LINKER_SCRIPTS = linker_scripts/hardware_regs.ld linker_scripts/n64_os_syms.ld linker_scripts/libultra_syms.ld \
                 linker_scripts/game_syms.ld \
                 linker_scripts/race_setup_menu_syms.ld \
                 linker_scripts/controller_subsystem_syms.ld \
                 linker_scripts/race_motion_syms.ld \
                 linker_scripts/training_course_ui_syms.ld \
                 linker_scripts/course_select_menu_player_syms.ld
ifeq ($(VERSION),jp)
LINKER_SCRIPTS = linker_scripts/hardware_regs.ld linker_scripts/n64_os_syms.ld \
                 linker_scripts/game_syms.jp.ld \
                 undefined_funcs_auto.jp.txt undefined_syms_auto.jp.txt
endif
LINKER_SCRIPTS += linker_scripts/race_graphics_bundles.ld
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
PROJECT_C_FILES := $(shell git ls-files 'src/**/*.c' 'src/*.c' 'src.us/**/*.c' 'src.us/*.c' 'src.jp/**/*.c' 'src.jp/*.c')
FORMAT_C_FILES := $(PROJECT_C_FILES)
PROJECT_H_FILES := $(shell git ls-files 'include/**/*.h' 'include/*.h' 'src/**/*.h' 'src/*.h' 'src.us/**/*.h' 'src.jp/**/*.h')
FORMAT_H_FILES := $(filter-out include/hasm.h include/include_asm.h include/sys/%,$(PROJECT_H_FILES))
FORMAT_FILES := $(FORMAT_C_FILES) $(FORMAT_H_FILES)
TIDY_C_FRAGMENT_FILES := \
	$(foreach version,us jp, \
		src.$(version)/libmus/player_api.c \
		src.$(version)/libmus/player_commands.c \
		src.$(version)/libmus/player_commands_tail.c)
TIDY_C_FILES := $(filter-out %.inc.c $(TIDY_C_FRAGMENT_FILES),$(PROJECT_C_FILES))

TEXTCONV = $(PYTHON) $(TOOLS_DIR)/textconv.py
CHARMAP = $(TOOLS_DIR)/charmap.txt
TEXTCONV_DIR = $(BUILD_DIR)/textconv

BIN_FILES   := $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))
BIN_O_FILES := $(patsubst %.bin,$(BUILD_DIR)/%.o,$(BIN_FILES))

ULTRA_IDO_AS_FILES := \
	$(SRC_DIR)/ultra/libc/bcopy.s \
	$(SRC_DIR)/ultra/libc/bzero.s \
	$(SRC_DIR)/ultra/os/exceptasm.s \
	$(SRC_DIR)/ultra/os/getcount.s \
	$(SRC_DIR)/ultra/os/getsr.s \
	$(SRC_DIR)/ultra/os/interrupt.s \
	$(SRC_DIR)/ultra/os/invaldcache.s \
	$(SRC_DIR)/ultra/os/invalicache.s \
	$(SRC_DIR)/ultra/os/maptlbrdb.s \
	$(SRC_DIR)/ultra/os/probetlb.s \
	$(SRC_DIR)/ultra/os/setcompare.s \
	$(SRC_DIR)/ultra/os/setfpccsr.s \
	$(SRC_DIR)/ultra/os/setintmask.s \
	$(SRC_DIR)/ultra/os/setsr.s \
	$(SRC_DIR)/ultra/os/writebackdcache.s \
	$(SRC_DIR)/ultra/os/writebackdcacheall.s
ULTRA_IDO_AS_O_FILES := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ULTRA_IDO_AS_FILES))

O_FILES := $(patsubst $(MATCHING_BUILD_DIR)/%,$(BUILD_DIR)/%,$(shell grep -E '$(MATCHING_BUILD_DIR)/[^ ()]+\.o' $(BASE_LD_SCRIPT) -o 2>/dev/null | sort | uniq))

TARGET = $(BUILD_DIR)/$(BASENAME)

### Targets

default: all

all: $(if $(filter us,$(VERSION)),course-definitions) dirs $(TARGET).z64 $(if $(filter matching,$(BUILD_MODE)),verify)

ifeq ($(BUILD_MODE),shiftable)
$(LD_SCRIPT): $(BASENAME).ld $(TOOLS_DIR)/make_shiftable_linker.py assets/layout/bundles.mk
	$(V)$(PYTHON) $(TOOLS_DIR)/make_shiftable_linker.py $< $@ --build-dir $(BUILD_DIR) $(if $(filter 1,$(ASSET_RUNTIME_INDEX_REMAP)),--runtime-indices,) $(if $(filter 1,$(ASSET_RUNTIME_SNOWBOARD_SIZE)),--runtime-snowboards,)
endif

course-definitions:
	$(V)$(PYTHON) $(TOOLS_DIR)/generate_course_definitions.py --check

# Build the ROM without SHA1 verification (useful while a build is non-matching).
# The N64 CRC is still applied in the .z64 step, so the result is a bootable ROM.
nonmatching: dirs $(TARGET).z64
	$(PRINTF) "[$(YELLOW) nonmatch $(NO_COL)]  Built $(TARGET).z64 (SHA1 verification skipped)\n"

dirs:
	@mkdir -p $(BUILD_DIR)/asm/data
	@mkdir -p $(BUILD_DIR)/assets
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)

extract: check
	$(PRINTF) "[$(CYAN) splat  $(NO_COL)]  Extracting $(CONFIG)\n"
	$(V)$(SPLAT) $(CONFIG)
ifeq ($(VERSION),us)
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_inventory.py --source-views-only
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_bundles.py init
endif

.PHONY: asset-report verify-assets asset-previews migrate-assets
ifeq ($(VERSION),jp)
asset-report verify-assets asset-previews migrate-assets decompilation-summary:
	$(error $@ currently supports VERSION=us only)
else
asset-report:
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_inventory.py

verify-assets:
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_inventory.py --verify

asset-previews:
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_bundles.py previews

migrate-assets:
	$(V)$(PYTHON) $(TOOLS_DIR)/asset_bundles.py migrate --if-absent
	$(V)$(PYTHON) $(TOOLS_DIR)/semantic_assets.py
endif

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
# JP splits may end at four-byte boundaries. Keep assembler section padding
# from shifting subsequent functions; extracted assembly owns its ROM padding.
ifeq ($(VERSION),jp)
ASFLAGS += -no-pad-sections
endif

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   as   $(NO_COL)]  $<\n"
	$(V)$(CPP) $(CPPFLAGS) $(VERSION_DEFINE) -I include $< | $(AS) $(ASFLAGS) -o $@

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
		$(filter-out -c -I. -Iinclude -Iinclude/PR -I$(SRC_DIR)/ultra/audio -I$(SRC_DIR)/ultra/libc,$(CFLAGS)) $(C_OPT) \
		-I$(abspath .) -I$(abspath include) -I$(abspath include/PR) \
		-I$(abspath $(SRC_DIR)/ultra/audio) -I$(abspath $(SRC_DIR)/ultra/libc) -I$(abspath $(dir $*)) \
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

# These data-only files use designated initializers, which IDO does not support.
# GNU as preserves the original 16-byte section padding; attributes align each table.
DESIGNATED_DATA_C_O_FILES := \
	$(BUILD_DIR)/$(SRC_DIR)/race/player/race_player_properties.o \
	$(BUILD_DIR)/src.us/demo/main_menu_demo_race_intro_data.o \
	$(BUILD_DIR)/$(SRC_DIR)/demo/title_demo_race_intro_data.o
$(BUILD_DIR)/$(SRC_DIR)/race/player/race_player_properties.o: ASFLAGS := $(filter-out -no-pad-sections,$(ASFLAGS))
# The title-demo tables end on a 16-byte boundary in both ROM versions.
$(BUILD_DIR)/$(SRC_DIR)/demo/title_demo_race_intro_data.o: ASFLAGS := $(filter-out -no-pad-sections,$(ASFLAGS))
$(DESIGNATED_DATA_C_O_FILES): $(BUILD_DIR)/%.o: $(TEXTCONV_DIR)/%.c
	@mkdir -p $(dir $@)
	$(PRINTF) "[$(GREEN)   c    $(NO_COL)]  $*.c\n"
	$(V)$(CC_CHECK) --target=mips-unknown-linux-gnu -mabi=32 -march=mips3 -mno-abicalls -fno-pic -G0 \
		-ffreestanding -fno-builtin -fno-common -fno-addrsig -std=gnu99 -Wall -Wextra \
		$(CC_CHECK_INCLUDES) $(C_DEFINES) -U_MIPS_ISA $(CC_CHECK_MIPS_DEFINES) -iquote $(dir $*) -S -o $(@:.o=.s) $<
	$(V)$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
	$(V)$(RM) $(@:.o=.s)

# Compile optional renderer variants without changing the matching objects.
# Flexible links select them only when an asset requires CPU index translation.
$(BUILD_DIR)/runtime-indices/%.o: $(TEXTCONV_DIR)/$(SRC_DIR)/menu/renderer/%.c include/game/menu/renderer/sprite_palette_relocation.h
	@mkdir -p $(dir $@)
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) $(CC_CHECK_INCLUDES) \
		$(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) -DSBK_SPRITE_PALETTE_REMAP -iquote $(SRC_DIR)/menu/renderer $<
	$(V)$(IDO_CC) $(CFLAGS) $(C_OPT) -DSBK_SPRITE_PALETTE_REMAP -I$(SRC_DIR)/menu/renderer -o $@ $<
	$(V)$(RM_MDEBUG)

$(BUILD_DIR)/runtime-indices/race_player_model_renderer.o: $(TEXTCONV_DIR)/$(SRC_DIR)/race/player/race_player_model_renderer.c
	@mkdir -p $(dir $@)
	$(V)$(CC_CHECK) $(CC_CHECK_FLAGS) $(CC_CHECK_WARNINGS) $(CC_CHECK_INCLUDES) \
		$(C_DEFINES) $(CC_CHECK_MIPS_DEFINES) -DSBK_SNOWBOARD_TEXTURE_SIZE -iquote $(SRC_DIR)/race/player $<
	$(V)$(IDO_CC) $(CFLAGS) $(C_OPT) -DSBK_SNOWBOARD_TEXTURE_SIZE -I$(SRC_DIR)/race/player -o $@ $<
	$(V)$(RM_MDEBUG)

# Match original JP text extents at partial-file boundaries. IDO pads C
# objects to 16 bytes; preserve only the padding present in the original ROM.
ifeq ($(VERSION),jp)
$(BUILD_DIR)/src/math/spatial_math.o: C_OBJ_POSTPROCESS = \
	$(PYTHON) $(TOOLS_DIR)/trim_elf_section_tail.py $@ .text 8
$(BUILD_DIR)/src.jp/menu/renderer/menu_render_utils.o: C_OBJ_POSTPROCESS = \
	$(PYTHON) $(TOOLS_DIR)/trim_elf_section_tail.py $@ .text 8

$(BUILD_DIR)/src.jp/menu/race_setup/race_setup_menu.o: C_OBJ_POSTPROCESS = \
	$(PYTHON) $(TOOLS_DIR)/trim_elf_section_tail.py $@ .data 16 \
		--undefine-symbol gControllerPakRumbleCheckPromptState

endif

# Patch ll.o for modern binutils, matching the upstream libultra build.
$(BUILD_DIR)/$(SRC_DIR)/ultra/libc/ll.o: $(SRC_DIR)/ultra/libc/ll.c
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

# Extraction and migration initialize these rules; cleaning must not rebuild them.
ifeq ($(VERSION),us)
ifeq ($(filter clean extract migrate-assets,$(MAKECMDGOALS)),)
-include $(wildcard $(BUILD_DIR)/assets/readable/*.d $(BUILD_DIR)/assets/course_model_resources/*.d $(BUILD_DIR)/assets/course_sprite_tables/*.d)
-include $(wildcard $(BUILD_DIR)/assets/course_display_lists/*.d $(BUILD_DIR)/assets/course_surface_data/*.d)
-include assets/layout/bundles.mk
endif
endif

# Build alternate renderers only when the edited assets require them.
ifeq ($(BUILD_MODE),shiftable)
$(TARGET).elf: $(if $(filter 1,$(ASSET_RUNTIME_INDEX_REMAP)),$(addprefix $(BUILD_DIR)/runtime-indices/,menu_renderer.o menu_render_utils.o))
$(TARGET).elf: $(if $(filter 1,$(ASSET_RUNTIME_SNOWBOARD_SIZE)),$(BUILD_DIR)/runtime-indices/race_player_model_renderer.o)
endif

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
	$(PRINTF) "[$(GREEN) verify $(NO_COL)]  Checking $(SHA1_FILE)\n"
	$(V)shasum --check $(SHA1_FILE)

# Check baserom exists
.PHONY: check
check:
	@if [ ! -f $(BASE_ROM) ]; then echo "Error: $(BASE_ROM) not found" >&2; exit 1; fi

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

ifeq ($(VERSION),us)
decompilation-summary: all
	$(PYTHON) -m mapfile_parser objdiff_report --quiet
	$(PYTHON) $(TOOLS_DIR)/decompilation_summary.py
endif

# C99 enables tidy checks that do not support C89; build checks still use gnu89.
# Tidy is diagnostic-only: automatic fixes can alter matching code generation.
tidy: check-clang-tidy-version
	$(CLANG_TIDY) $(TIDY_C_FILES) -- $(CC_CHECK_FLAGS) -std=gnu99 $(CC_CHECK_INCLUDES) $(C_DEFINES) $(CC_CHECK_MIPS_DEFINES)

###########
## CLEAN ##
###########

clean:
	$(PRINTF) "[$(YELLOW) clean  $(NO_COL)]  Removing generated files\n"
	$(V)rm -rf asm$(VERSION_SUFFIX)
ifeq ($(VERSION),us)
	$(V)rm -rf $(filter-out build/jp,$(wildcard build/*))
else
	$(V)rm -rf $(BUILD_DIR)
endif
	$(V)rm -f $(BASE_LD_SCRIPT) $(BASENAME)$(VERSION_SUFFIX).d undefined_funcs_auto$(VERSION_SUFFIX).txt undefined_syms_auto$(VERSION_SUFFIX).txt

### Settings
.SECONDARY:
.PHONY: all clean default extract nonmatching verify check course-definitions \
	check-clang-format-version check-clang-tidy-version format format-check tidy \
	decompilation-summary
