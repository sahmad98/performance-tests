cmake_minimum_required(VERSION 3.10)

project(PerfTest)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE ${PROJECT_SOURCE_DIR}/src)
#set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(RowColumnAccess ${SOURCE}/array_access_test.cpp)
add_executable(CPUPinning ${SOURCE}/cpu_pinning.cpp)
add_executable(FileReadPerf ${SOURCE}/memory_mapped_io.cpp)
