cmake_minimum_required(VERSION 3.5)
project(gst_rebooter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -pthread")
add_definitions(-std=c++14)

find_package(eCAL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads)

#设置输出路径
SET(MESSAGE_DIR ${PROJECT_SOURCE_DIR}/message)
if(EXISTS "${MESSAGE_DIR}" AND IS_DIRECTORY "${MESSAGE_DIR}")
        SET(PROTO_META_BASE_DIR ${MESSAGE_DIR})
else()
        file(MAKE_DIRECTORY ${MESSAGE_DIR})
        SET(PROTO_META_BASE_DIR ${MESSAGE_DIR})
endif()

# set global value
set_property(GLOBAL PROPERTY SMILES_MOWER_PROTOBUF_DIR ${MESSAGE_DIR})

#设置protoc的搜索路径
LIST(APPEND PROTO_FLAGS -I${PROJECT_SOURCE_DIR}/protobuf)
#获取需要编译的proto文件
file(GLOB_RECURSE MSG_PROTOS ${PROJECT_SOURCE_DIR}/protobuf/*.proto)

set(MESSAGE_SRC "")
set(MESSAGE_HDRS "")

foreach(msg ${MSG_PROTOS})
    file(RELATIVE_PATH REL_FIL ${PROJECT_SOURCE_DIR}/protobuf ${msg})
    get_filename_component(FIL_WE ${msg} NAME_WE)
    get_filename_component(DIR ${REL_FIL} DIRECTORY)

    list(APPEND MESSAGE_SRC "${MESSAGE_DIR}/${DIR}/${FIL_WE}.pb.cc")
    list(APPEND MESSAGE_HDRS "${MESSAGE_DIR}/${DIR}/${FIL_WE}.pb.h")

	execute_process(
        COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${msg}
      )
endforeach()

# 设置文件属性为 GENERATED
set_source_files_properties(${MESSAGE_SRC} ${MESSAGE_HDRS} PROPERTIES GENERATED TRUE)

include_directories(${MESSAGE_DIR})
add_library(intelligent_drive_protobuf_lib ${MESSAGE_SRC} ${MESSAGE_HDRS})

include_directories("${PROJECT_SOURCE_DIR}/include")
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

add_executable(gst_rebooter ${DIR_SRCS})
target_link_libraries(gst_rebooter 
                        ${OpenCV_LIBRARIES} 
                        ${CMAKE_THREAD_LIBS_INIT}
                        dl
                        eCAL::core 
                        protobuf::libprotobuf
                        intelligent_drive_protobuf_lib
                        )

add_executable(send_cmd test/send_cmd.cpp src/loguru.cpp)
target_link_libraries(send_cmd 
                        ${OpenCV_LIBRARIES} 
                        ${CMAKE_THREAD_LIBS_INIT}
                        dl
                        eCAL::core 
                        protobuf::libprotobuf
                        intelligent_drive_protobuf_lib
                        )