# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
# ====================================================================
#
# Makefile.inc Master makefile definitions.
# Common params:
#               BUILD       Build version
#                           RRELEASE (default)
#                           DEBUG
#               TARGET      Build application target
#                           EXE Console executable (default)
#                           GUI Windows GUI executable
#                           DLL Dynamic Link Library
#                           LIB Static library
#               STATIC_CRT  Link to static MSVCRT library
#                           by default binaries are linked
#                           to msvcrt.dll
# Environment variables  used:
#           EXTRA_CFLAGS    Added to the common CFLAGS
#           EXTRA_CXXFLAGS  Added to the common CXXFLAGS
#           EXTRA_LIBS      Added to the common LIBS
#           EXTRA_LFLAGS    Added to the common LFLAGS
#           EXTRA_RCFLAGS   Added to the common RCFLAGS
#
# Compiler tools environment variables:
#                 CC        C compiler  (defaults to cl.exe)
#                 LN        Linker (defaults to link.exe)
#                 AR        Lib (defaults to lib.exe)
#                 RC        Resource compiler (defaults to rc.exe)
#                 MT        Manifest toolkit (defaults to mt.exe)
#
# Originally contributed by Mladen Turk <mturk jboss.com>
#
# ====================================================================
#

CC = cl.exe
LN = link.exe
RC = rc.exe
MT = mt.exe
AR = lib.exe

CPU=X64
MACHINE=X64
WINVER=0x0601

!IF !DEFINED(TARGET) || "$(TARGET)" == ""
TARGET=EXE
!ENDIF

!IF "$(TARGET)" != "EXE"
!IF "$(TARGET)" != "GUI"
!IF "$(TARGET)" != "DLL"
!IF "$(TARGET)" != "LIB"
!ERROR Must specify TARGET environment variable (EXE, GUI, DLL, LIB)
!ENDIF
!ENDIF
!ENDIF
!ENDIF

!IF DEFINED(DEBUG)
BUILD=DEBUG
!ELSE
!IF !DEFINED(BUILD)
BUILD=RELEASE
!ENDIF
!ENDIF

!IF "$(BUILD)" != "RELEASE"
!IF "$(BUILD)" != "DEBUG"
!ERROR Must specify BUILD environment variable (RELEASE, DEBUG)
!ENDIF
!ENDIF

# Common flags for all platforms
CMN_CFLAGS = -DWIN32 -D_WIN32 -D_WINDOWS -D_WINNT
CMN_CFLAGS = $(CMN_CFLAGS) -D_WIN32_WINNT=$(WINVER) -DWINVER=$(WINVER)
CMN_CFLAGS = $(CMN_CFLAGS) -DWIN32_LEAN_AND_MEAN
!IF "$(TARGET)" == "EXE"
CMN_CFLAGS = $(CMN_CFLAGS) -D_CONSOLE
!ENDIF

# All supported platforms support unicode
CMN_CFLAGS = $(CMN_CFLAGS) -D_UNICODE -DUNICODE -D_CRT_SECURE_NO_WARNINGS
CMN_CFLAGS = $(CMN_CFLAGS) -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE

CPU_CFLAGS = -D_AMD64_=1 -DWIN64 -D_WIN64

!IF DEFINED(STATIC_CRT)
CRT_BUILD  = MT
CRT_CFLAGS = -MT
EXTRA_LIBS =
!ELSE
CRT_BUILD  = MD
CRT_CFLAGS = -MD
!ENDIF

# Mark that extern C newer throws C++ exception
OPT_CFLAGS = -EHsc -W3 -Zi 

!IF "$(BUILD)" == "DEBUG"
CRT_CFLAGS = $(CRT_CFLAGS)d
OPT_CFLAGS = $(OPT_CFLAGS) -O2 -Ob2 -DNDEBUG
!IF DEFINED(RTC_CHECK)
OPT_CFLAGS = -RTC1 $(OPT_CFLAGS)
!ENDIF
!ELSE
OPT_CFLAGS = $(OPT_CFLAGS) -Od -DDEBUG -D_DEBUG
!ENDIF

