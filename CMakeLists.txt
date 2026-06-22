cmake_minimum_required(VERSION 3.24)

project(bad_zip VERSION 0.1.0 LANGUAGES CXX)

add_library(bad_zip_lib)

target_sources(bad_zip_lib PRIVATE
        src/ArgParse.cpp
        src/engine/BZipEngine.cpp
        src/logging/Logger.cpp
)

target_include_directories(bad_zip_lib PUBLIC
        include
)

target_sources(bad_zip_lib PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/bad_zip/ArgParse.hpp
        include/bad_zip/engine/BZipEngine.hpp
        include/bad_zip/logging/Logger.hpp
)

target_compile_features(bad_zip_lib PUBLIC cxx_std_20)

target_compile_options(bad_zip_lib PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(bad_zip
        src/main.cpp
)

target_compile_options(bad_zip PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

target_link_libraries(bad_zip PRIVATE
        bad_zip_lib
)

enable_testing()

find_package(Catch2 3 REQUIRED)

add_executable(bad_zip_tests
        tests/ArgParseTests.cpp
        tests/logging/LoggerTests.cpp
)

target_compile_options(bad_zip_tests PRIVATE
        -Wall
        -Wextra
)

target_link_libraries(bad_zip_tests PRIVATE
        bad_zip_lib
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(bad_zip_tests)