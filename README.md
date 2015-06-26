# senior-design Project
EE 498-499 UAB Senior Design Project Work

By: Peter M. Corcoran

Copyright © 2015


## Overview

## Environment Setup
### Windows 

#### Setup Cross-Compiling
A step-by-step guide is available here: http://jkuhlm.bplaced.net/hellobone/
The documentation folder contains the PDF version of the site above.

This project was setup using this method, however other build configurations have been created. 

#### Setup Environment Variable 
On Windows System Add Environment Variable: LINARO_CPP_COMPILER point var to: <your install path>\gcc-linaro\bin\arm-linux-gnueabihf-g++.exe for example: 

```
D:\Development\BBB\gcc-linaro\bin\arm-linux-gnueabihf-g++.exe
```

## Build Configurations
### BeagleBuild
The BeagleBuild will use the Linaro compiler to create a cross-compiled binary for the BeagleBone Black

### Debug
THe Debug build configuration uses Eclipse's built in Debug options to produce a windows executable.

## Styling in this document
```
This is a code block
```

*Italic characters*
_Italic characters_
**bold characters**
__bold characters__
~~strikethrough text~~

Introducing my quote:
  
> Neque porro quisquam est qui
> dolorem ipsum quia dolor sit amet,
> consectetur, adipisci velit...
