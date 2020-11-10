project(render)

# The runtime binary
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE types)

# The compile time binary
set(PROJECT_CT_NAME "${PROJECT_NAME}_ct")
add_executable(${PROJECT_CT_NAME} EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(${PROJECT_CT_NAME} PRIVATE types)
target_compile_definitions(${PROJECT_CT_NAME} PRIVATE RENDER_COMPILE_TIME)

add_subdirectory(types)