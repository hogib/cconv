cmake_minimum_required(VERSION 3.15)

project(cconv LANGUAGES C)

add_executable(cconv
		src/main.c
		src/basic_operators.c
)

target_include_directories(cconv PRIVATE include)
