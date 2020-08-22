project(render)
add_executable(${PROJECT_NAME})

# Require c++20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED TRUE)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp)