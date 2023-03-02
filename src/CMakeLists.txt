project(FX31337-wasm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

add_executable(TaskRunner TaskRunner.cpp)
add_executable(TradeSignalManager TradeSignalManager.cpp)
add_executable(IndicatorTest IndicatorTest.cpp)
