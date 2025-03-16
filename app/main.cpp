/**
 * @file main.cpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.0.0 Beta
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "configs.hpp"
#include "simulator.hpp"
// This warning is due to some kind of design flaw
#include "isa.hpp"

// verilator demands this function
double sc_time_stamp() {
    return 0;
}

int main() {
    auto simulator = Simulator<Configs>();
    simulator.run();
    return 0;
}