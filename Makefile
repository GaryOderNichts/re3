RELEASE_BUILD := 1
CHANNEL_BUILD := 0

#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

include $(DEVKITPRO)/wut/share/wut_rules

#-------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#-------------------------------------------------------------------------------
TARGET		:=	re3
BUILD		:=	build
SOURCES		:=	src \
				src/animation \
				src/audio \
				src/audio/oal \
				src/audio/eax \
				src/buildings \
				src/collision \
				src/control \
				src/core \
				src/entities \
				src/extras \
				src/fakerw \
				src/math \
				src/modelinfo \
				src/objects \
				src/peds \
				src/render \
				src/rw \
				src/save \
				src/skel \
				src/skel/wiiu \
				src/skel/wiiu/debug \
				src/text \
				src/vehicles \
				src/weapons
DATA		:=	data
INCLUDES	:=	$(SOURCES) \
				vendor/librw \
				vendor/librw/inc \
				vendor/openal-soft/include \
				vendor/dr_libs

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
CFLAGS	:=	-Wall -O2 -ffunction-sections -Wno-strict-aliasing \
			-Wno-unknown-pragmas -Wno-parentheses -Wno-sign-compare -Wno-stringop-truncation -Wno-unused-variable \
			$(MACHDEP)

CFLAGS	+=	$(INCLUDE) -D__WIIU__ -D__WUT__ -DLIBRW -DAUDIO_OAL -DrwBIGENDIAN -DBIGENDIAN

CXXFLAGS	:= $(CFLAGS)

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	$(ARCH) $(RPXSPECS) -Wl,-Map,$(notdir $*.map)

ifeq ($(RELEASE_BUILD), 0)
	CFLAGS += -g -D_DEBUG_BUILD_
	CXXFLAGS += -g -D_DEBUG_BUILD_
	ASFLAGS += -g
	LDFLAGS += -g
endif

ifeq ($(CHANNEL_BUILD), 1)
	CFLAGS += -DWIIU_CHANNEL
	CXXFLAGS += -DWIIU_CHANNEL
endif

LIBS	:= -lrw -lmpg123 -lsndfile -lopenal -lSDL2 -lwut

#-------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level
# containing include and lib
#-------------------------------------------------------------------------------
LIBDIRS	:= $(PORTLIBS) $(WUT_ROOT) $(TOPDIR)/vendor/librw $(TOPDIR)/vendor/openal-soft


#-------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#-------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#-------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#-------------------------------------------------------------------------------
	export LD	:=	$(CC)
#-------------------------------------------------------------------------------
else
#-------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
export OFILES 		:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean vendor all

#-------------------------------------------------------------------------------
all: $(BUILD)

vendor:
	@echo Building librw
	@$(MAKE) --no-print-directory -C $(TOPDIR)/vendor/librw
	@echo Building openal-soft
	@$(MAKE) --no-print-directory -C $(TOPDIR)/vendor/openal-soft

$(BUILD): | vendor
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#-------------------------------------------------------------------------------
clean:
	@$(MAKE) --no-print-directory -C $(TOPDIR)/vendor/librw clean
	@$(MAKE) --no-print-directory -C $(TOPDIR)/vendor/openal-soft clean
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).rpx $(TARGET).elf $(TARGET).strip.elf

#-------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS	:=	$(OFILES:.o=.d)

#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
all	: $(OUTPUT).rpx

$(OUTPUT).rpx	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES_BIN)

#-------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#-------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#-------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------