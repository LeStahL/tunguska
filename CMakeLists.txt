# Tunguska - 64k Demo by Team210 at Solskogen 2019
# Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.12)

project(tunguska)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CTRL_ALT_TEST_MINIFIER OFF)

# Find the right python interpreter version
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if(MSVC AND ${CTRL_ALT_TEST_MINIFIER})
    find_program(MINIFIER shader_minifier.exe HINTS ${PROJECT_BINARY_DIR})
    add_custom_target(minification
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
#             COMMAND ${MINIFIER} "${PROJECT_SOURCE_DIR}/gfx.frag" -o "${PROJECT_SOURCE_DIR}/gfx.h"  --preserve-all-globals --preserve-externals
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/sfx.h" "${PROJECT_SOURCE_DIR}/sfx.frag" --preserve-externals
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/load.h" "${PROJECT_SOURCE_DIR}/gfx/load.frag" --preserve-all-globals --preserve-externals
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/post.h" "${PROJECT_SOURCE_DIR}/gfx/post.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/logo210.h" "${PROJECT_SOURCE_DIR}/gfx/logo210.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/greet.h" "${PROJECT_SOURCE_DIR}/gfx/greet.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/text.h" "${PROJECT_SOURCE_DIR}/gfx/text.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/logosmall.h" "${PROJECT_SOURCE_DIR}/gfx/logosmall.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/nr4.h" "${PROJECT_SOURCE_DIR}/gfx/nr4.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/qm.h" "${PROJECT_SOURCE_DIR}/gfx/qm.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/trip.h" "${PROJECT_SOURCE_DIR}/gfx/trip.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/surface.h" "${PROJECT_SOURCE_DIR}/gfx/surface.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/hangout.h" "${PROJECT_SOURCE_DIR}/gfx/hangout.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/fourtwenty.h" "${PROJECT_SOURCE_DIR}/gfx/fourtwenty.frag"
#             COMMAND ${MINIFIER} -o "${PROJECT_SOURCE_DIR}/gfx/solskogen.h" "${PROJECT_SOURCE_DIR}/gfx/solskogen.frag"
    )
else()
    add_custom_target(minification
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/minification/
#             COMMAND python shader-compressor.py "${PROJECT_SOURCE_DIR}/gfx.frag" -o "${PROJECT_SOURCE_DIR}/gfx.h"  --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py  -o "${PROJECT_SOURCE_DIR}/sfx.h" "${PROJECT_SOURCE_DIR}/sfx.frag" --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/load.h" "${PROJECT_SOURCE_DIR}/gfx/load.frag" --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/post.h" "${PROJECT_SOURCE_DIR}/gfx/post.frag" --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/logo210.h" "${PROJECT_SOURCE_DIR}/gfx/logo210.frag" --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/decayingfactory.h" "${PROJECT_SOURCE_DIR}/gfx/decayingfactory.frag" --no-minification
        COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/text.h" "${PROJECT_SOURCE_DIR}/gfx/text.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/logosmall.h" "${PROJECT_SOURCE_DIR}/gfx/logosmall.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/nr4.h" "${PROJECT_SOURCE_DIR}/gfx/nr4.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/qm.h" "${PROJECT_SOURCE_DIR}/gfx/qm.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/trip.h" "${PROJECT_SOURCE_DIR}/gfx/trip.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/surface.h" "${PROJECT_SOURCE_DIR}/gfx/surface.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/hangout.h" "${PROJECT_SOURCE_DIR}/gfx/hangout.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/fourtwenty.h" "${PROJECT_SOURCE_DIR}/gfx/fourtwenty.frag" --no-minification
#             COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/gfx/solskogen.h" "${PROJECT_SOURCE_DIR}/gfx/solskogen.frag" --no-minification
    )
endif()

add_custom_target(texture_packing
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/font/
    COMMAND "${Python3_EXECUTABLE}" pack.py
)

add_custom_target(symbolization
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gfx/
    COMMAND "${Python3_EXECUTABLE}" symbolize.py decayingfactory.frag fogforest.frag -o symbols.h
)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:demo /SUBSYSTEM:Windows  /OPT:REF  /OPT:ICF=10 /VERBOSE /NODEFAULTLIB")
endif()

add_executable(tunguska t.c common.h sequence.h sfx.h pal_win32.h pal_linux.h gfx/logo210.h gfx/decayingfactory.h font/font.h gfx/text.h gfx/symbols.h gfx/load.h gfx/post.h)
add_dependencies(tunguska minification)
add_dependencies(tunguska texture_packing)
add_dependencies(tunguska symbolization)

target_include_directories(tunguska SYSTEM PRIVATE "${OPENGL_INCLUDE_DIR}")

target_link_libraries(tunguska OpenGL::GL)

if(MSVC)
    target_compile_options(tunguska PRIVATE /Gy /O1 /fp:fast /GR- /GS- /MT)

    if (MSVC_VERSION GREATER_EQUAL 1910) # VS2017
        target_link_libraries(tunguska Winmm.lib User32.lib ucrt.lib msvcrt.lib vcruntime.lib ucrt.lib)
    else()
        # for debug output: User32.lib ucrt.lib msvcrt.lib
        target_link_libraries(tunguska Winmm.lib User32.lib ucrt.lib msvcrt.lib)
    endif()

    find_program(KKRUNCHY_K7 kkrunchy_k7.exe HINTS "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")
    add_custom_command(TARGET tunguska COMMAND "${KKRUNCHY_K7}" --best "$<TARGET_FILE:tunguska>" --out "$<TARGET_FILE_DIR:tunguska>/tunguskac.exe" POST_BUILD)
else()
    find_package(SDL2 REQUIRED)
    target_include_directories(tunguska SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(tunguska ${SDL2_LIBRARIES})
endif()
