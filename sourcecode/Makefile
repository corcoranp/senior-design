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
	PROJECT		=SeniorDesign
	TARGET		=ANGSTROM
	BIN			=blazebot

#could just compile for everyone....
	ifeq ($(TARGET),ANGSTROM)
		# ANGSTROM: Used to indicate BeagleBone
		BUILD_DIR	=./builds/angstrom
		
		#Compiler & Linker to use when building for BeagleBone 
		# Added as environment variable
		CC = $(CPP_BBB_COMPILER)
		LD = $(CPP_BBB_LINKER)
		
	else ifeq ($(TARGET),RPI)
		# RPI: 		Used to indicate Raspberry Pi
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
# SOURCE FILES & DIRECTORIES
#---------------------------------------------------------------------
#Binary Name & Location

# Directory for C-Source
	vpath %.cpp $(CURDIR)/source

# Directory for includes
	CINCLUDE = $(CURDIR)/include

# Directory for object files
	OBJDIR = $(CURDIR)/object
 
# Other dependencies
	DEPS = \
 		Makefile \
 		include/*.h

# Compiler object files 
	COBJ = \
 $(OBJDIR)/bootloader.o



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
		CFLAGS += -std=c++0x

	else ifeq ($(TARGET),RPI)

		#Raspberry Pi options
		
		CFLAGS += -mcpu=cortex-a7
		CFLAGS += -march=armv7-a
		CFLAGS += -mfloat-abi=hard
		CFLAGS += -mthumb
		CFLAGS += -mfpu=neon-vfpv4
		
	else
		CFLAGS += -std=c++11x		
	endif

	CFLAGS += -O0 
	CFLAGS += -g 
	CFLAGS += -I. 
	CFLAGS += -I$(CINCLUDE) 
	CFLAGS += $(CDEFINE) 
	
#---------------------------------------------------------------------
# BUILD TARGETS
#---------------------------------------------------------------------

#all builds the 
all: $(BUILD_DIR)/$(BIN)
	@echo 'Building target: $@'
	@echo $(MSG_EMPTYLINE) 
	
	
# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN): $(COBJ) 
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_LINKING)
	$(LD) -o $@ $^ $(CFLAGS) 
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_SUCCESS) $(BIN)

# Compiler call
$(COBJ): $(OBJDIR)/%.o: %.cpp $(DEPS)

	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_COMPILING) $<
	$(CC) -c -o $@ $< $(CFLAGS) 
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_SUCCESS) $(BIN)

clean:
	@echo Clean Builds
	$(REMOVE) $(OBJDIR)/*.o
	$(REMOVE) $(BIN)

print: *.cpp
	lpr -p $?
	