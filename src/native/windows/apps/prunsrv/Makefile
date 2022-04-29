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

TARGET = EXE
PROJECT = prunsrv

SRCDIR = .\..\..
!IF !DEFINED(PREFIX) || "$(PREFIX)" == ""
PREFIX = .\..\..\..\..\..\target
!ENDIF
INSTALLLOC = $(PREFIX)

# Include common definitions
!include <..\..\include\Makefile.inc>

LFLAGS = $(LFLAGS) /VERSION:1.3
INCLUDES = -I $(SRCDIR)\include -I $(SRCDIR)\src $(JAVA_INCLUDES)
PDBFLAGS = -Fo$(WORKDIR)\ -Fd$(WORKDIR)\$(PROJECT)

OBJECTS = \
	$(WORKDIR)\cmdline.obj \
	$(WORKDIR)\console.obj \
	$(WORKDIR)\gui.obj \
	$(WORKDIR)\handles.obj \
	$(WORKDIR)\javajni.obj \
	$(WORKDIR)\log.obj \
	$(WORKDIR)\mclib.obj \
	$(WORKDIR)\registry.obj \
	$(WORKDIR)\rprocess.obj \
	$(WORKDIR)\security.obj \
	$(WORKDIR)\service.obj \
	$(WORKDIR)\utils.obj \
	$(WORKDIR)\$(PROJECT).obj \
	$(WORKDIR)\$(PROJECT).res

BUILDEXE = $(WORKDIR)\$(PROJECT).exe
BUILDPDB = $(WORKDIR)\$(PROJECT).pdb

all : $(WORKDIR) $(BUILDEXE)

$(WORKDIR):
	@$(MAKEWORKDIR)

{$(SRCDIR)\src}.c{$(WORKDIR)}.obj:
	$(CC) $(CFLAGS) $(INCLUDES) $(PDBFLAGS) $<

{$(SRCDIR)\apps\$(PROJECT)}.c{$(WORKDIR)}.obj:
	$(CC) $(CFLAGS) $(INCLUDES) $(PDBFLAGS) $<

{$(SRCDIR)\apps\$(PROJECT)}.rc{$(WORKDIR)}.res:
	$(RC) $(RCFLAGS) /i $(SRCDIR)\include /fo $@ $<

$(BUILDEXE): $(OBJECTS)
	$(LN) $(LFLAGS) $(OBJECTS) $(LIBS) /pdb:$(BUILDPDB) /out:$(BUILDEXE)

clean:
	@$(CLEANTARGET)

install: all
	@$(MAKEINSTALL)
	@xcopy /I /Y /Q "$(WORKDIR)\*.exe" "$(INSTALLLOC)"
	@xcopy /I /Y /Q "$(WORKDIR)\*.pdb" "$(INSTALLLOC)"
