# Real-Time Drone Telemetry Simulator

A Qt-based desktop application that simulates and displays real-time telemetry data from a drone. The application demonstrates proper use of design patterns, multithreading, and modular architecture.

## Features

### Telemetry Display
- **Drone ID**: Unique identifier for the drone
- **Latitude/Longitude**: GPS coordinates with 6 decimal precision
- **Altitude**: Height in meters with 1 decimal precision  
- **Heading**: Direction in degrees with 1 decimal precision
- **Speed**: Velocity in m/s with 2 decimal precision
- **Battery**: Charge level as percentage with 1 decimal precision
- **GPS Fix Status**: No Fix / 2D / 3D with color coding

### Real-Time Simulation
- Updates every 500 milliseconds using worker thread
- Realistic data changes: location shifts, speed variations, heading drift, battery drain
- Toggle "Simulate Failure" mode that drops GPS fix and rapidly reduces battery

### Movement Behaviors  
- **Hover Mode**: Small circular movement with minor drift
- **Random Walk**: Unpredictable movement within geographic bounds

## Architecture & Design Patterns

This project demonstrates four key design patterns as required:

### 1. Observer Pattern
**Location**: `src/observer/observer.h`, `src/observer/observer.cpp`  
**Purpose**: UI updates from drone data model  
**Implementation**: 
- `Observer` interface with `update(const DroneData&)` method
- `Subject` interface with `attach()`, `detach()`, `notify()` methods  
- `DroneSimulator` inherits from `Subject` and notifies observers on data changes
- `MainWindow` inherits from `Observer` and updates UI when notified

### 2. Factory Pattern  
**Location**: `src/simulation/simulationfactory.h`, `src/simulation/simulationfactory.cpp`  
**Purpose**: Create drone simulator and movement strategy objects  
**Implementation**:
- `SimulationFactory::createSimulator()` creates different simulator types
- `SimulationFactory::createMovementStrategy()` creates different movement behaviors
- Enum types define available simulator and movement types
- Centralized object creation with consistent interfaces

### 3. Strategy Pattern
**Location**: `src/movement/` directory  
**Purpose**: Implement different drone movement behaviors  
**Implementation**:
- `MovementStrategy` abstract base class defines interface
- `HoverStrategy` implements hovering behavior with small circular movements
- `RandomWalkStrategy` implements unpredictable movement within bounds
- Strategies are interchangeable at runtime via Factory pattern

### 4. Singleton Pattern  
**Location**: `src/logging/logger.h`, `src/logging/logger.cpp`  
**Purpose**: Centralized logger for system events  
**Implementation**:
- Thread-safe Meyer's Singleton with `getInstance()` method
- Mutex-protected logging to prevent race conditions
- Configurable log levels (DEBUG, INFO, WARNING, ERROR)
- Outputs to both console and file simultaneously
- Private constructor and deleted copy operations

## Building & Running

### Prerequisites
- Qt 6.0 or later
- CMake 3.16 or later  
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd drone-telemetry-simulator

# Create build directory
mkdir build
cd build

# Configure with CMake  
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt6 ..

# Build the project
cmake --build .

# Run the application
./DroneTelemSimulator
```

#### Alternative: Using Qt Creator
1. Open `CMakeLists.txt` in Qt Creator
2. Configure the project with your Qt kit
3. Build and run using F5

### Running Tests

```bash
# In the build directory
ctest --verbose

# Or run individual test executables
./DroneTests
./MovementTests  
./LoggerTests
```

## Project Structure

```
drone-telemetry-simulator/
├── src/
│   ├── main.cpp                    # Application entry point
│   ├── mainwindow.h/.cpp/.ui      # Main GUI window
│   ├── drone/
│   │   ├── drone.h/.cpp           # Drone model class  
│   │   └── dronedata.h/.cpp       # Telemetry data structure
│   ├── simulation/
│   │   ├── dronesimulator.h/.cpp  # Core simulation engine
│   │   └── simulationfactory.h/.cpp # Factory for creating objects
│   ├── movement/
│   │   ├── movementstrategy.h/.cpp    # Strategy interface
│   │   ├── hoverstrategy.h/.cpp       # Hover movement implementation
│   │   └── randomwalkstrategy.h/.cpp  # Random walk implementation  
│   ├── logging/
│   │   └── logger.h/.cpp          # Singleton logger implementation
│   └── observer/
│       └── observer.h/.cpp        # Observer pattern interfaces
├── tests/
│   ├── test_simulation.cpp        # Simulation logic tests
│   ├── test_movement.cpp         # Movement strategy tests  
│   └── test_logger.cpp           # Logger functionality tests
├── CMakeLists.txt                # Build configuration
└── README.md                     # This file
```

## Design Decisions & Technical Details

### Multithreading Architecture
- **Main Thread**: Handles GUI updates and user interactions
- **Timer Thread**: QTimer generates update signals every 500ms
- **Worker Pattern**: DroneSimulator runs telemetry updates in background
- **Thread Safety**: Mutex protection in Logger, signal/slot communication

### Data Flow
1. `DroneSimulator` generates telemetry data every 500ms
2. Movement strategy updates drone position/orientation  
3. Battery simulation decreases charge over time
4. Observer notification triggers UI updates
5. All events logged through centralized Logger

### Error Handling
- Failure mode simulation (GPS loss, rapid battery drain)
- Bounds checking for geographic coordinates
- Null pointer checks for strategy objects
- Exception-safe resource management with smart pointers

### Performance Considerations  
- Efficient observer notification (vector iteration)
- Minimal heap allocations in update loop
- Asynchronous logging to prevent UI blocking
- Smart pointer usage for automatic memory management

## Testing

The project includes comprehensive unit tests covering:

- **Simulation Logic**: Start/stop, failure modes, data generation
- **Movement Strategies**: Position updates, bounds checking, strategy switching  
- **Logger Functionality**: Singleton behavior, file output, log levels
- **Observer Pattern**: Notification delivery, attachment/detachment

## Future Enhancements

- Multiple drone support
- Real-time data visualization (maps, charts)
- Network communication simulation  
- Flight path recording and playback
- Advanced failure scenarios
- Configuration file support
