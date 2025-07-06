cmake_minimum_required(VERSION 3.5)
project(org_chart_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_subdirectory(google/googletest/googletest)

enable_testing()

file(GLOB_RECURSE MAIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp_projects/*.cc"
)

file(GLOB_RECURSE REFINED_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/refined_tests/*_refined.cpp"
)

add_executable(org_chart_refined_tests
    ${MAIN_SOURCES}
    ${REFINED_TEST_SOURCES}
)

target_include_directories(org_chart_refined_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_projects
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_projects/third_party/libbcrypt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_projects/third_party/drogon/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_projects/third_party/jwt-cpp/include
)

target_link_libraries(org_chart_refined_tests PRIVATE
    gtest_main
    drogon
    jwt-cpp
    bcrypt
)

add_test(NAME RunAllRefinedTests COMMAND org_chart_refined_tests)
