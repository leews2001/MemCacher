# CMakeList.txt : CMake project for main.cpp to test MemCacherLib
#


set(TEST_EXEC_NAME "cacher")

# Add source to this project's executable.
add_executable (${TEST_EXEC_NAME} "cacher.cpp"  "../incl/MemCacher.h")

#include directories
 target_link_libraries(${TEST_EXEC_NAME} ${PROJECT_NAME} )

 # local.cmake
message("PROJECT_NAME = " ${PROJECT_NAME})
message("TEST_EXEC_NAME = " ${TEST_EXEC_NAME})
message("CMAKE_SOURCE_DIR =" ${CMAKE_SOURCE_DIR})
message("CMAKE_INSTALL_INCLUDEDIR= " ${CMAKE_INSTALL_INCLUDEDIR})
message("CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${TEST_EXEC_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # Relative install include path
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/incl> #put this under PUBLIC for library
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${TEST_EXEC_NAME} PROPERTY CXX_STANDARD 17)
endif()

 