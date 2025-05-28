cmake_minimum_required(VERSION 3.23)

project(game VERSION 0.0.1 LANGUAGES C)

set(GAME_COMPILE_FLAGS -Wall -Wextra -Wpedantic)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND GAME_COMPILE_FLAGS -O0 -g3 -fsanitize=address)
else()
  list(APPEND GAME_COMPILE_FLAGS -O3)
endif()

set(GAME_LIB_SOURCES src/game.c src/renderer.c)
set(GAME_LIB_TARGET_NAME gamelib)

if(APPLE)
  enable_language(OBJC)
  list(APPEND GAME_COMPILE_FLAGS -xobjective-c)
  # list(APPEND SOURCES src/platforms/macos.m)
endif()

add_library(
  ${GAME_LIB_TARGET_NAME}
  STATIC
  ${GAME_LIB_SOURCES}
)
target_compile_options(${GAME_LIB_TARGET_NAME} PRIVATE ${GAME_COMPILE_FLAGS})
if(NOT APPLE)
  target_precompile_headers(${GAME_LIB_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h")
endif()
target_include_directories(
  ${GAME_LIB_TARGET_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/zap/include
)
if(WIN32)
  target_compile_definitions(${GAME_LIB_TARGET_NAME} PRIVATE PLATFORM_WINDOWS)
  target_link_libraries(
    ${GAME_LIB_TARGET_NAME}
    PRIVATE
    user32.lib
    d3d11.lib
    dxgi.lib
    d3dcompiler.lib
  )
elseif(APPLE)
  target_compile_definitions(${GAME_LIB_TARGET_NAME} PRIVATE PLATFORM_MACOS)
  target_link_libraries(
    ${GAME_LIB_TARGET_NAME}
    PRIVATE
    "-framework Cocoa"
    "-framework Foundation"
  )
elseif(LINUX)
  target_compile_definitions(${GAME_LIB_TARGET_NAME} PRIVATE PLATFORM_X11)
  target_link_libraries(
    ${GAME_LIB_TARGET_NAME}
    PRIVATE
    "X11"
    "Xrandr"
    "GL"
  )
endif()

set(GAME_EXE_SOURCES src/targets/main.c)
set(GAME_EXE_TARGET_NAME game)
add_executable(
  ${GAME_EXE_TARGET_NAME}
  ${GAME_EXE_SOURCES}
)
if(NOT APPLE)
  target_precompile_headers(${GAME_EXE_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h")
endif()
target_compile_options(${GAME_EXE_TARGET_NAME} PRIVATE ${GAME_COMPILE_FLAGS})
if(WIN32)
  target_compile_definitions(${GAME_EXE_TARGET_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
  target_compile_definitions(${GAME_EXE_TARGET_NAME} PRIVATE PLATFORM_MACOS)
elseif(LINUX)
  target_compile_definitions(${GAME_EXE_TARGET_NAME} PRIVATE PLATFORM_X11)
endif()
target_include_directories(
  ${GAME_EXE_TARGET_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/zap/include
)
target_link_libraries(
  ${GAME_EXE_TARGET_NAME}
  PRIVATE
  ${GAME_LIB_TARGET_NAME}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_options(${GAME_LIB_TARGET_NAME} PRIVATE -fsanitize=address)
  target_link_options(${GAME_EXE_TARGET_NAME} PRIVATE -fsanitize=address)

  if (WIN32)
    execute_process(
      COMMAND ${CMAKE_C_COMPILER} --print-resource-dir
      OUTPUT_VARIABLE CLANG_RESOURCE_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(ASAN_DLL "${CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic-x86_64.dll")
    add_custom_command(TARGET ${GAME_LIB_TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${ASAN_DLL}"
              "$<TARGET_FILE_DIR:game>"
      COMMENT "Copying ASAN runtime DLL"
    )
  endif()
endif()
