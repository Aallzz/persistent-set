cmake_minimum_required(VERSION 2.8)

project(persistent_set)
add_executable(${PROJECT_NAME} main.cpp persistent_set.h smart_shared_pointer.h smart_linked_pointer.h)

#set (gtest_disable_pthreads on)
add_subdirectory (googletest)
#config_compiler_and_linker()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -Wformat=2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -Wfloat-equal -Wconversion -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries (${PROJECT_NAME} gtest gmock)
