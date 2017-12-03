cmake_minimum_required(VERSION 2.8)

project(persistent_set)
add_executable(${PROJECT_NAME} main.cpp persistent_set.h smart_shared_pointer.h smart_linked_pointer.h)

#set (gtest_disable_pthreads on)
add_subdirectory (googletest)
#config_compiler_and_linker()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fsanitize=address -fpermissive  -Wall -Wextra -pedantic  -Wformat=2")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG -fpermissive")
endif()

target_link_libraries (${PROJECT_NAME} gtest gmock)
