# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /public/source/cmake-3.27.0-rc5/bin/cmake

# The command to remove a file.
RM = /public/source/cmake-3.27.0-rc5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build

# Include any dependencies generated for this target.
include CMakeFiles/NN_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/NN_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/NN_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NN_test.dir/flags.make

CMakeFiles/NN_test.dir/Search.cpp.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/Search.cpp.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/Search.cpp
CMakeFiles/NN_test.dir/Search.cpp.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NN_test.dir/Search.cpp.o"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/NN_test.dir/Search.cpp.o -MF CMakeFiles/NN_test.dir/Search.cpp.o.d -o CMakeFiles/NN_test.dir/Search.cpp.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/Search.cpp

CMakeFiles/NN_test.dir/Search.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/NN_test.dir/Search.cpp.i"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/Search.cpp > CMakeFiles/NN_test.dir/Search.cpp.i

CMakeFiles/NN_test.dir/Search.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/NN_test.dir/Search.cpp.s"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/Search.cpp -o CMakeFiles/NN_test.dir/Search.cpp.s

CMakeFiles/NN_test.dir/main.c.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/main.c.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/main.c
CMakeFiles/NN_test.dir/main.c.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/NN_test.dir/main.c.o"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NN_test.dir/main.c.o -MF CMakeFiles/NN_test.dir/main.c.o.d -o CMakeFiles/NN_test.dir/main.c.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/main.c

CMakeFiles/NN_test.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NN_test.dir/main.c.i"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/main.c > CMakeFiles/NN_test.dir/main.c.i

CMakeFiles/NN_test.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NN_test.dir/main.c.s"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/main.c -o CMakeFiles/NN_test.dir/main.c.s

CMakeFiles/NN_test.dir/nn_1.cpp.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/nn_1.cpp.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_1.cpp
CMakeFiles/NN_test.dir/nn_1.cpp.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/NN_test.dir/nn_1.cpp.o"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/NN_test.dir/nn_1.cpp.o -MF CMakeFiles/NN_test.dir/nn_1.cpp.o.d -o CMakeFiles/NN_test.dir/nn_1.cpp.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_1.cpp

CMakeFiles/NN_test.dir/nn_1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/NN_test.dir/nn_1.cpp.i"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_1.cpp > CMakeFiles/NN_test.dir/nn_1.cpp.i

CMakeFiles/NN_test.dir/nn_1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/NN_test.dir/nn_1.cpp.s"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_1.cpp -o CMakeFiles/NN_test.dir/nn_1.cpp.s

CMakeFiles/NN_test.dir/nn_init.cpp.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/nn_init.cpp.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_init.cpp
CMakeFiles/NN_test.dir/nn_init.cpp.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/NN_test.dir/nn_init.cpp.o"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/NN_test.dir/nn_init.cpp.o -MF CMakeFiles/NN_test.dir/nn_init.cpp.o.d -o CMakeFiles/NN_test.dir/nn_init.cpp.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_init.cpp

CMakeFiles/NN_test.dir/nn_init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/NN_test.dir/nn_init.cpp.i"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_init.cpp > CMakeFiles/NN_test.dir/nn_init.cpp.i

CMakeFiles/NN_test.dir/nn_init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/NN_test.dir/nn_init.cpp.s"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_init.cpp -o CMakeFiles/NN_test.dir/nn_init.cpp.s

CMakeFiles/NN_test.dir/nn_print.cpp.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/nn_print.cpp.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_print.cpp
CMakeFiles/NN_test.dir/nn_print.cpp.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/NN_test.dir/nn_print.cpp.o"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/NN_test.dir/nn_print.cpp.o -MF CMakeFiles/NN_test.dir/nn_print.cpp.o.d -o CMakeFiles/NN_test.dir/nn_print.cpp.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_print.cpp

CMakeFiles/NN_test.dir/nn_print.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/NN_test.dir/nn_print.cpp.i"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_print.cpp > CMakeFiles/NN_test.dir/nn_print.cpp.i

CMakeFiles/NN_test.dir/nn_print.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/NN_test.dir/nn_print.cpp.s"
	/public/software/compiler/gnu/7.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/nn_print.cpp -o CMakeFiles/NN_test.dir/nn_print.cpp.s

CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/comm_mpi.c
CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o -MF CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o.d -o CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/comm_mpi.c

CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.i"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/comm_mpi.c > CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.i

CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.s"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/comm_mpi.c -o CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.s

CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/get_global_mesh.c
CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o -MF CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o.d -o CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/get_global_mesh.c

CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.i"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/get_global_mesh.c > CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.i

CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.s"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/get_global_mesh.c -o CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.s

