# Microsoft Developer Studio Generated NMAKE File, Based on ember_game_server.dsp
!IF "$(CFG)" == ""
CFG=ember_game_server - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ember_game_server - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ember_game_server - Win32 Release" && "$(CFG)" != "ember_game_server - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ember_game_server.mak" CFG="ember_game_server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ember_game_server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ember_game_server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ember_game_server - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\ember_game_server.exe" "$(OUTDIR)\ember_game_server.bsc"


CLEAN :
	-@erase "$(INTDIR)\b_ability.obj"
	-@erase "$(INTDIR)\b_ability.sbr"
	-@erase "$(INTDIR)\b_cfmms.obj"
	-@erase "$(INTDIR)\b_cfmms.sbr"
	-@erase "$(INTDIR)\b_class.obj"
	-@erase "$(INTDIR)\b_class.sbr"
	-@erase "$(INTDIR)\b_map.obj"
	-@erase "$(INTDIR)\b_map.sbr"
	-@erase "$(INTDIR)\b_netaddress.obj"
	-@erase "$(INTDIR)\b_netaddress.sbr"
	-@erase "$(INTDIR)\b_npc.obj"
	-@erase "$(INTDIR)\b_npc.sbr"
	-@erase "$(INTDIR)\b_race.obj"
	-@erase "$(INTDIR)\b_race.sbr"
	-@erase "$(INTDIR)\b_toon.obj"
	-@erase "$(INTDIR)\b_toon.sbr"
	-@erase "$(INTDIR)\c_log.obj"
	-@erase "$(INTDIR)\c_log.sbr"
	-@erase "$(INTDIR)\c_socket.obj"
	-@erase "$(INTDIR)\c_socket.sbr"
	-@erase "$(INTDIR)\dlstorm.obj"
	-@erase "$(INTDIR)\dlstorm.sbr"
	-@erase "$(INTDIR)\ember_game_server.obj"
	-@erase "$(INTDIR)\ember_game_server.sbr"
	-@erase "$(INTDIR)\fm_util.obj"
	-@erase "$(INTDIR)\fm_util.sbr"
	-@erase "$(INTDIR)\s_client.obj"
	-@erase "$(INTDIR)\s_client.sbr"
	-@erase "$(INTDIR)\s_server.obj"
	-@erase "$(INTDIR)\s_server.sbr"
	-@erase "$(INTDIR)\s_serverinfo.obj"
	-@erase "$(INTDIR)\s_serverinfo.sbr"
	-@erase "$(INTDIR)\s_socket.obj"
	-@erase "$(INTDIR)\s_socket.sbr"
	-@erase "$(INTDIR)\s_win32.obj"
	-@erase "$(INTDIR)\s_win32.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ember_game_server.bsc"
	-@erase ".\ember_game_server.exe"
	-@erase ".\ember_game_server.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_EMBER_SERVER_" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ember_game_server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ember_game_server.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\b_ability.sbr" \
	"$(INTDIR)\b_cfmms.sbr" \
	"$(INTDIR)\b_class.sbr" \
	"$(INTDIR)\b_map.sbr" \
	"$(INTDIR)\b_netaddress.sbr" \
	"$(INTDIR)\b_npc.sbr" \
	"$(INTDIR)\b_race.sbr" \
	"$(INTDIR)\b_toon.sbr" \
	"$(INTDIR)\c_log.sbr" \
	"$(INTDIR)\c_socket.sbr" \
	"$(INTDIR)\dlstorm.sbr" \
	"$(INTDIR)\ember_game_server.sbr" \
	"$(INTDIR)\fm_util.sbr" \
	"$(INTDIR)\s_client.sbr" \
	"$(INTDIR)\s_server.sbr" \
	"$(INTDIR)\s_serverinfo.sbr" \
	"$(INTDIR)\s_socket.sbr" \
	"$(INTDIR)\s_win32.sbr"

