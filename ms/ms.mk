##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=ms
ConfigurationName      :=Debug
WorkspacePath          := "/home/sparson/dev/ember/ms"
ProjectPath            := "/home/sparson/dev/ember/ms"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Seth Parson
Date                   :=23/05/16
CodeLitePath           :="/home/sparson/.codelite"
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)_DLCS_CONSOLE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ms.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/home/sparson/dev/dlstorm $(IncludeSwitch)/home/sparson/dev/SDK/sqlite $(IncludeSwitch)/home/sparson/dev/SDK/fmodapi44418linux/api/inc $(IncludeSwitch)/home/sparson/dev/SDK/assimp/include/assimp $(IncludeSwitch)/usr/include/lua50/ $(IncludeSwitch)/usr/include/GL $(IncludeSwitch)/home/sparson/dev/SDK/mysql/include/ $(IncludeSwitch)/usr/include/tcl8.6 $(IncludeSwitch)/home/sparson/dev/SDK/SDL-1.2.15/include  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS :=  -g -O0 -Wall -fpermissive -pthread $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -fpermissive -pthread $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(ObjectSuffix) $(IntermediateDirectory)/dlstorm_md5.cpp$(ObjectSuffix) $(IntermediateDirectory)/dlstorm_c_data.cpp$(ObjectSuffix) $(IntermediateDirectory)/dlstorm_c_log.cpp$(ObjectSuffix) $(IntermediateDirectory)/mantra_mantra.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(ObjectSuffix): ../../dlstorm/dlstorm.cpp $(IntermediateDirectory)/dlstorm_dlstorm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sparson/dev/dlstorm/dlstorm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(DependSuffix): ../../dlstorm/dlstorm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(DependSuffix) -MM "../../dlstorm/dlstorm.cpp"

$(IntermediateDirectory)/dlstorm_dlstorm.cpp$(PreprocessSuffix): ../../dlstorm/dlstorm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlstorm_dlstorm.cpp$(PreprocessSuffix) "../../dlstorm/dlstorm.cpp"

$(IntermediateDirectory)/dlstorm_md5.cpp$(ObjectSuffix): ../../dlstorm/md5.cpp $(IntermediateDirectory)/dlstorm_md5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sparson/dev/dlstorm/md5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dlstorm_md5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlstorm_md5.cpp$(DependSuffix): ../../dlstorm/md5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlstorm_md5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/dlstorm_md5.cpp$(DependSuffix) -MM "../../dlstorm/md5.cpp"

$(IntermediateDirectory)/dlstorm_md5.cpp$(PreprocessSuffix): ../../dlstorm/md5.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlstorm_md5.cpp$(PreprocessSuffix) "../../dlstorm/md5.cpp"

$(IntermediateDirectory)/dlstorm_c_data.cpp$(ObjectSuffix): ../../dlstorm/c_data.cpp $(IntermediateDirectory)/dlstorm_c_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sparson/dev/dlstorm/c_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dlstorm_c_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlstorm_c_data.cpp$(DependSuffix): ../../dlstorm/c_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlstorm_c_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/dlstorm_c_data.cpp$(DependSuffix) -MM "../../dlstorm/c_data.cpp"

$(IntermediateDirectory)/dlstorm_c_data.cpp$(PreprocessSuffix): ../../dlstorm/c_data.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlstorm_c_data.cpp$(PreprocessSuffix) "../../dlstorm/c_data.cpp"

$(IntermediateDirectory)/dlstorm_c_log.cpp$(ObjectSuffix): ../../dlstorm/c_log.cpp $(IntermediateDirectory)/dlstorm_c_log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sparson/dev/dlstorm/c_log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dlstorm_c_log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlstorm_c_log.cpp$(DependSuffix): ../../dlstorm/c_log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlstorm_c_log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/dlstorm_c_log.cpp$(DependSuffix) -MM "../../dlstorm/c_log.cpp"

$(IntermediateDirectory)/dlstorm_c_log.cpp$(PreprocessSuffix): ../../dlstorm/c_log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlstorm_c_log.cpp$(PreprocessSuffix) "../../dlstorm/c_log.cpp"

$(IntermediateDirectory)/mantra_mantra.cpp$(ObjectSuffix): ../../mantra/mantra.cpp $(IntermediateDirectory)/mantra_mantra.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sparson/dev/mantra/mantra.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mantra_mantra.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mantra_mantra.cpp$(DependSuffix): ../../mantra/mantra.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mantra_mantra.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mantra_mantra.cpp$(DependSuffix) -MM "../../mantra/mantra.cpp"

$(IntermediateDirectory)/mantra_mantra.cpp$(PreprocessSuffix): ../../mantra/mantra.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mantra_mantra.cpp$(PreprocessSuffix) "../../mantra/mantra.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


