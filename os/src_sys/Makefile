DEBUG_MODE  = 3
CPU_NAME    = mipsel
ARCH_NAME   = soclib
TARGET_FILE = kernel-$(ARCH_NAME).bin
TARGET_DIR  = ../../simul
OS_DIR      = ..
SRC_DIR     = cpu/$(CPU_NAME) arch/$(ARCH_NAME) kernel libk 

#@echo "directories: " $SRC_DIR

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

BUILD_DIR    ?= $(OS_DIR)/build/kernel
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

INC_OPTION   = $(addprefix -I,$(SRC_DIR))
CFLAGS       = -fno-builtin -fomit-frame-pointer -O3 -G0 -Wall -Werror -mips32r2
CFLAGS       += -std=c99 -DDEBUG=$(DEBUG_MODE) $(INC_OPTION)

#------------------------------------------------------------------------------

.PHONY      : all clean cleanall print param

dep         : $(BIN_FILE)

all	        : param cleanall depend dep print

clean       : 
	@echo [RM] $(BIN_FILE) files
	$(RM) $(BIN_FILE).dump $(BIN_FILE).pdf $(BUILD_DIR)/kldscript $(BUILD_DIR)/kldscript \
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

$(BUILD_DIR)/kldscript : arch/$(ARCH_NAME)/kldscript.h arch/$(ARCH_NAME)/segmentation.h
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
$(BUILD_DIR)/hal_cpu_code.o: cpu/mipsel/hal_cpu_code.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h kernel/syscall.h kernel/pthread.h
$(BUILD_DIR)/__boot_loader.o: arch/soclib/__boot_loader.c arch/soclib/segmentation.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h
$(BUILD_DIR)/driver_bd.o: arch/soclib/driver_bd.c kernel/config.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h kernel/scheduler.h libk/libk.h libk/kstdio.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h
$(BUILD_DIR)/driver_cpu.o: arch/soclib/driver_cpu.c arch/soclib/driver_cpu.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdio.h kernel/scheduler.h libk/kstdlib.h kernel/ctype.h \
 libk/error.h
$(BUILD_DIR)/driver_fb.o: arch/soclib/driver_fb.c kernel/config.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h kernel/scheduler.h libk/libk.h libk/kstdio.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h
$(BUILD_DIR)/driver_icu.o: arch/soclib/driver_icu.c arch/soclib/driver_icu.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h libk/libk.h \
 libk/kstdio.h kernel/scheduler.h libk/kstdlib.h kernel/ctype.h \
 libk/error.h
$(BUILD_DIR)/driver_timer.o: arch/soclib/driver_timer.c arch/soclib/driver_timer.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdio.h kernel/scheduler.h libk/kstdlib.h kernel/ctype.h \
 libk/error.h
$(BUILD_DIR)/driver_tty.o: arch/soclib/driver_tty.c kernel/config.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/libk.h libk/kstdio.h kernel/scheduler.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h
$(BUILD_DIR)/hal_arch_code.o: arch/soclib/hal_arch_code.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h
$(BUILD_DIR)/app.o: kernel/app.c kernel/scheduler.h libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h libk/kstdlib.h kernel/ctype.h \
 libk/error.h kernel/thread.h kernel/mutex.h kernel/barrier.h
$(BUILD_DIR)/barrier.o: kernel/barrier.c kernel/barrier.h libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h kernel/thread.h
$(BUILD_DIR)/__do_exception.o: kernel/__do_exception.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h
$(BUILD_DIR)/__do_init.o: kernel/__do_init.c kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/libk.h libk/kstdio.h kernel/scheduler.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/thread.h kernel/app.h
$(BUILD_DIR)/__do_interrupt.o: kernel/__do_interrupt.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h
$(BUILD_DIR)/ksyscall.o: kernel/ksyscall.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h kernel/syscall.h kernel/pthread.h
$(BUILD_DIR)/mutex.o: kernel/mutex.c kernel/mutex.h libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h kernel/thread.h
$(BUILD_DIR)/scheduler.o: kernel/scheduler.c libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h kernel/scheduler.h libk/kstdlib.h \
 kernel/ctype.h libk/error.h kernel/thread.h
