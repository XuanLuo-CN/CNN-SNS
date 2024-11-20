cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set (CMAKE_C_COMPILER 		"/public/software/mpi/mpich2-1.5-intel/bin/mpicc")
set (CMAKE_CXX_COMPILER 		"/public/software/compiler/gnu/7.2.0/bin/g++")

project(NN_test)
set(CMAKE_BUILD_TYPE Debug)

set (TOP_PATH /public/home/graduate/sFlow-LUO2)
set (Torch_DIR ${TOP_PATH}/lib/libtorch-1.8.0+cu111/share/cmake/Torch)
# set (Torch_DIR ${TOP_PATH}/lib/libtorch/share/cmake/Torch)

set(CMAKE_CUDA_ARCHITECTURES "75")

set(CUDNN_LIBRARY_PATH ${TOP_PATH}/lib/cudnn-linux-x86_64-8.8.1.3_cuda11-archive/lib/libcudnn.so)
set(CUDNN_INCLUDE_PATH ${TOP_PATH}/lib/cudnn-linux-x86_64-8.8.1.3_cuda11-archive/include)

set(Adv_LIBRARY_PATH /public/software/ADVENTURE/lib)
set(Adv_INCLUDE_PATH /public/software/ADVENTURE/include)

set(MPI_LIBRARY_PATH /public/software/mpi/mpich2-1.5-intel/lib)
set(MPI_INCLUDE_PATH /public/software/mpi/mpich2-1.5-intel/include)

include_directories(${Adv_INCLUDE_PATH})
link_directories(${Adv_LIBRARY_PATH})
include_directories(${MPI_INCLUDE_PATH})
link_directories(${MPI_LIBRARY_PATH})

add_library(AdvBase STATIC IMPORTED GLOBAL)
set_property(TARGET AdvBase 
            PROPERTY IMPORTED_LOCATION /public/software/ADVENTURE/lib/libAdvBase.a)

add_library(AdvDocIO STATIC IMPORTED GLOBAL)
set_property(TARGET AdvDocIO 
            PROPERTY IMPORTED_LOCATION /public/software/ADVENTURE/lib/libAdvDocIO.a)
set_property(TARGET AdvDocIO 
            PROPERTY INTERFACE_LINK_LIBRARIES AdvBase)

# find_package(Python REQUIRED COMPONENTS Interpreter Development) # for the bug "Failed to compute shorthash for libnvrtc.so" in cmake
find_package(Torch REQUIRED)

aux_source_directory(/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3 SOURCE)
aux_source_directory(/public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/sFlow_source SFLOW_SOURCE)
add_executable(NN_test ${SOURCE} ${SFLOW_SOURCE})

# target_link_libraries(AdvDocIO AdvBase)
target_link_libraries(NN_test 
                        "${TORCH_LIBRARIES}" 
                        AdvBase AdvIDL AdvDocIO AdvFileIO 
                        mpich
                     )
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD 11)
set_property(TARGET NN_test PROPERTY CXX_STANDARD 14)