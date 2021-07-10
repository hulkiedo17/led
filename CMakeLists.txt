cmake_minimum_required(VERSION 3.0)

project(led
	VERSION 0.3
	DESCRIPTION "Led - line-oriented text editor"
	LANGUAGES C)

option(BUILD_DEBUG "Enable debug flags" ON)

set(SOURCE src/append.c src/buffer.c src/buffer.c 
	src/delete.c src/file.c src/insert.c src/io.c
	src/misc.c src/parser.c src/prompt.c src/main.c)
set(HEADER include/append.h include/buffer.h
	include/delete.h include/file.h include/insert.h 
	include/io.h include/misc.h include/parser.h
	include/prompt.h include/main.h)
set(EXECUTABLE led)

add_executable(${EXECUTABLE} ${HEADER} ${SOURCE})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_features(${EXECUTABLE} PRIVATE c_std_11)

	if(BUILD_DEBUG EQUAL ON)
		target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -Werror -g)
	else()
		target_compile_options(${EXECUTABLE} PRIVATE -O2)
	endif()
endif()

