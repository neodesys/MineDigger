################################################################################
#
# MineDigger
#
# Copyright (C) 2015, Loïc Le Page
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################

#Build configuration
#
#Available options are:
#  BUILD=[debug] or release
#  PLATFORM=[linux] or mingw
#  ARCH=[64] or 32
#
BUILD?=debug
PLATFORM?=linux
ARCH?=64

ifneq ($(BUILD),debug)
    ifneq ($(BUILD),release)
        $(error Invalid BUILD type, please select "debug" or "release")
    endif
endif

ifneq ($(PLATFORM),linux)
    ifneq ($(PLATFORM),mingw)
        $(error Invalid PLATFORM type, please select "linux" or "mingw")
    endif
endif

ifneq ($(ARCH),64)
    ifneq ($(ARCH),32)
        $(error Invalid ARCH type, please select "64" or "32")
    endif
endif

#Project definitions
PROJECT_NAME=MineDigger

SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

SRC_PRECOMP_HEADER=precomp.h

EXT_DIR=ext
SDL2_VERSION=2.0.3

#Build paths and tools
ifeq ($(PLATFORM),linux)
    CXX=g++
else
    ifeq ($(ARCH),64)
        CXX=x86_64-w64-mingw32-g++
    else
        CXX=i686-w64-mingw32-g++
    endif
endif

LD=$(CXX)
MKDIR=mkdir -p
RMDIR=rmdir --ignore-fail-on-non-empty
RM=rm -rf
CP=cp -f
SED=sed

#Compiler/Linker default options
CXXFLAGS+=-Wall -Werror -std=c++11 -pedantic -fno-rtti
LDLIBS=-lSDL2

ifeq ($(ARCH),64)
    CXXFLAGS+=-m64
    LDFLAGS+=-m64
else
    CXXFLAGS+=-m32
    LDFLAGS+=-m32
endif

ifeq ($(PLATFORM),linux)
    CXXFLAGS+=-pthread
    LDFLAGS+=-pthread
endif

ifeq ($(BUILD),debug)
    CXXFLAGS:=-g $(CXXFLAGS)
else
    CXXFLAGS:=-O3 -s -DNDEBUG $(CXXFLAGS)
    LDFLAGS:=-s $(LDFLAGS)
endif

ifeq ($(PLATFORM),mingw)
    INCDIRS+=-I$(EXT_DIR)/SDL2-$(SDL2_VERSION)/include
    LIBDIRS+=-L$(EXT_DIR)/SDL2-$(SDL2_VERSION)/lib/$(PLATFORM)$(ARCH)
    LDFLAGS+=-static-libgcc -static-libstdc++ -mwindows
endif

#Build variables
TARGET=$(BIN_DIR)/$(PLATFORM)$(ARCH)/$(PROJECT_NAME)_$(BUILD)
OBJ_DIR=$(BUILD_DIR)/$(PLATFORM)$(ARCH)_$(BUILD)

ifeq ($(PLATFORM),mingw)
    TARGET:=$(TARGET).exe
endif

RECFIND_CPP=$(strip $(wildcard $1/*.cpp) $(foreach d, $(wildcard $1/*), $(call RECFIND_CPP, $d)))
SRC_FILES=$(call RECFIND_CPP, $(SRC_DIR))

OBJ_FILES=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES=$(patsubst %.cpp, $(OBJ_DIR)/%.d, $(SRC_FILES))

#Build targets and rules
.PHONY: all clean clean-all

ifeq ($(PLATFORM),mingw)
    SDL2_DLL_TARGET=$(BIN_DIR)/$(PLATFORM)$(ARCH)/SDL2.dll
    all: $(TARGET) $(SDL2_DLL_TARGET)

    $(SDL2_DLL_TARGET): $(EXT_DIR)/SDL2-$(SDL2_VERSION)/bin/win$(ARCH)/SDL2.dll
	@$(MKDIR) $(@D)
	@$(CP) $< $@
else
    all: $(TARGET)
endif

clean:
	$(info Cleaning $(BUILD) build on $(PLATFORM) $(ARCH) bits...)
	@$(RM) $(OBJ_DIR) $(TARGET)
	@-$(RMDIR) $(BIN_DIR)/$(PLATFORM)$(ARCH) $(BIN_DIR) $(BUILD_DIR) 2>/dev/null

clean-all:
	$(info Cleaning all builds on all platforms...)
	@$(RM) $(BIN_DIR) $(BUILD_DIR)

$(TARGET): $(OBJ_FILES)
	$(info Linking $(BUILD) binary on $(PLATFORM) $(ARCH) bits...)
	@$(MKDIR) $(@D)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBDIRS) $(LDLIBS)

$(OBJ_DIR)/%.d: %.cpp
	@$(MKDIR) $(@D)
	@$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< 2>/dev/null | $(SED) "s#.*: *#$(OBJ_DIR)/$*.o $@: #g" > $@

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
    -include $(DEP_FILES)
endif

ifneq ($(strip $(SRC_PRECOMP_HEADER)),)
    $(OBJ_DIR)/$(SRC_PRECOMP_HEADER): $(SRC_DIR)/$(SRC_PRECOMP_HEADER)
	@$(MKDIR) $(@D)
	@$(CP) $< $@

    PRECOMP_DEP=$(SRC_PRECOMP_HEADER:%.h=%.d)

    $(OBJ_DIR)/$(PRECOMP_DEP): $(OBJ_DIR)/$(SRC_PRECOMP_HEADER)
	@$(MKDIR) $(@D)
	@$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< 2>/dev/null | $(SED) "s#.*: *#$(OBJ_DIR)/$(SRC_PRECOMP_HEADER).gch $@: #g" > $@

    ifeq ($(findstring clean,$(MAKECMDGOALS)),)
        -include $(OBJ_DIR)/$(PRECOMP_DEP)
    endif

    $(OBJ_DIR)/$(SRC_PRECOMP_HEADER).gch: $(OBJ_DIR)/$(SRC_PRECOMP_HEADER)
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $<

    $(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)/$(SRC_PRECOMP_HEADER).gch
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -include $(OBJ_DIR)/$(SRC_PRECOMP_HEADER) -o $@ -c $<
else
    $(OBJ_DIR)/%.o: %.cpp
	@$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o $@ -c $<
endif
