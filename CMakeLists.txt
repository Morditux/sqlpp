cmake_minimum_required(VERSION 3.10)
project(sqlpp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find system SQLite3
find_package(SQLite3)

if(SQLite3_FOUND)
    message(STATUS "Using system SQLite3")
    set(SQLITE_LIBRARIES SQLite3::SQLite3)
    set(SQLITE_INCLUDE_DIRS "") # Already handled by SQLite3::SQLite3 target
else()
    message(STATUS "System SQLite3 not found, using bundled version")
    add_library(sqlite3_bundled OBJECT sqlite/sqlite3.c sqlite/sqlite3.h)
    target_include_directories(sqlite3_bundled PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sqlite)
    set(SQLITE_LIBRARIES sqlite3_bundled)
    set(SQLITE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/sqlite)
endif()

# Main library
set(LIB_SRCS
    Blob.cpp
    Cursor.cpp
    Database.cpp
    PreparedStatement.cpp
    SQLiteException.cpp
)

add_library(sqlitepp SHARED ${LIB_SRCS})

target_include_directories(sqlitepp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SQLITE_INCLUDE_DIRS})
target_link_libraries(sqlitepp PRIVATE ${SQLITE_LIBRARIES})

# Example executable
add_executable(example example.cpp)
target_link_libraries(example PRIVATE sqlitepp)
