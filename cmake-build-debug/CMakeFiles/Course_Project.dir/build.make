# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "E:\CLion\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "E:\CLion\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\AKS\Course_Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\AKS\Course_Project\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Course_Project.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Course_Project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Course_Project.dir/flags.make

CMakeFiles/Course_Project.dir/main.cpp.obj: CMakeFiles/Course_Project.dir/flags.make
CMakeFiles/Course_Project.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\AKS\Course_Project\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Course_Project.dir/main.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Course_Project.dir\main.cpp.obj -c E:\AKS\Course_Project\main.cpp

CMakeFiles/Course_Project.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Course_Project.dir/main.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\AKS\Course_Project\main.cpp > CMakeFiles\Course_Project.dir\main.cpp.i

CMakeFiles/Course_Project.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Course_Project.dir/main.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\AKS\Course_Project\main.cpp -o CMakeFiles\Course_Project.dir\main.cpp.s

# Object files for target Course_Project
Course_Project_OBJECTS = \
"CMakeFiles/Course_Project.dir/main.cpp.obj"

# External object files for target Course_Project
Course_Project_EXTERNAL_OBJECTS =

Course_Project.exe: CMakeFiles/Course_Project.dir/main.cpp.obj
Course_Project.exe: CMakeFiles/Course_Project.dir/build.make
Course_Project.exe: CMakeFiles/Course_Project.dir/linklibs.rsp
Course_Project.exe: CMakeFiles/Course_Project.dir/objects1.rsp
Course_Project.exe: CMakeFiles/Course_Project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\AKS\Course_Project\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Course_Project.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Course_Project.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Course_Project.dir/build: Course_Project.exe

.PHONY : CMakeFiles/Course_Project.dir/build

CMakeFiles/Course_Project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Course_Project.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Course_Project.dir/clean

CMakeFiles/Course_Project.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\AKS\Course_Project E:\AKS\Course_Project E:\AKS\Course_Project\cmake-build-debug E:\AKS\Course_Project\cmake-build-debug E:\AKS\Course_Project\cmake-build-debug\CMakeFiles\Course_Project.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Course_Project.dir/depend