# Combine all XXX_CFLAGS
CFLAGS = /c /nologo $(CRT_CFLAGS) $(OPT_CFLAGS) $(CPU_CFLAGS) $(CMN_CFLAGS)

!IF DEFINED(EXTRA_CFLAGS)
CFLAGS = $(CFLAGS) $(EXTRA_CFLAGS)
!ENDIF

# Cleanup CXXFLAGS
CXXFLAGS =
!IF DEFINED(EXTRA_CXXFLAGS)
CXXFLAGS = $(EXTRA_CXXFLAGS)
!ENDIF

# Linker section
LIBS = kernel32.lib user32.lib gdi32.lib advapi32.lib
LIBS = $(LIBS) shell32.lib comdlg32.lib comctl32.lib shlwapi.lib
# Extra libs from command line or env
!IF DEFINED(EXTRA_LIBS)
LIBS = $(LIBS) $(EXTRA_LIBS)
!ENDIF

!IF DEFINED(RTC_CHECK) && "$(BUILD)" == "DEBUG"
LIBS = $(LIBS) RunTmChk.lib
!ENDIF


LFLAGS = /nologo /MACHINE:$(MACHINE)
!IF "$(TARGET)" == "EXE"
LFLAGS = $(LFLAGS) /SUBSYSTEM:CONSOLE
!ELSEIF "$(TARGET)" == "GUI"
LFLAGS = $(LFLAGS) /SUBSYSTEM:WINDOWS
!ELSEIF "$(TARGET)" == "DLL"
LFLAGS = $(LFLAGS) /DLL /SUBSYSTEM:WINDOWS
!ENDIF

!IF "$(TARGET)" != "LIB"
!IF "$(BUILD)" == "RELEASE"
LFLAGS = $(LFLAGS) /OPT:REF
!ENDIF
# Always add debugging to the linker
LFLAGS = $(LFLAGS) /INCREMENTAL:NO /DEBUG
!ENDIF

!IF DEFINED(EXTRA_LFLAGS)
LFLAGS = $(LFLAGS) $(EXTRA_LFLAGS)
!ENDIF

# Resource compiler flags

RCFLAGS = /l 0x409 /n
RCFLAGS = $(RCFLAGS) /d WINVER=$(WINVER) /d _WIN32_WINNT=$(WINVER)
!IF "$(BUILD)" == "RELEASE"
RCFLAGS = $(RCFLAGS) /d "NDEBUG"
!ELSE
RCFLAGS = $(RCFLAGS) /d "_DEBUG" /d "DEBUG"
!ENDIF
!IF DEFINED(EXTRA_RCFLAGS)
RCFLAGS = $(RCFLAGS) $(EXTRA_RCFLAGS)
!ENDIF


# Build Target dir e.g. X64_DLL_RELEASE_MD
WORKDIR = $(CPU)_$(TARGET)_$(BUILD)_$(CRT_BUILD)
WORKDIR_DLL = $(CPU)_DLL_$(BUILD)_$(CRT_BUILD)
WORKDIR_LIB = $(CPU)_LIB_$(BUILD)_$(CRT_BUILD)
WORKDIR_EXE = $(CPU)_EXE_$(BUILD)_$(CRT_BUILD)

CLEANTARGET=if exist "$(WORKDIR)\" rd /S /Q $(WORKDIR)
MAKEWORKDIR=if not exist "$(WORKDIR)\" mkdir $(WORKDIR)
MAKEINSTALL=if not exist "$(INSTALLLOC)\" mkdir "$(INSTALLLOC)"

!IF DEFINED(JAVA_HOME)
JAVA_INCLUDES = -I "$(JAVA_HOME)\include" -I "$(JAVA_HOME)\include\win32"
!ENDIF
