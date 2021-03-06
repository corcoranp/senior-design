#---------------------------------------------------------------------
# MAKEFILE for UAB SeniorDesign Project
# BY Peter Corcoran
# File is used to build BeagleBone Black ANGSTROM binary via 
# Cross-Compiling
# 
#---------------------------------------------------------------------
# PROJECT SETTINGS
#
#	TARGET	= used to define what platform to build
#	BIN		= default binary name 
#
#---------------------------------------------------------------------
PROJECT=SeniorDesign
TARGET=ANGSTROM
BIN=blazebot
RES_DIR=resources
# ANGSTROM: Used to indicate BeagleBone
BUILD_DIR	=./builds
	
#Compiler & Linker to use when building for BeagleBone 
# Added as environment variable
#CC = $(CPP_BBB_COMPILER)
#LD = $(CPP_BBB_LINKER)
CC = /usr/local/linaro/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
LD = /usr/local/linaro/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++

#---------------------------------------------------------------------
# DIRECTORIES
#---------------------------------------------------------------------

# Directory for includes
CINCLUDE=include
SRCDIR=source
OBJDIR=object

#---------------------------------------------------------------------
# EXECUTABLES
# Setup of variables that can be used in place of command
#---------------------------------------------------------------------	
# rm is part of yagarto-tools
#SHELL  = cmd
REMOVE = rm -f
COPY = cp -f

	
#---------------------------------------------------------------------
# ECHO STATEMENTS
#---------------------------------------------------------------------	
# for a better output
MSG_EMPTYLINE 	= . 
MSG_COMPILING 	= ---COMPILE--- 
MSG_LINKING 	= ---LINK--- 
MSG_SUCCESS 	= ---SUCCESS--- 
		
#---------------------------------------------------------------------
# Options Controlling C Dialect
#---------------------------------------------------------------------
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
#---------------------------------------------------------------------
	
#Select between generating code that executes in ARM and Thumb states.
CFLAGS = -marm
CXXFLAGS = -marm
# Two additional CFLAGS must be used for Angstrom
# They must not be used for Debian or Ubuntu. I couldn't find out why. 
# The hint came from C:\gcc-linaro\share\doc\gcc-linaro-arm-linux-gnueabihf\README.txt 
ifeq ($(TARGET),ANGSTROM)
	#BBB options		
	CFLAGS += -march=armv4t
	CFLAGS += -mfloat-abi=soft
	
	CXXFLAGS += -march=armv4t
	CXXFLAGS += -mfloat-abi=soft
endif

CFLAGS += -O0 
CFLAGS += -g 
CFLAGS += -I. 
CFLAGS += -I$(CINCLUDE) 
CFLAGS += $(CDEFINE)

#C++ FLAGS NEEDED FOR THREADING------------------ 
CXXFLAGS += --std=c++11 -O0 -g -I.
CXXFLAGS += -I$(CINCLUDE) 
CXXFLAGS += -pthread -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8


#---------------------------------------------------------------------
# SOURCE FILES
#---------------------------------------------------------------------

# wildcard search for files in the source or include directory and sub dirs.
SOURCES  := $(wildcard source/*.cpp) $(wildcard source/**/*.cpp)
INCLUDES := $(wildcard include/*.h) $(wildcard include/**/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

#---------------------------------------------------------------------
# BUILD TARGETS
#---------------------------------------------------------------------

#all builds the 
all: $(BUILD_DIR)/$(BIN)
	@echo 'Building target: $@'
	@echo $(MSG_EMPTYLINE) 
	
# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN): $(OBJECTS)    
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_LINKING)
# $(LD) -o $@ $^ $(CFLAGS)
	$(LD) -o $@ $^ $(CXXFLAGS)
	@echo $(MSG_SUCCESS)

# Compiler call
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_COMPILING) $<
# $(CC) -c -o $@ $< $(CFLAGS)
	$(CC) -c -o $@ $< $(CXXFLAGS)
	@echo $(MSG_SUCCESS)
	
clean:
	@echo Clean Builds
	$(REMOVE) $(OBJDIR)/*.o $(OBJDIR)/**/*.o
	$(REMOVE) $(BIN)

	