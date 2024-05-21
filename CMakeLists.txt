cmake_minimum_required(VERSION 2.8)
project(sylar)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON) 
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS}  -g -std=c++11 -Wall  -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined ")
# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT /opt/homebrew/Cellar/boost@1.76/1.76.0_5)
set(Boost_NO_SYSTEM_PATHS ON) # 只搜索上语句设置的搜索路径
find_package(Boost COMPONENTS regex system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# find_package(yaml-cpp REQUIRED)
set(YAMLCPP_INCLUDE_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0/include/")
set(YAMLCPP_LIBRARIES "/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib/libyaml-cpp.0.8.0.dylib")
include_directories(${YAMLCPP_INCLUDE_DIR})
find_library(YAMLCPP yaml-cpp)
message(---------------------------)
message(${YAMLCPP})
# message(${Boost_LIBRARIES})
message(-----------------------)
set(LIB_SRC
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
    sylar/thread.cc
    sylar/mutex.cc
    )
    set(LIB_LIB 
        sylar
        pthread
        ${Boost_LIBRARIES}
    )

add_library(sylar SHARED ${LIB_SRC})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${YAMLCPP})
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")

# add_executable(test tests/test.cc)
# add_dependencies(test sylar)
# force_redefine_file_macro_for_sources(sylar) #__FILE__ 重定义该宏为相对路径
# target_link_libraries(test sylar ${Boost_LIBRARIES} ${YAMLCPP})

force_redefine_file_macro_for_sources(test) #__FILE__ 重定义该宏为相对路径
force_redefine_file_macro_for_sources(test_config) #__FILE__ 重定义该宏为相对路径
force_redefine_file_macro_for_sources(sylar) #__FILE__ 重定义该宏为相对路径

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread) #__FILE__ 重定义该宏为相对路径
target_link_libraries(test_thread ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

