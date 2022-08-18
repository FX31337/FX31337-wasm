project(FX31337-wasm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)

add_executable(TaskRunner TaskRunner.cpp)
add_executable(TradeSignalManager TradeSignalManager.cpp)