"$(OUTDIR)\ember_game_server.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclient.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ember_game_server.pdb" /machine:I386 /out:"ember_game_server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\b_ability.obj" \
	"$(INTDIR)\b_cfmms.obj" \
	"$(INTDIR)\b_class.obj" \
	"$(INTDIR)\b_map.obj" \
	"$(INTDIR)\b_netaddress.obj" \
	"$(INTDIR)\b_npc.obj" \
	"$(INTDIR)\b_race.obj" \
	"$(INTDIR)\b_toon.obj" \
	"$(INTDIR)\c_log.obj" \
	"$(INTDIR)\c_socket.obj" \
	"$(INTDIR)\dlstorm.obj" \
	"$(INTDIR)\ember_game_server.obj" \
	"$(INTDIR)\fm_util.obj" \
	"$(INTDIR)\s_client.obj" \
	"$(INTDIR)\s_server.obj" \
	"$(INTDIR)\s_serverinfo.obj" \
	"$(INTDIR)\s_socket.obj" \
	"$(INTDIR)\s_win32.obj"

".\ember_game_server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ember_game_server - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ember_game_server.exe" "$(OUTDIR)\ember_game_server.bsc"


CLEAN :
	-@erase "$(INTDIR)\b_ability.obj"
	-@erase "$(INTDIR)\b_ability.sbr"
	-@erase "$(INTDIR)\b_cfmms.obj"
	-@erase "$(INTDIR)\b_cfmms.sbr"
	-@erase "$(INTDIR)\b_class.obj"
	-@erase "$(INTDIR)\b_class.sbr"
	-@erase "$(INTDIR)\b_map.obj"
	-@erase "$(INTDIR)\b_map.sbr"
	-@erase "$(INTDIR)\b_netaddress.obj"
	-@erase "$(INTDIR)\b_netaddress.sbr"
	-@erase "$(INTDIR)\b_npc.obj"
	-@erase "$(INTDIR)\b_npc.sbr"
	-@erase "$(INTDIR)\b_race.obj"
	-@erase "$(INTDIR)\b_race.sbr"
	-@erase "$(INTDIR)\b_toon.obj"
	-@erase "$(INTDIR)\b_toon.sbr"
	-@erase "$(INTDIR)\c_log.obj"
	-@erase "$(INTDIR)\c_log.sbr"
	-@erase "$(INTDIR)\c_socket.obj"
	-@erase "$(INTDIR)\c_socket.sbr"
	-@erase "$(INTDIR)\dlstorm.obj"
	-@erase "$(INTDIR)\dlstorm.sbr"
	-@erase "$(INTDIR)\ember_game_server.obj"
	-@erase "$(INTDIR)\ember_game_server.sbr"
	-@erase "$(INTDIR)\fm_util.obj"
	-@erase "$(INTDIR)\fm_util.sbr"
	-@erase "$(INTDIR)\s_client.obj"
	-@erase "$(INTDIR)\s_client.sbr"
	-@erase "$(INTDIR)\s_server.obj"
	-@erase "$(INTDIR)\s_server.sbr"
	-@erase "$(INTDIR)\s_serverinfo.obj"
	-@erase "$(INTDIR)\s_serverinfo.sbr"
	-@erase "$(INTDIR)\s_socket.obj"
	-@erase "$(INTDIR)\s_socket.sbr"
	-@erase "$(INTDIR)\s_win32.obj"
	-@erase "$(INTDIR)\s_win32.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ember_game_server.bsc"
	-@erase "$(OUTDIR)\ember_game_server.exe"
	-@erase "$(OUTDIR)\ember_game_server.ilk"
	-@erase "$(OUTDIR)\ember_game_server.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_EMBER_SERVER_" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ember_game_server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ember_game_server.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\b_ability.sbr" \
	"$(INTDIR)\b_cfmms.sbr" \
	"$(INTDIR)\b_class.sbr" \
	"$(INTDIR)\b_map.sbr" \
	"$(INTDIR)\b_netaddress.sbr" \
	"$(INTDIR)\b_npc.sbr" \
	"$(INTDIR)\b_race.sbr" \
	"$(INTDIR)\b_toon.sbr" \
	"$(INTDIR)\c_log.sbr" \
	"$(INTDIR)\c_socket.sbr" \
	"$(INTDIR)\dlstorm.sbr" \
	"$(INTDIR)\ember_game_server.sbr" \
	"$(INTDIR)\fm_util.sbr" \
	"$(INTDIR)\s_client.sbr" \
	"$(INTDIR)\s_server.sbr" \
	"$(INTDIR)\s_serverinfo.sbr" \
	"$(INTDIR)\s_socket.sbr" \
	"$(INTDIR)\s_win32.sbr"

