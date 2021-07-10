cmake_minimum_required(VERSION 3.0)

project(led
	VERSION 0.3
	DESCRIPTION "Led - line-oriented text editor"
	LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Werror -g -O0")
	set(CMAKE_C_FLAGS_RELEASE "-O2")
endif()

set(SOURCE src/append.c src/buffer.c src/buffer.c 
	src/delete.c src/file.c src/insert.c src/io.c
	src/misc.c src/parser.c src/prompt.c src/main.c)
set(HEADER include/append.h include/buffer.h
	include/delete.h include/file.h include/insert.h 
	include/io.h include/misc.h include/parser.h
	include/prompt.h include/main.h)
set(EXECUTABLE led)

add_executable(${EXECUTABLE} ${HEADER} ${SOURCE})

target_compile_features(${EXECUTABLE} PRIVATE c_std_11)

