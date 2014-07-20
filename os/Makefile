DIR = src_sys

#------------------------------------------------------------------------------
# MAKE_OPT     : option of make 
# DEBUG_MODE   : O = no message debug, 1 = message debug 
# TAGS_MODE    : tags mode (maybe ctags or etags)
# DIR          : subdirectories to make
#------------------------------------------------------------------------------

DEBUG_MODE ?= 3
MAKE_OPT   ?= 
TAGS_MODE  ?= ctags
#DIR        ?= undef

ifeq ($(DIR),undef)
    $(error "[ERROR] DIR undefined")
endif

TARGET = all print clean cleanall tags
.PHONY : $(TARGET)

#dep:$(addsuffix /dep, $(DIR))

all:$(addsuffix /all, $(DIR))

print:$(addsuffix /print, $(DIR))

clean:$(addsuffix /clean, $(DIR))

cleanall:$(addsuffix /cleanall, $(DIR))
	$(RM) .DS_Store */.DS_Store tags *~ */*~ */*/*~ */*/*/*~ */*/*/*/*~ 2>/dev/null||true

tags:
	@echo [TAGS] $(DIR) sources
	$(TAGS_MODE) -R $(DIR)

$(foreach target,$(TARGET),\
    $(foreach dir,$(DIR),\
        $(eval $(dir)/$(target):;\
            @cd $(dir)&&$(MAKE) $(MAKE_OPT) MAKE_OPT=$(MAKE_OPT) DEBUG_MODE=$(DEBUG_MODE) $(target)\
        )\
    )\
)
