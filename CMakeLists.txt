cmake_minimum_required(VERSION 3.0)

project(led
	VERSION 0.3
	DESCRIPTION "led - line-oriented text editor"
	LANGUAGES C)

if(NOT UNIX)
	message(FATAL_ERROR "Your system is not unix. Fail to build.")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O2 -DNDEBUG")

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -std=c99 -Wall -Wextra -g -O0 -Wpedantic -Wshadow -Wvla -Wpointer-arith -Wwrite-strings -Wfloat-equal -Wcast-align -Wcast-qual -Wbad-function-cast -Wstrict-overflow=4 -Wunreachable-code -Wformat=2 -Wundef -Wstrict-prototypes -Wold-style-definition -Wconversion -Wshift-overflow=2 -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wlogical-op -Wcast-align=strict -Wduplicated-cond -Wjump-misses-init -Wstringop-overflow")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -std=c99 -Wall -Wextra -g -O0 -Wpedantic -Wshadow -Wvla -Wpointer-arith -Wwrite-strings -Wfloat-equal -Wcast-align -Wcast-qual -Wbad-function-cast -Wstrict-overflow=4 -Wunreachable-code -Wformat=2 -Wundef -Wstrict-prototypes -Wold-style-definition -Wconversion -Wshift-overflow -Wredundant-decls -Wnested-externs -Wmissing-include-dirs")
else()
	message(FATAL_ERROR "Clang or GCC is not installed on your system. Fail to build.")
endif()

set(EXECUTABLE led)
set(INSTALL_DIR bin)
set(INCLUDE_DIR include)
set(SOURCE
	src/append.c
	src/buffer.c
	src/buffer.c
	src/delete.c
	src/file.c
	src/insert.c
	src/io.c
	src/misc.c
	src/parser.c
	src/prompt.c
	src/main.c
	src/execute.c)

add_executable(${EXECUTABLE} ${SOURCE})

target_include_directories(${EXECUTABLE} PRIVATE ${INCLUDE_DIR})

install(TARGETS ${EXECUTABLE} DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR})

