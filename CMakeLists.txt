cmake_minimum_required(VERSION 3.8)
project(viper)

include_directories(./include)

set(CMAKE_LIBRARY_FLAGS "")

if(MSVC)
  set(CMAKE_LIBRARY_FLAGS "/LTCG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /bigobj /std:c++17 /Wv:18")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_WIN32_WINNT=0x0501)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(/experimental:external)
  add_definitions(/external:W0)
  add_definitions(/external:anglebrackets)
endif()
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_RELEASE} -O3 -DNDEBUG")
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_RELEASE} -pthreads")
endif()
if(NOT EXISTS build/debug)
  file(MAKE_DIRECTORY build/debug)
endif()
if(NOT EXISTS build/release)
  file(MAKE_DIRECTORY build/release)
endif()
if(NOT EXISTS tests/debug)
  file(MAKE_DIRECTORY tests/debug)
endif()
if(NOT EXISTS tests/release)
  file(MAKE_DIRECTORY tests/release)
endif()

file(GLOB header_files ./include/viper/*.hpp)
file(GLOB source_files ./source/*.cpp)
file(GLOB data_types_header_files ./include/viper/data_types/*.hpp)
file(GLOB sqlite_header_files ./include/viper/sqlite/*.hpp)

add_library(viper ${header_files} ${data_types_header_files}
  ${sqlite_header_files} ${source_files})
set_source_files_properties(${header_files} PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(${sqlite_header_files} PROPERTIES
  HEADER_FILE_ONLY TRUE)
set_source_files_properties(${data_types_header_files} PROPERTIES
  HEADER_FILE_ONLY TRUE)
source_group("Header Files\\data_types" FILES ${data_types_header_files})
source_group("Header Files\\sqlite" FILES ${sqlite_header_files})
set_target_properties(viper PROPERTIES
  STATIC_LIBRARY_FLAGS_RELEASE "${CMAKE_LIBRARY_FLAGS}" OUTPUT_NAME viper)

install(TARGETS viper CONFIGURATIONS Debug
  DESTINATION build/debug)
install(TARGETS viper CONFIGURATIONS Release RelWithDebInfo
  DESTINATION build/release)
