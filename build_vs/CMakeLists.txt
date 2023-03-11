macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(ZX-ESPectrum-SDL2)

message(STATUS "Generating build files for project ${PROJECT_NAME}")
message(STATUS "CMAKE_SIZEOF_VOID_P: ${CMAKE_SIZEOF_VOID_P}")
message(STATUS "CMAKE_GENERATOR_PLATFORM  ${CMAKE_GENERATOR_PLATFORM}")

set(SDL2_PATH ../sdl2/sdl2-devel-win-2.24.2)
#set(SDL2_IMAGE_PATH ../sdl2/sdl2-image-devel-win-2.6.2)

# print_all_variables()

if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(MY_ARCH x64)
else()
    set(MY_ARCH x32)
endif()

set(MY_BIN_DIR ../../../bin/${MY_ARCH})

# https://stackoverflow.com/a/7750816
# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_BIN_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MY_BIN_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MY_BIN_DIR} )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${MY_BIN_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${MY_BIN_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${MY_BIN_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

add_executable(${PROJECT_NAME}
    ../src/AySound.cpp
    ../src/CaptureBMP.cpp
    ../src/Config.cpp
    ../src/CPU.cpp
    ../src/ESPectrum.cpp
    ../src/FileSNA.cpp
    ../src/FileZ80.cpp
    ../src/Font.cpp
    ../src/MemESP.cpp
    ../src/OSDMain.cpp
    ../src/OSDMenu.cpp
    ../src/Ports.cpp
    ../src/Tape.cpp
    ../src/Video.cpp
    ../src/Z80_JLS.cpp
    ../include/AySound.h
    ../include/CaptureBMP.h
    ../include/Config.h
    ../include/CPU.h
    ../include/ESPectrum.h
    ../include/FileSNA.h
    ../include/FileUtils.h
    ../include/FileZ80.h
    ../include/MemESP.h
    ../include/OSDMain.h
    ../include/Ports.h
    ../include/Tape.h
    ../include/Video.h
    ../include/Z80_JLS/z80.h
    ../include/Z80_JLS/z80operations.h
    ../sdl2/wrapper/esp32_sdl2_wrapper.h
    ../sdl2/wrapper/wrap_ESP32Lib.h
    ../sdl2/wrapper/wrap_ESP32Lib.cpp
    ../sdl2/wrapper/fabgl.h
    ../sdl2/wrapper/fabgl.cpp
    ../sdl2/wrapper/sdmmc_cmd.h
    ../sdl2/wrapper/main.cpp
    ../sdl2/wrapper/wrap_FileUtils.cpp
)

if(MSVC)
    add_definitions(/FI"esp32_sdl2_wrapper.h")
else()
    # GCC or Clang
    add_definitions(-include esp32_sdl2_wrapper.h)
endif()

add_definitions(-DPLATFORM_NAME="sdl" -DPLATFORM_SCREEN_WIDTH=440 -DPLATFORM_SCREEN_HEIGHT=224 -DPLATFORM_MAP_WINDOW_TILES_WIDTH=16 -DPLATFORM_MAP_WINDOW_TILES_HEIGHT=8 -DPLATFORM_INTRO_OPTIONS=3 -DPLATFORM_DEFAULT_CONTROL=2 -DPLATFORM_MODULE_BASED_AUDIO -DPLATFORM_TILE_BASED_RENDERING -DPLATFORM_IMAGE_BASED_TILES -DPLATFORM_IMAGE_SUPPORT -DPLATFORM_SPRITE_SUPPORT -DPLATFORM_COLOR_SUPPORT -DPLATFORM_CURSOR_SUPPORT -DPLATFORM_CURSOR_SHAPE_SUPPORT -DPLATFORM_FADE_SUPPORT -DPLATFORM_LIVE_MAP_SUPPORT -DOPTIMIZED_MAP_RENDERING)

target_include_directories(
	${PROJECT_NAME} PUBLIC ${SDL2_PATH}/include
	# ${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_PATH}/include
	${PROJECT_NAME} PUBLIC ../include
	${PROJECT_NAME} PUBLIC ../sdl2/wrapper
)

target_link_directories(
	${PROJECT_NAME} PUBLIC ${SDL2_PATH}/lib/${MY_ARCH}
	# ${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_PATH}/lib/${MY_ARCH}
)

# target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)

# https://cmake.org/pipermail/cmake/2008-May/021653.html
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_OUTPUT_NAME ${PROJECT_NAME}D)

