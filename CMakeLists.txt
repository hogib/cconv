cmake_minimum_required(VERSION 3.15)

project(cconv LANGUAGES C)

add_executable(cconv
		src/main.c
		src/basic_operators.c
		src/cli_options.c
		src/helpers.c
)

target_include_directories(cconv PRIVATE include)

target_link_libraries(cconv PRIVATE m)

target_compile_options(cconv PRIVATE
		-Wall
		-Wextra
)
set_target_properties(cconv PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