"$(OUTDIR)\ember_game_server.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ember_game_server.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ember_game_server.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\b_ability.obj" \
	"$(INTDIR)\b_cfmms.obj" \
	"$(INTDIR)\b_class.obj" \
	"$(INTDIR)\b_map.obj" \
	"$(INTDIR)\b_netaddress.obj" \
	"$(INTDIR)\b_npc.obj" \
	"$(INTDIR)\b_race.obj" \
	"$(INTDIR)\b_toon.obj" \
	"$(INTDIR)\c_log.obj" \
	"$(INTDIR)\c_socket.obj" \
	"$(INTDIR)\dlstorm.obj" \
	"$(INTDIR)\ember_game_server.obj" \
	"$(INTDIR)\fm_util.obj" \
	"$(INTDIR)\s_client.obj" \
	"$(INTDIR)\s_server.obj" \
	"$(INTDIR)\s_serverinfo.obj" \
	"$(INTDIR)\s_socket.obj" \
	"$(INTDIR)\s_win32.obj"

"$(OUTDIR)\ember_game_server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ember_game_server.dep")
!INCLUDE "ember_game_server.dep"
!ELSE 
!MESSAGE Warning: cannot find "ember_game_server.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ember_game_server - Win32 Release" || "$(CFG)" == "ember_game_server - Win32 Debug"
SOURCE=..\b_ability.cpp

"$(INTDIR)\b_ability.obj"	"$(INTDIR)\b_ability.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_cfmms.cpp

"$(INTDIR)\b_cfmms.obj"	"$(INTDIR)\b_cfmms.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_class.cpp

"$(INTDIR)\b_class.obj"	"$(INTDIR)\b_class.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_map.cpp

"$(INTDIR)\b_map.obj"	"$(INTDIR)\b_map.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_netaddress.cpp

"$(INTDIR)\b_netaddress.obj"	"$(INTDIR)\b_netaddress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_npc.cpp

"$(INTDIR)\b_npc.obj"	"$(INTDIR)\b_npc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_race.cpp

"$(INTDIR)\b_race.obj"	"$(INTDIR)\b_race.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\b_toon.cpp

"$(INTDIR)\b_toon.obj"	"$(INTDIR)\b_toon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\dlstorm\c_log.cpp

"$(INTDIR)\c_log.obj"	"$(INTDIR)\c_log.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\c_socket.cpp

"$(INTDIR)\c_socket.obj"	"$(INTDIR)\c_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\dlstorm\dlstorm.cpp

"$(INTDIR)\dlstorm.obj"	"$(INTDIR)\dlstorm.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ember_game_server.cpp

"$(INTDIR)\ember_game_server.obj"	"$(INTDIR)\ember_game_server.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\fm_util.cpp

"$(INTDIR)\fm_util.obj"	"$(INTDIR)\fm_util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\s_client.cpp

"$(INTDIR)\s_client.obj"	"$(INTDIR)\s_client.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\s_server.cpp

"$(INTDIR)\s_server.obj"	"$(INTDIR)\s_server.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\s_serverinfo.cpp

"$(INTDIR)\s_serverinfo.obj"	"$(INTDIR)\s_serverinfo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\s_socket.cpp

"$(INTDIR)\s_socket.obj"	"$(INTDIR)\s_socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\s_win32.cpp

"$(INTDIR)\s_win32.obj"	"$(INTDIR)\s_win32.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

