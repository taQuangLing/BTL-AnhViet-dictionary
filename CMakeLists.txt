project(demo_jrb
        LANGUAGES C)

cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")

add_library(pre_fix STATIC pre_fix/llist.c pre_fix/prefix.c)
add_library(sqlite3 STATIC sqlite/sqlite3.c)
include_directories(sqlite)
add_subdirectory(libfdr)
add_executable(main main.c)
target_link_libraries(main ${GTK3_LIBRARIES})
target_link_libraries(main pre_fix fdr)
foreach (example main)
  target_link_libraries(${example} sqlite3 pthread dl)
endforeach(example)
