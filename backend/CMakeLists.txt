cmake_minimum_required(VERSION 3.10)
project(CrimeIntelligenceBackend)

set(CMAKE_CXX_STANDARD 11)

# Find MySQL
find_package(MySQL REQUIRED)

# Find JSONCPP
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Add executable
add_executable(backend
    src/main.cpp
    src/BTree/BTree.cpp
    src/HashTable/HashTable.cpp
    src/Queue/Queue.cpp
    src/Stack/Stack.cpp
    src/Database/DBManager.cpp
    src/API/Server.cpp
)

# Include directories
include_directories(src)
include_directories(${MYSQL_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})

# Link libraries
target_link_libraries(backend ${MYSQL_LIBRARIES} ${JSONCPP_LIBRARIES} pthread)
