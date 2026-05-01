cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK or pass -DCMAKE_TOOLCHAIN_FILE=/path/to/vita.toolchain.cmake")
  endif()
endif()

project(VitaHUD C)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaHUD")
set(VITA_TITLEID  "VHUD00001")
set(VITA_VERSION  "01.00")

add_definitions(-D__PSVITA__)

add_executable(vitahud
  src/vitahud.c
)

target_compile_options(vitahud PRIVATE -Wall -Wextra -O2)

target_link_libraries(vitahud
  taihen_stub
  SceDisplay_stub
  SceCtrl_stub
  ScePower_stub
  SceRtc_stub
  SceLibKernel_stub
)

vita_create_self(vitahud.suprx vitahud
  CONFIG exports.yml
  UNSAFE
)