CMakeFiles/NN_test.dir/sFlow_source/mem.c.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/sFlow_source/mem.c.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/mem.c
CMakeFiles/NN_test.dir/sFlow_source/mem.c.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/NN_test.dir/sFlow_source/mem.c.o"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NN_test.dir/sFlow_source/mem.c.o -MF CMakeFiles/NN_test.dir/sFlow_source/mem.c.o.d -o CMakeFiles/NN_test.dir/sFlow_source/mem.c.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/mem.c

CMakeFiles/NN_test.dir/sFlow_source/mem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NN_test.dir/sFlow_source/mem.c.i"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/mem.c > CMakeFiles/NN_test.dir/sFlow_source/mem.c.i

CMakeFiles/NN_test.dir/sFlow_source/mem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NN_test.dir/sFlow_source/mem.c.s"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/mem.c -o CMakeFiles/NN_test.dir/sFlow_source/mem.c.s

CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o: CMakeFiles/NN_test.dir/flags.make
CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o: /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/stdfunc.c
CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o: CMakeFiles/NN_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o -MF CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o.d -o CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o -c /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/stdfunc.c

CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.i"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/stdfunc.c > CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.i

CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.s"
	/public/software/mpi/mpich2-1.5-intel/bin/mpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source/stdfunc.c -o CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.s

# Object files for target NN_test
NN_test_OBJECTS = \
"CMakeFiles/NN_test.dir/Search.cpp.o" \
"CMakeFiles/NN_test.dir/main.c.o" \
"CMakeFiles/NN_test.dir/nn_1.cpp.o" \
"CMakeFiles/NN_test.dir/nn_init.cpp.o" \
"CMakeFiles/NN_test.dir/nn_print.cpp.o" \
"CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o" \
"CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o" \
"CMakeFiles/NN_test.dir/sFlow_source/mem.c.o" \
"CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o"

# External object files for target NN_test
NN_test_EXTERNAL_OBJECTS =

NN_test: CMakeFiles/NN_test.dir/Search.cpp.o
NN_test: CMakeFiles/NN_test.dir/main.c.o
NN_test: CMakeFiles/NN_test.dir/nn_1.cpp.o
NN_test: CMakeFiles/NN_test.dir/nn_init.cpp.o
NN_test: CMakeFiles/NN_test.dir/nn_print.cpp.o
NN_test: CMakeFiles/NN_test.dir/sFlow_source/comm_mpi.c.o
NN_test: CMakeFiles/NN_test.dir/sFlow_source/get_global_mesh.c.o
NN_test: CMakeFiles/NN_test.dir/sFlow_source/mem.c.o
NN_test: CMakeFiles/NN_test.dir/sFlow_source/stdfunc.c.o
NN_test: CMakeFiles/NN_test.dir/build.make
NN_test: /public/home/graduate/sFlow-LUO2/lib/libtorch-1.8.0+cu111/lib/libtorch.so
NN_test: /public/home/graduate/sFlow-LUO2/lib/libtorch-1.8.0+cu111/lib/libc10.so
NN_test: /usr/local/cuda/lib64/stubs/libcuda.so
NN_test: /usr/local/cuda/lib64/libnvrtc.so
NN_test: /usr/local/cuda/lib64/libnvToolsExt.so
NN_test: /usr/local/cuda/lib64/libcudart.so
NN_test: /public/home/graduate/sFlow-LUO2/lib/libtorch-1.8.0+cu111/lib/libc10_cuda.so
NN_test: /public/software/ADVENTURE/lib/libAdvBase.a
NN_test: /public/software/ADVENTURE/lib/libAdvDocIO.a
NN_test: /public/home/graduate/sFlow-LUO2/lib/libtorch-1.8.0+cu111/lib/libc10_cuda.so
NN_test: /public/home/graduate/sFlow-LUO2/lib/libtorch-1.8.0+cu111/lib/libc10.so
NN_test: /usr/local/cuda/lib64/libcufft.so
NN_test: /usr/local/cuda/lib64/libcurand.so
NN_test: /usr/local/cuda/lib64/libcublas.so
NN_test: /public/home/graduate/sFlow-LUO2/lib/cudnn-linux-x86_64-8.8.1.3_cuda11-archive/lib/libcudnn.so
NN_test: /usr/local/cuda/lib64/libnvToolsExt.so
NN_test: /usr/local/cuda/lib64/libcudart.so
NN_test: /public/software/ADVENTURE/lib/libAdvBase.a
NN_test: CMakeFiles/NN_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable NN_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NN_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NN_test.dir/build: NN_test
.PHONY : CMakeFiles/NN_test.dir/build

CMakeFiles/NN_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NN_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NN_test.dir/clean

CMakeFiles/NN_test.dir/depend:
	cd /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3 /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3 /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/CMakeFiles/NN_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/NN_test.dir/depend

