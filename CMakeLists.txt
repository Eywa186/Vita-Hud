cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vitahud C)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O2")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles")

add_executable(vitahud
  src/vitahud.c
)

target_link_libraries(vitahud
  SceLibKernel_stub
  SceKernelThreadMgr_stub
  SceCtrl_stub
  ScePower_stub
  SceRtc_stub
  SceDisplay_stub
  SceNetCtl_stub
)

vita_create_self(vitahud.suprx vitahud
  CONFIG exports.yml
  UNSAFE
)
