

enable_testing()

 



###google test
if(0)
    include(FetchContent)
    # Download and build Google Test
    FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)


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
    )

    # Link the Google Test and Google Mock libraries to your test target
    target_link_libraries(${PROJECT_NAME}_Tests gtest_main gmock_main)

    target_link_libraries(
    ${PROJECT_NAME}_Tests
    ${PROJECT_NAME}
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()

###QTest#####
if(0)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

    add_executable(${PROJECT_NAME}QTests
        tests/main.cpp
        tests/framework/VectorFQTests.h
        tests/framework/VectorFQTests.cpp
        tests/framework/WayptQTests.h
        tests/framework/WayptQTests.cpp
        tests/framework/SegmentQTests.h
        tests/framework/SegmentQTests.cpp
        tests/framework/PlanQTests.h
        tests/framework/PlanQTests.cpp
        tests/framework/PlanHelperQTests.h
        tests/framework/PlanHelperQTests.cpp
        tests/framework/UblasHelperQTests.h
        tests/framework/UblasHelperQTests.cpp
        tests/framework/EllMapQTests.h
        tests/framework/EllMapQTests.cpp

        tests/framework/algorithm/gjk/PolygonQTests.h
        tests/framework/algorithm/gjk/PolygonQTests.cpp
        )

    target_include_directories(${PROJECT_NAME}QTests PRIVATE
        ./
        ./incl/${PROJECT_NAME}/
        ./incl/${PROJECT_NAME}/framework
        ./incl/${PROJECT_NAME}/framework/algorithm
        ./incl/${PROJECT_NAME}/framework/algorithm/gjk
        ./incl/${PROJECT_NAME}/controllers
        ./incl/${PROJECT_NAME}/models
        ./pimpl/${PROJECT_NAME}/framework
        ./pimpl/${PROJECT_NAME}/framework/algorithm
        ./pimpl/${PROJECT_NAME}/controllers
        ./pimpl/${PROJECT_NAME}/models
        ./tests
        ./tests/framework
        ./tests/framework/algorithm
        ./tests/framework/algorithm/gjk
        ./tests/controllers
        ./tests/models
        ${Boost_INCLUDE_DIRS}
        )


    target_link_libraries(
        ${PROJECT_NAME}QTests
        Qt::Test
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Widgets
        ${PROJECT_NAME}
        )


    add_test(NAME ${PROJECT_NAME}QTests COMMAND ${PROJECT_NAME}QTests)
endif()