$(BUILD_DIR)/sys_drivers.o: kernel/sys_drivers.c arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h kernel/syscall.h \
 kernel/pthread.h libk/error.h libk/libk.h libk/kstdio.h \
 kernel/scheduler.h libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/sys_info.o: kernel/sys_info.c cpu/mipsel/stdint.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 libk/klist.h arch/soclib/hal_arch_code.h arch/soclib/segmentation.h \
 arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h
$(BUILD_DIR)/sys_pthread.o: kernel/sys_pthread.c libk/error.h kernel/thread.h \
 libk/libk.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h kernel/config.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h \
 arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/kstdio.h \
 kernel/scheduler.h libk/kstdlib.h kernel/ctype.h kernel/mutex.h \
 kernel/pthread.h
$(BUILD_DIR)/thread.o: kernel/thread.c kernel/scheduler.h libk/libk.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/kstdio.h libk/kstdlib.h kernel/ctype.h \
 libk/error.h kernel/thread.h
$(BUILD_DIR)/katob.o: libk/katob.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kgets.o: libk/kgets.c cpu/mipsel/stdint.h libk/kstdio.h \
 arch/soclib/hal_arch.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/scheduler.h
$(BUILD_DIR)/kmalloc.o: libk/kmalloc.c libk/kstdlib.h kernel/ctype.h libk/kstdio.h \
 arch/soclib/hal_arch.h cpu/mipsel/stdint.h libk/klist.h \
 arch/soclib/hal_arch_code.h arch/soclib/segmentation.h kernel/hal_cpu.h \
 cpu/mipsel/hal_cpu_code.h kernel/config.h arch/soclib/driver_cpu.h \
 arch/soclib/driver_tty.h arch/soclib/driver_timer.h \
 arch/soclib/driver_icu.h libk/libk.h libk/error.h kernel/scheduler.h
$(BUILD_DIR)/kmemmove.o: libk/kmemmove.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kprintf.o: libk/kprintf.c libk/kstdio.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/scheduler.h
$(BUILD_DIR)/kputs.o: libk/kputs.c arch/soclib/hal_arch.h cpu/mipsel/stdint.h \
 libk/klist.h arch/soclib/hal_arch_code.h arch/soclib/segmentation.h \
 kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h kernel/config.h \
 arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/kstdio.h \
 libk/libk.h libk/kstdlib.h kernel/ctype.h libk/error.h \
 kernel/scheduler.h
$(BUILD_DIR)/krand.o: libk/krand.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/ksnprintf.o: libk/ksnprintf.c libk/kstdio.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/scheduler.h
$(BUILD_DIR)/kstrcmp.o: libk/kstrcmp.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kstrcpy.o: libk/kstrcpy.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kstrdup.o: libk/kstrdup.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kstring.o: libk/kstring.c libk/kstdio.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/scheduler.h
$(BUILD_DIR)/kstrlen.o: libk/kstrlen.c libk/kstdlib.h kernel/ctype.h
$(BUILD_DIR)/kvsnprintf.o: libk/kvsnprintf.c libk/kstdio.h arch/soclib/hal_arch.h \
 cpu/mipsel/stdint.h libk/klist.h arch/soclib/hal_arch_code.h \
 arch/soclib/segmentation.h kernel/hal_cpu.h cpu/mipsel/hal_cpu_code.h \
 kernel/config.h arch/soclib/driver_cpu.h arch/soclib/driver_tty.h \
 arch/soclib/driver_timer.h arch/soclib/driver_icu.h libk/libk.h \
 libk/kstdlib.h kernel/ctype.h libk/error.h kernel/scheduler.h
