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

#could just compile for everyone....
ifeq ($(TARGET),ANGSTROM)
	# ANGSTROM: Used to indicate BeagleBone
	BUILD_DIR	=./builds/angstrom
	
	#Compiler & Linker to use when building for BeagleBone 
	# Added as environment variable
	CC = $(CPP_BBB_COMPILER)
	LD = $(CPP_BBB_LINKER)
	
else ifeq ($(TARGET),RPI)
	# RPI: 		Used to indicate Raspberry Pi (DOES NOT WORK YET!!!!!!!!!!!!!!!!!!!!!!!!)
	BUILD_DIR	=./builds/raspberry

	#Compiler & Linker to use when building for Raspberry Pi
	# Added as environment variable
	CC = $(CPP_RPI_COMPILER)
	LD = $(CPP_RPI_LINKER)
	
else
	# x86_64	Used to indicate Windows x86 or x64
	BUILD_DIR	=./builds/x86_64
	BIN			=blazebot.exe
	
	#Compiler & Linker to use when building for Windows...
	CC = $(LINARO_CPP_COMPILER)
	LD = $(LINARO_CPP_COMPILER)
	
endif

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
SHELL  = cmd
REMOVE = rm -f
	
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

# Two additional CFLAGS must be used for Angstrom
# They must not be used for Debian or Ubuntu. I couldn't find out why. 
# The hint came from C:\gcc-linaro\share\doc\gcc-linaro-arm-linux-gnueabihf\README.txt 
ifeq ($(TARGET),ANGSTROM)
	#BBB options		
	CFLAGS += -march=armv4t
	CFLAGS += -mfloat-abi=soft
#	CFLAGS += -std=c++0x

else ifeq ($(TARGET),RPI)
	#Raspberry Pi options
	CFLAGS += -mcpu=cortex-a7
	CFLAGS += -march=armv7-a
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mthumb
	CFLAGS += -mfpu=neon-vfpv4
	
else
	
endif

#CFLAGS += -std=c++11x 
CFLAGS += -O0 
CFLAGS += -g 
CFLAGS += -I. 
CFLAGS += -I$(CINCLUDE) 
CFLAGS += $(CDEFINE)

CFLAGS += -pthread
CFLAGS += -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 
CFLAGS += -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 
CFLAGS += -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 
CFLAGS += -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 

#C++ FLAGS NEEDED FOR THREADING------------------ 
CXXFLAGS = --std=c++11 -pthread -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8


#---------------------------------------------------------------------
# SOURCE FILES
#---------------------------------------------------------------------

SOURCES  := $(wildcard source/*.cpp)
INCLUDES := $(wildcard include/*.h)
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
	$(REMOVE) $(OBJDIR)/*.o
	$(REMOVE) $(BIN)

	