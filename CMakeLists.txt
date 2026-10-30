cmake_minimum_required(VERSION 3.12)
project(SmartCanteenSimpleDSA LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(headers)

add_executable(SmartCanteen
    src/main_program.cpp
    src/food_menu_management.cpp
    src/token_lookup.cpp
    src/order_processing.cpp
)

add_executable(SmartCanteenTests
    tests/testing.cpp
    src/food_menu_management.cpp
    src/token_lookup.cpp
)

enable_testing()
add_test(NAME BasicDSATests COMMAND SmartCanteenTests)
