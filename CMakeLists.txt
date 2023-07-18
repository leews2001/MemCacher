# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.16)
project(MemCacherLib VERSION 0.0.0 LANGUAGES CXX)

message("CMAKE_VERSION = " ${CMAKE_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
message("CMAKE_MODULE_PATH (cmake script are located here): " ${CMAKE_MODULE_PATH})

message("CMAKE_CXX_COMPILER_ID = " ${CMAKE_CXX_COMPILER_ID})
message("CMAKE_CXX_COMPILER_VERSION = " ${CMAKE_CXX_COMPILER_VERSION})
message("CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX})
message("CMAKE_CURRENT_BINARY_DIR= " ${CMAKE_CURRENT_BINARY_DIR})


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wno-c++98-compat)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-deprecated-declarations)
endif()

#############################
# find dependencies
if(0)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network Widgets)
    message("QT_VERSION_MAJOR = " ${QT_VERSION_MAJOR})


    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost REQUIRED)
    message("Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS} )
endif()

#############################
# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
if(0)
    if(${QT_VERSION_MAJOR} STRLESS 6)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_AUTOUIC ON)
    else()
        qt_standard_project_setup()
    endif()
    include(GNUInstallDirs)
endif()


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


#############################
#add project files to our exe/lib
add_library(${PROJECT_NAME} 
 incl/MemCacher.h
 src/MemCacher.cpp
 "src/FileDB.cpp" "incl/FileDB.h" "incl/my_utils.h" "src/my_utils.cpp")

# Include sub-projects.
add_subdirectory ("example")

#############################
#include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # Relative install include path
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/incl> #put this under PUBLIC for library
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

#############################
#Preprocess definitions
target_compile_definitions(${PROJECT_NAME}
    PRIVATE 
)


#############################
#link dependencies
if(0)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Widgets
    )
 
endif()

#############################
#installation
include(install) #run install.cmake located in CMAKE_MODULE_PATH (set on top)


 
#############################
#Deploy qt libs
#if(${QT_VERSION_MAJOR} STRLESS 6)
#    # Include Qt deployment helper function
#    include(windeployqt)
#    windeployqt(${PROJECT_NAME})
#else()
#    qt_generate_deploy_app_script(
#        TARGET ${PROJECT_NAME}
#        FILENAME_VARIABLE deploy_script
#        NO_UNSUPPORTED_PLATFORM_ERROR
#    )
#    install(SCRIPT ${deploy_script})
#endif()
message("CMAKE_INSTALL_INCLUDEDIR= " ${CMAKE_INSTALL_INCLUDEDIR})
message("CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})


#############################
message("BUILD_TESTING = " ${BUILD_TESTING})
if(BUILD_TESTING)
    include(buildtest)
endif()
 