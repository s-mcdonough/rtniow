# Download catch2 via FetchContent
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.0 )

# This line configures/builds the item specifed in _Declare and is done at
# configuration time. Targets in the Catch2 namespace are now avaible for use.
FetchContent_MakeAvailable(catch2)

# A slightly heavy handed approach to add a specific script into our cmake
# project. Used for catch_discover_tests()
include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)

# Our unit test executable
project(ut)
add_executable(${PROJECT_NAME})

# Require c++20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED TRUE)

target_sources(${PROJECT_NAME} PRIVATE 
    main.cpp
    types/vec.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Catch2::Catch2
    types)

# Links catch tests into CTest
catch_discover_tests(${PROJECT_NAME})