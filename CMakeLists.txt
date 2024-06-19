cmake_minimum_required(VERSION 3.16)
project(slotus)

#set static-library generate path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/lib/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build/lib/Release) 
#set shared-library generate path
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/lib/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_SOURCE_DIR}/build/lib/Release) 

add_subdirectory(third_party/xlog)
add_subdirectory(src/client)
add_subdirectory(src/server)