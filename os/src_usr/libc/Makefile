DEBUG_MODE  = 3
ARCH_NAME   = soclib
CPU_NAME    = mipsel
TARGET_FILE = libc.a
TARGET_DIR  = ../../../
OS_DIR      = ../..
SRC_DIR     = .
INC_DIR     = $(SRC_DIR) $(OS_DIR)/src_sys/cpu/$(CPU_NAME) $(OS_DIR)/src_sys/arch/$(ARCH_NAME) $(OS_DIR)/src_sys/kernel $(OS_DIR)/src_sys/libk

SRC_FILES    = $(foreach d,$(SRC_DIR),$(wildcard $(d)/*.c))

#------------------------------------------------------------------------------
# DEBUG_MODE   : O = no message debug, 1 = message debug 
#              
# CPU_NAME     : processor target name
# ARCH_NAME    : architecture plateform name
#
# TARGET_FILE  : name of binary result file
#              
# TARGET_DIR   : output directory where the binary result is written
# OS_DIR       : operating system root directory
# SRC_DIR      : source directories
# BUILD_DIR    : build directory
# ARCH_DIR     : directory of architecture specific source code
#
# CCTOOLS      : directory-of-gcc-tools/gcc-tools-prefix-name
#------------------------------------------------------------------------------

BUILD_DIR    ?= $(OS_DIR)/build/lib
ARCH_DIR     ?= $(OS_DIR)/src_sys/arch/$(ARCH_NAME)

CCTOOLS      ?= undef

#------------------------------------------------------------------------------

BIN_FILE     = $(TARGET_DIR)/$(TARGET_FILE)

SRC_FILES    = $(foreach d,$(SRC_DIR),$(wildcard $(d)/*.c))
INC_FILES    = $(foreach d,$(SRC_DIR),$(wildcard $(d)/*.h))
OBJ_FILES    = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC_FILES:%.c=%.o)))

CC           = $(CCTOOLS)/bin/$(CPU_NAME)-unknown-elf-gcc
LD           = $(CCTOOLS)/bin/$(CPU_NAME)-unknown-elf-ld
AR           = $(CCTOOLS)/bin/$(CPU_NAME)-unknown-elf-ar
OBJDUMP      = $(CCTOOLS)/bin/$(CPU_NAME)-unknown-elf-objdump
OBJCOPY      = $(CCTOOLS)/bin/$(CPU_NAME)-unknown-elf-objcopy
RM           = /bin/rm
TRASH        = /dev/null||true

#INC_OPTION   = $(addprefix -I,$(SRC_DIR))
INC_OPTION   = $(addprefix -I,$(INC_DIR))
CFLAGS       = -fno-builtin -fomit-frame-pointer -O3 -G0 -Wall -Werror -mips32r2
CFLAGS       += -std=c99 -DDEBUG=$(DEBUG_MODE) $(INC_OPTION)

#------------------------------------------------------------------------------

.PHONY      : all clean cleanall print param

dep         : $(BIN_FILE)

all	        : param cleanall depend dep print

clean       : 
	@echo [RM] $(BIN_FILE) files
	$(RM) $(BIN_FILE).dump $(BIN_FILE).pdf $(BUILD_DIR)/ldscript \
	      .DS_Store */.DS_Store */*/.DS_Store \
          *.log $(BUILD_DIR)/* *~ */*~ */*/*~ */*/*/*~ */*/*/*/*~ Makefile~ 2> $(TRASH)

cleanall    : clean 
	$(RM) $(BIN_FILE) $(TARGET_FILE).pdf 2> $(TRASH)
	$(RM) tags $(TARGET_DIR)/*tty* $(addsuffix .od,$(BIN_FILE)) 2> $(TRASH)
	awk '(NR==1),/^\t*# .* AUTOMATIC DEPENDANCIES/' Makefile > Makefile.new;\
    mv Makefile.new Makefile

print       :
	@echo [LP] $(BIN_FILE).pdf
	a2ps -2 --medium=A4 --file-align=fill -o - -l100 Makefile $(INC_FILES) $(SRC_FILES) |\
	ps2pdf -sPAPERSIZE=a4 - $(BIN_FILE).pdf

param        :
	@echo "======================================================================"
	@echo "                             $(TARGET_FILE)" 
	@echo "----------------------------------------------------------------------"
	@echo "[DEBUG_MODE    ] $(DEBUG_MODE)"
	@echo "[ARCH_NAME     ] $(ARCH_NAME)"
	@echo "[CPU_NAME      ] $(CPU_NAME)"
	@echo "[BIN_FILE      ] $(BIN_FILE)"
	@echo "[TARGET_FILE   ] $(TARGET_FILE)"
	@echo "[SRC_FILES     ] $(SRC_FILES)"
	@echo "[INC_FILES     ] $(INC_FILES)"
	@echo "[TARGET_DIR    ] $(TARGET_DIR)"
	@echo "[OS_DIR        ] $(OS_DIR)"
	@echo "[SRC_DIR       ] $(SRC_DIR)"
	@echo "[BUILD_DIR     ] $(BUILD_DIR)"
	@echo "[CCTOOLS       ] $(CCTOOLS)"
	@echo "----------------------------------------------------------------------"
	@if (test "$(ARCH_NAME)"   = "undef");then echo [ERROR] ARCH_NAME   undefined && false;fi
	@if (test "$(CPU_NAME)"    = "undef");then echo [ERROR] CPU_NAME    undefined && false;fi
	@if (test "$(TARGET_DIR)"  = "undef");then echo [ERROR] TARGET_DIR  undefined && false;fi
	@if (test "$(OS_DIR)"      = "undef");then echo [ERROR] OS_DIR      undefined && false;fi
	@if (test "$(SRC_DIR)"     = "undef");then echo [ERROR] SRC_DIR     undefined && false;fi
	@if (test "$(TARGET_FILE)" = "undef");then echo [ERROR] TARGET_FILE undefined && false;fi
	@test -d $(BUILD_DIR)||echo "[ERROR] Build directory $(BUILD_DIR) does not exist"

#------------------------------------------------------------------------------

$(BIN_FILE)   : $(INC_FILES) $(OBJ_FILES) $(BUILD_DIR)/kldscript
	@echo [LD] $@
	$(LD) -o $@ $(OBJ_FILES) $(LDFLAGS) -T$(BUILD_DIR)/kldscript
	$(OBJDUMP) $@ -D > $@.dump

$(BUILD_DIR)/ldscript : arch/$(ARCH_NAME)/ldscript.h arch/$(ARCH_NAME)/segmentation.h
	@echo [CP] `basename $@`
	cpp -Iarch/$(ARCH_NAME) $< | egrep -v "#|//" | grep . > $@

#------------------------------------------------------------------------------

COMPIL = echo [CC] $(ROOT_SRC_DIR)/$<;$(CC) $(CFLAGS) -c $< -o $@;$(OBJDUMP) $@ -D > $@.od

$(BUILD_DIR)/%.o: */*/*/%.c ; $(COMPIL)
$(BUILD_DIR)/%.o: */*/%.c   ; $(COMPIL)
$(BUILD_DIR)/%.o: */%.c     ; $(COMPIL)
$(BUILD_DIR)/%.o: %.c       ; $(COMPIL)

#------------------------------------------------------------------------------

depend:
	awk '(NR==1),/^\t*# .* AUTOMATIC DEPENDANCIES/' Makefile                  |\
	grep -v "^[ \t]*# .* AUTOMATIC DEPENDANCIES"              >  Makefile.new ;\
	echo "# DO NOT DELETE THIS LINE : AUTOMATIC DEPENDANCIES" >> Makefile.new ;\
	gcc -MM $(INC_OPTION) $(SRC_FILES)                                        |\
    sed 's/^\([^ ]\)/$$(BUILD_DIR)\/\1/'                      >> Makefile.new ;\
	mv Makefile.new Makefile
# DO NOT DELETE THIS LINE : AUTOMATIC DEPENDANCIES
