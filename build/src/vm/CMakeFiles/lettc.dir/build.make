# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Projects\C++\Lett

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Projects\C++\Lett\build

# Include any dependencies generated for this target.
include src/vm/CMakeFiles/lettc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/vm/CMakeFiles/lettc.dir/compiler_depend.make

# Include the progress variables for this target.
include src/vm/CMakeFiles/lettc.dir/progress.make

# Include the compile flags for this target's objects.
include src/vm/CMakeFiles/lettc.dir/flags.make

src/vm/CMakeFiles/lettc.dir/codegen:
.PHONY : src/vm/CMakeFiles/lettc.dir/codegen

src/vm/CMakeFiles/lettc.dir/main.cpp.obj: src/vm/CMakeFiles/lettc.dir/flags.make
src/vm/CMakeFiles/lettc.dir/main.cpp.obj: src/vm/CMakeFiles/lettc.dir/includes_CXX.rsp
src/vm/CMakeFiles/lettc.dir/main.cpp.obj: D:/Projects/C++/Lett/src/vm/main.cpp
src/vm/CMakeFiles/lettc.dir/main.cpp.obj: src/vm/CMakeFiles/lettc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Projects\C++\Lett\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/vm/CMakeFiles/lettc.dir/main.cpp.obj"
	cd /d D:\Projects\C++\Lett\build\src\vm && C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/vm/CMakeFiles/lettc.dir/main.cpp.obj -MF CMakeFiles\lettc.dir\main.cpp.obj.d -o CMakeFiles\lettc.dir\main.cpp.obj -c D:\Projects\C++\Lett\src\vm\main.cpp

src/vm/CMakeFiles/lettc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lettc.dir/main.cpp.i"
	cd /d D:\Projects\C++\Lett\build\src\vm && C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Projects\C++\Lett\src\vm\main.cpp > CMakeFiles\lettc.dir\main.cpp.i

src/vm/CMakeFiles/lettc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lettc.dir/main.cpp.s"
	cd /d D:\Projects\C++\Lett\build\src\vm && C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Projects\C++\Lett\src\vm\main.cpp -o CMakeFiles\lettc.dir\main.cpp.s

# Object files for target lettc
lettc_OBJECTS = \
"CMakeFiles/lettc.dir/main.cpp.obj"

# External object files for target lettc
lettc_EXTERNAL_OBJECTS =

bin/lettc.exe: src/vm/CMakeFiles/lettc.dir/main.cpp.obj
bin/lettc.exe: src/vm/CMakeFiles/lettc.dir/build.make
bin/lettc.exe: lib/liblettcomm.a
bin/lettc.exe: src/vm/CMakeFiles/lettc.dir/linkLibs.rsp
bin/lettc.exe: src/vm/CMakeFiles/lettc.dir/objects1.rsp
bin/lettc.exe: src/vm/CMakeFiles/lettc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Projects\C++\Lett\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ..\..\bin\lettc.exe"
	cd /d D:\Projects\C++\Lett\build\src\vm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lettc.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/vm/CMakeFiles/lettc.dir/build: bin/lettc.exe
.PHONY : src/vm/CMakeFiles/lettc.dir/build

src/vm/CMakeFiles/lettc.dir/clean:
	cd /d D:\Projects\C++\Lett\build\src\vm && $(CMAKE_COMMAND) -P CMakeFiles\lettc.dir\cmake_clean.cmake
.PHONY : src/vm/CMakeFiles/lettc.dir/clean

src/vm/CMakeFiles/lettc.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Projects\C++\Lett D:\Projects\C++\Lett\src\vm D:\Projects\C++\Lett\build D:\Projects\C++\Lett\build\src\vm D:\Projects\C++\Lett\build\src\vm\CMakeFiles\lettc.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/vm/CMakeFiles/lettc.dir/depend

