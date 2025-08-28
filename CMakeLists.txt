cmake_minimum_required(VERSION 3.16)

project(DroneTelemSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt's meta-object system
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(src)
include_directories(src/drone)
include_directories(src/simulation)
include_directories(src/movement)
include_directories(src/logging)
include_directories(src/observer)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/drone/drone.cpp
    src/drone/dronedata.cpp
    src/simulation/dronesimulator.cpp
    src/simulation/simulationfactory.cpp
    src/movement/movementstrategy.cpp
    src/movement/hoverstrategy.cpp
    src/movement/randomwalkstrategy.cpp
    src/logging/logger.cpp
    src/observer/observer.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/drone/drone.h
    src/drone/dronedata.h
    src/simulation/dronesimulator.h
    src/simulation/simulationfactory.h
    src/movement/movementstrategy.h
    src/movement/hoverstrategy.h
    src/movement/randomwalkstrategy.h
    src/logging/logger.h
    src/observer/observer.h
)

# UI files
set(UI_FILES
    src/mainwindow.ui
)

# Create the main executable
add_executable(DroneTelemSimulator
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Link Qt libraries
target_link_libraries(DroneTelemSimulator
    Qt6::Core
    Qt6::Widgets
)

# Set executable properties
set_target_properties(DroneTelemSimulator PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Enable testing
enable_testing()

# Find required Qt Test component for testing
find_package(Qt6 COMPONENTS Test QUIET)

if(Qt6Test_FOUND)
    # Enable MOC for test files
    set_property(SOURCE tests/test_simulation.cpp PROPERTY SKIP_AUTOMOC OFF)
    set_property(SOURCE tests/test_movement.cpp PROPERTY SKIP_AUTOMOC OFF)
    set_property(SOURCE tests/test_logger.cpp PROPERTY SKIP_AUTOMOC OFF)

    # Test sources - include all needed implementation files
    set(TEST_SOURCES
        tests/test_simulation.cpp
        src/simulation/dronesimulator.cpp
        src/simulation/simulationfactory.cpp
        src/movement/movementstrategy.cpp
        src/movement/hoverstrategy.cpp
        src/movement/randomwalkstrategy.cpp
        src/logging/logger.cpp
        src/drone/drone.cpp
        src/drone/dronedata.cpp
        src/observer/observer.cpp
    )

    # Create test executable with MOC enabled
    add_executable(DroneTests ${TEST_SOURCES})
    set_target_properties(DroneTests PROPERTIES AUTOMOC ON)
    target_link_libraries(DroneTests Qt6::Core Qt6::Test)
    add_test(NAME SimulationTest COMMAND DroneTests)

    # Additional test executables with MOC
    add_executable(MovementTests
        tests/test_movement.cpp
        src/movement/movementstrategy.cpp
        src/movement/hoverstrategy.cpp
        src/movement/randomwalkstrategy.cpp
        src/drone/dronedata.cpp
    )
    set_target_properties(MovementTests PROPERTIES AUTOMOC ON)
    target_link_libraries(MovementTests Qt6::Core Qt6::Test)
    add_test(NAME MovementTest COMMAND MovementTests)

    add_executable(LoggerTests
        tests/test_logger.cpp
        src/logging/logger.cpp
    )
    set_target_properties(LoggerTests PROPERTIES AUTOMOC ON)
    target_link_libraries(LoggerTests Qt6::Core Qt6::Test)
    add_test(NAME LoggerTest COMMAND LoggerTests)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(DroneTelemSimulator PRIVATE /W4)
else()
    target_compile_options(DroneTelemSimulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
