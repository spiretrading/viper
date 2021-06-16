cmake_minimum_required(VERSION 3.8)
project(viper)

include_directories(./Include)

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
  add_definitions(/external:anglebrackets)
  add_definitions(/external:W0)
endif()
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_RELEASE} -O3 -DNDEBUG")
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_RELEASE} -pthreads")
endif()

file(GLOB data_types_header_files ./Include/Viper/DataTypes/*.hpp)
source_group("Data Types" FILES ${data_types_header_files})
file(GLOB expressions_header_files ./Include/Viper/Expressions/*.hpp)
source_group("Expressions" FILES ${expressions_header_files})
file(GLOB mysql_header_files ./Include/Viper/MySql/*.hpp)
source_group("MySQL" FILES ${mysql_header_files})
file(GLOB sqlite_header_files ./Include/Viper/Sqlite3/*.hpp)
source_group("Sqlite3" FILES ${sqlite_header_files})
file(GLOB viper_header_files ./Include/Viper/*.hpp)
source_group("Viper" FILES ${viper_header_files})

file(GLOB header_files ${data_types_header_files} ${expressions_header_files}
  ${mysql_header_files} ${sqlite_header_files} ${viper_header_files})

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_library(viper ${header_files})
set_source_files_properties(${header_files} PROPERTIES HEADER_FILE_ONLY TRUE)
set_target_properties(viper PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE
  "${CMAKE_LIBRARY_FLAGS}" LINKER_LANGUAGE CXX OUTPUT_NAME viper)
