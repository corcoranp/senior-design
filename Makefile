#---------------------------------------------------------------------
# MAKEFILE for UAB SeniorDesign Project
# BY Peter Corcoran
# File is used to build BeagleBone Black ANGSTROM binary via 
# Cross-Compiling
# 
#---------------------------------------------------------------------
# PROJECT SETTINGS
#---------------------------------------------------------------------
	PROJECT	=SeniorDesign
	TARGET	=ANGSTROM #Used to indicate BeagleBone is running Angstrom
	BIN		=blazer
	BUILD_DIR=./angstromBuild

#---------------------------------------------------------------------
# FILES & DIRECTORIES
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
	COBJ = $(OBJDIR)/*.o 


#---------------------------------------------------------------------
# EXECUTABLES
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
# COMPILER OPTIONS
#---------------------------------------------------------------------

# g++ Binaries to Use
# On Windows System Add Environment Variable: LINARO_CPP_COMPILER
# point var to: <your install path>\gcc-linaro\bin\arm-linux-gnueabihf-g++.exe
# for example: D:\Development\BBB\gcc-linaro\bin\arm-linux-gnueabihf-g++.exe
	CC = $(LINARO_CPP_COMPILER)
	LD = $(LINARO_CPP_COMPILER)

# Two additional flags neccessary for Angstrom Linux. Don't use them with Ubuntu or Debian  
	CFLAGS = -marm

# Two additional CFLAGS must be used for Angstrom
# They must not be used for Debian or Ubuntu. I couldn't find out why. 
# The hint came from C:\gcc-linaro\share\doc\gcc-linaro-arm-linux-gnueabihf\README.txt 
	ifeq ($(TARGET),ANGSTROM)
		CFLAGS += -march=armv4t
		CFLAGS += -mfloat-abi=soft
	endif

	CFLAGS += -O0 
	CFLAGS += -g 
	CFLAGS += -I.
	CFLAGS += -I$(CINCLUDE)
	CFLAGS += $(CDEFINE)
	CFLAGS += -std=c++0x
	
#---------------------------------------------------------------------
# BUILD TARGETS
#---------------------------------------------------------------------

all: $(BUILD_DIR)/$(BIN) 

# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN) : $(COBJ)
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
	$(REMOVE) $(OBJDIR)/*.o
	$(REMOVE) $(BIN)

