cmake_minimum_required(VERSION 3.20.0)
project(LoongCODSim VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
  #include <format>
  int main() {
    std::format(\"Hello {}\", \"world\");
    return 0;
  }
" HAS_STD_FORMAT)

if(HAS_STD_FORMAT)
    message(STATUS "Compiler supports std::format")
    add_compile_definitions(HAS_STD_FORMAT)
else()
    message(STATUS "Compiler does NOT support std::format, falling back to fmt")
    find_package(fmt REQUIRED)
endif()

add_executable(sim app/main.cpp)

include_directories(include)

if(NOT HAS_STD_FORMAT)
    target_link_libraries(sim PRIVATE fmt::fmt)
endif()

find_package(VERILATOR HINTS $ENV{VERILATOR_ROOT})
file(GLOB_RECURSE VSRCS ${CMAKE_SOURCE_DIR}/vsrc/your_vsrc/*.v)
verilate(   sim
            SOURCES ${CMAKE_SOURCE_DIR}/vsrc/top.v ${VSRCS} ${CMAKE_SOURCE_DIR}/vsrc/mem_ip_sim/data_mem.v ${CMAKE_SOURCE_DIR}/vsrc/mem_ip_sim/inst_mem.v
            INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/vsrc/configs ${CMAKE_SOURCE_DIR}/vsrc/your_include
            TOP_MODULE Top
            PREFIX VTop
            DIRECTORY ${CMAKE_SOURCE_DIR}/generated
            TRACE
)
