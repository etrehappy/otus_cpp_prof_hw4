cmake_minimum_required(VERSION 3.26)

project(print_ip_prj)

add_executable(print_ip 
	print_ip.cpp
)

if(MSVC)
		set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT print_ip)		
endif ()

target_compile_options(print_ip PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX >
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

set_target_properties(print_ip PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

