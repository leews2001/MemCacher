

enable_testing()


###google test
if(1)

    # Set the desired output directory for the build executables
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

    message("Google Test")
    include(FetchContent)
    # Download and build Google Test
    FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)

    message("PROJECT_NAME = " ${PROJECT_NAME})
    message("CMAKE_RUNTIME_OUTPUT_DIRECTORY = " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    message("CMAKE_BINARY_DIR = " ${CMAKE_BINARY_DIR})
    message("PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR})
    message("googletest_SOURCE_DIR = " ${googletest_SOURCE_DIR})
    
    include_directories(${googletest_SOURCE_DIR}/googletest/include)
  #  set(GTEST_LIBRARY ${googletest_BINARY_DIR}/lib/libgtest.a)
  #  set(GTEST_INCLUDE_DIR ${googletest_SOURCE_DIR}/googletest/include)
  #  set(GTEST_MAIN_LIBRARY ${googletest_BINARY_DIR}/lib/libgtest_main.a)

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(gmock_force_shared_crt ON CACHE BOOL "" FORCE)

    add_executable(
    ${PROJECT_NAME}_Tests
    tests/main.cpp
    )

    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
    ./
    ./incl
    ./tests
    ${googletest_SOURCE_DIR}/googletest/include
    )

 

    # Link the Google Test and Google Mock libraries to your test target
    target_link_libraries(${PROJECT_NAME}_Tests gtest_main gmock_main)

    target_link_libraries(
    ${PROJECT_NAME}_Tests
    ${PROJECT_NAME}
    )

    include(GoogleTest)

    # Note: in linux gtest seems to generate cmake files with quotes.
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()
