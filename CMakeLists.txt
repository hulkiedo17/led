cmake_minimum_required(VERSION 3.0)

project(led
	VERSION 0.3
	DESCRIPTION "Led - line-oriented text editor"
	LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_C_FLAGS_DEBUG "-std=c11 -Wall -Wextra -Werror -Wpedantic -Wformat=2 -Wshadow -Wunused-parameter -Wwrite-strings -Wstrict-prototypes -Wold-style-definition -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wlogical-op -g -O0")
		set(CMAKE_C_FLAGS_RELEASE "-std=c11 -O2")
	endif()
endif()

set(SOURCE src/append.c src/buffer.c src/buffer.c src/delete.c src/file.c src/insert.c src/io.c src/misc.c src/parser.c src/prompt.c src/main.c src/execute.c)
set(HEADER include/append.h include/buffer.h include/delete.h include/file.h include/insert.h include/io.h include/misc.h include/parser.h include/prompt.h include/main.h include/execute.h)
set(EXECUTABLE led)

add_executable(${EXECUTABLE} ${HEADER} ${SOURCE})
