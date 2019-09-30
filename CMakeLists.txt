cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

huntergate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
        SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)


project(lab-01-pasrer)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

# TODO: change template word to project name and delete this comment
add_library(lab-01-parser STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_json.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/json.h
        sources/my_json.cpp)
add_executable(tests
        # TODO: enum your files and delete this comment
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp
        )
add_executable(prices
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/json.cpp
        )

# TODO: change template word to project name and delete this comment
target_include_directories(lab-01-parser
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
            nlohmann_json::nlohmann_json
        )
target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
            nlohmann_json::nlohmann_json
        )
target_include_directories(prices
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
            nlohmann_json::nlohmann_json
        )
# TODO: change template word to project name and delete this comment
find_library(
        GTEST_LOCAL_LIB
        NAMES gtest
        PATHS ${PROJECT_NAME}/lib64/
)
find_library(
        GTEST_LOCAL_LIB_MAIN
        NAMES gtest_main
        PATHS ${PROJECT_NAME}/lib64/
)
target_link_libraries(
        tests
        ${CMAKE_THREAD_LIBS_INIT}
        ${GTEST_LOCAL_LIB}
        ${GTEST_LOCAL_LIB_MAIN}
        lab-01-parser nlohmann_json::nlohmann_json
)
target_link_libraries(prices
        PUBLIC
        nlohmann_json::nlohmann_json
        )

enable_testing()
add_test(NAME unit_tests COMMAND tests)

if(BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(codecov)
    # TODO: change template word to project name and delete this comment
    add_coverage(lab-01-parser)
    add_coverage(tests)
    list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
    coverage_evaluate()
endif()
