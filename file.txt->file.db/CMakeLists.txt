project(demo-sqlite
        LANGUAGES C)
cmake_minimum_required(VERSION 2.6)
add_library(sqlite3 STATIC ../sqlite/sqlite3.c)
include_directories(sqlite)
add_executable(main main1.c)
foreach (example main)
  target_link_libraries(${example} sqlite3 pthread dl)
endforeach(example)
