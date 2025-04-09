/**
 * @file simulator.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include "dut.hpp"
#include "enums.hpp"
#include "golden.hpp"
#include <bitset>
#include <iostream>

#ifdef HAS_STD_FORMAT

#include <format>

#else

#include <fmt/core.h>

namespace std {

// fake std::format
using fmt::format;

}

#endif // USE_STD_FORMAT

template<typename, DifftestLevel>
class SimulatorImpl;

template<typename __Configs>
class SimulatorImpl<__Configs, DifftestLevel::NONE> {
private:
    Dut<__Configs> __dut;

public:
    inline SimulatorImpl() : __dut() {}
    inline void run() {
        __start();
        while(!__dut.__halt) {
            __dut.step();
        }
        __finish();
    }

private:
    inline void __start() {
        std::cout << "Simulation started..." << std::endl;
        std::cout << "Difftest level: " << "\033[32m" << "NONE" << "\033[0m" << "." << std::endl;
        if constexpr(__Configs::dump_waveform) {
            std::cout << "\033[1;33m" << "Dumping waveform to " << __Configs::dump_path << "." << "\033[0m" << std::endl;
        }
    }

    inline void __finish() {
        std::cout << std::format("Simulation finished.\n"
                                 "Time:                           {:<10}\n"
                                 "Instruction count:              {:<10}\n"
                                 "PC:                             0x{:08X}\n"
                                 "Instruction:                    0x{:08X}\n",
                                 __dut.__time,
                                 __dut.__instr_count,
                                 __dut.__pc,
                                 __dut.__instr);
        std::cout << "Registers at the end:\n";
        for(int i = 0; i < 32; i += 4) {
            std::cout << std::format("[{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X}\n",
                                     i, __dut.__reg[i],
                                     i + 1, __dut.__reg[i + 1],
                                     i + 2, __dut.__reg[i + 2],
                                     i + 3, __dut.__reg[i + 3]);
        }
        std::cout << std::flush;
    }
};

template<typename __Configs>
class SimulatorImpl<__Configs, DifftestLevel::COMMIT> {
private:
    Dut<__Configs> __dut;
    Golden<__Configs> __golden;

public:
    inline SimulatorImpl() : __dut(), __golden() {}
    inline void run() {
        __start();
        auto diff = std::bitset<static_cast<int>(DiffType::COUNT)>();
        while(!__dut.__halt && !__golden.__halt) {
            __dut.step();
            __golden.step();
            if(diff = __diff(); diff.any()) break;
        }
        if(diff.any()) {
            __print_difference(diff);
            __abort();
        }
        else {
            __win();
            __finish();
        }
    }

private:
    enum class DiffType {
        HALT_DIFF,
        PC_DIFF,
        INSTR_DIFF,
        REG_WE_DIFF,
        REG_WA_DIFF,
        REG_WD_DIFF,
        DMEM_WE_DIFF,
        DMEM_WA_DIFF,
        DMEM_WD_DIFF,
        COUNT
    };

    inline void __start() {
        std::cout << "Simulation started..." << std::endl;
        std::cout << "Difftest level: " << "\033[33m" << "COMMIT" << "\033[0m" << "." << std::endl;
        if constexpr(__Configs::dump_waveform) {
            std::cout << "\033[1;33m" << "Dumping waveform to " << __Configs::dump_path << "." << "\033[0m" << std::endl;
        }
    }

    inline std::bitset<static_cast<int>(DiffType::COUNT)> __diff() {
        auto res = std::bitset<static_cast<int>(DiffType::COUNT)>();
        res.set(static_cast<int>(DiffType::HALT_DIFF), __dut.__halt != __golden.__halt);
        res.set(static_cast<int>(DiffType::PC_DIFF), __dut.__pc != __golden.__pc);
        res.set(static_cast<int>(DiffType::INSTR_DIFF), __dut.__instr != __golden.__instr);
        res.set(static_cast<int>(DiffType::REG_WE_DIFF), __dut.__reg_we != __golden.__reg_we);
        res.set(static_cast<int>(DiffType::REG_WA_DIFF), __golden.__reg_we && __dut.__reg_wa != __golden.__reg_wa);
        res.set(static_cast<int>(DiffType::REG_WD_DIFF), __golden.__reg_we && __golden.__reg_wa && __dut.__reg_wd != __golden.__reg_wd);
        if constexpr(__Configs::core_type != CoreType::SIMPLE) {
            res.set(static_cast<int>(DiffType::DMEM_WE_DIFF), __dut.__dmem_we != __golden.__dmem_we);
            res.set(static_cast<int>(DiffType::DMEM_WA_DIFF), __golden.__dmem_we && __dut.__dmem_wa != __golden.__dmem_wa);
            res.set(static_cast<int>(DiffType::DMEM_WD_DIFF), __golden.__dmem_we && __dut.__dmem_wd != __golden.__dmem_wd);
        }
        return res;
    }

    inline void __print_difference(const std::bitset<static_cast<int>(DiffType::COUNT)> & _diff) {
        std::cout << "Difference detected:" << std::string(52 - 18, ' ') << std::endl;
        std::cout << "|   Signal   |  Your Core  |  Reference Core  |" << std::endl;
        std::cout << "|------------|-------------|------------------|" << std::endl;
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::HALT_DIFF)) ? "\033[31m" : "\033[32m", "halt", "\033[0m", __dut.__halt ? "true" : "false", __golden.__halt ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::PC_DIFF)) ? "\033[31m" : "\033[32m", "pc", "\033[0m", __dut.__pc, __golden.__pc);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::INSTR_DIFF)) ? "\033[31m" : "\033[32m", "inst", "\033[0m", __dut.__instr, __golden.__instr);
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::REG_WE_DIFF)) ? "\033[31m" : "\033[32m", "reg_we", "\033[0m", __dut.__reg_we ? "true" : "false", __golden.__reg_we ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::REG_WA_DIFF)) ? "\033[31m" : "\033[32m", "reg_wa", "\033[0m", __dut.__reg_wa, __golden.__reg_wa);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::REG_WD_DIFF)) ? "\033[31m" : "\033[32m", "reg_wd", "\033[0m", __dut.__reg_wd, __golden.__reg_wd);
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::DMEM_WE_DIFF)) ? "\033[31m" : "\033[32m", "dmem_we", "\033[0m", __dut.__dmem_we ? "true" : "false", __golden.__dmem_we ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::DMEM_WA_DIFF)) ? "\033[31m" : "\033[32m", "dmem_wa", "\033[0m", __dut.__dmem_wa, __golden.__dmem_wa);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::DMEM_WD_DIFF)) ? "\033[31m" : "\033[32m", "dmem_wd", "\033[0m", __dut.__dmem_wd, __golden.__dmem_wd);
        std::cout << std::flush;
    }

    inline void __finish() {
        std::cout << std::format("Simulation finished.\n"
                                 "Time:                           {:<10}\n"
                                 "Instruction count:              {:<10}\n"
                                 "PC:                             0x{:08X}\n"
                                 "Instruction:                    0x{:08X}\n",
                                 __dut.__time,
                                 __dut.__instr_count,
                                 __dut.__pc,
                                 __dut.__instr);
        std::cout << "Registers at the end:\n";
        for(int i = 0; i < 32; i += 4) {
            std::cout << std::format("[{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X}\n",
                                     i, __dut.__reg[i],
                                     i + 1, __dut.__reg[i + 1],
                                     i + 2, __dut.__reg[i + 2],
                                     i + 3, __dut.__reg[i + 3]);
        }
        std::cout << std::flush;
    }

    inline void __abort() {
        std::cout << "\033[31m" << "Simulation aborted." << "\033[0m" << std::endl;
    }

    inline void __win() {
        std::cout << "\033[32m" << "Hit good trap." << "\033[0m" << std::endl;
    }
};

template<typename __Configs>
class SimulatorImpl<__Configs, DifftestLevel::FULL> {
private:
    Golden<__Configs> __golden;
    Dut<__Configs> __dut;

public:
    inline SimulatorImpl() : __golden(), __dut() {}
    inline void run() {
        __start();
        auto diff = std::bitset<static_cast<int>(DiffType::COUNT)>();
        while(!__dut.__halt && !__golden.__halt) {
            __dut.step();
            __golden.step();
            if(diff = __diff(); diff.any()) break;
        }
        if(diff.any()) {
            __print_difference(diff);
            __abort();
        }
        else {
            __win();
            __finish();
        }
    }

private:
    enum class DiffType {
        HALT_DIFF,
        PC_DIFF,
        INSTR_DIFF,
        REG_WE_DIFF,
        REG_WA_DIFF,
        REG_WD_DIFF,
        DMEM_WE_DIFF,
        DMEM_WA_DIFF,
        DMEM_WD_DIFF,
        REG_DIFF,
        DMEM_DIFF,
        COUNT
    };

    inline void __start() {
        std::cout << "Simulation started..." << std::endl;
        std::cout << "Difftest level: " << "\033[31m" << "FULL" << "\033[0m" << "." << std::endl;
        if constexpr(__Configs::dump_waveform) {
            std::cout << "\033[1;33m" << "Dumping waveform to " << __Configs::dump_path << "." << "\033[0m" << std::endl;
        }
    }

    inline std::bitset<static_cast<int>(DiffType::COUNT)> __diff() {
        auto res = std::bitset<static_cast<int>(DiffType::COUNT)>();
        res.set(static_cast<int>(DiffType::HALT_DIFF), __dut.__halt != __golden.__halt);
        res.set(static_cast<int>(DiffType::PC_DIFF), __dut.__pc != __golden.__pc);
        res.set(static_cast<int>(DiffType::INSTR_DIFF), __dut.__instr != __golden.__instr);
        res.set(static_cast<int>(DiffType::REG_WE_DIFF), __dut.__reg_we != __golden.__reg_we);
        res.set(static_cast<int>(DiffType::REG_WA_DIFF), __golden.__reg_we && __dut.__reg_wa != __golden.__reg_wa);
        res.set(static_cast<int>(DiffType::REG_WD_DIFF), __golden.__reg_we && __golden.__reg_wa && __dut.__reg_wd != __golden.__reg_wd);
        for(int i = 0; i < 32; ++i) {
            if(__dut.__reg[i] != __golden.__reg[i]) {
                res.set(static_cast<int>(DiffType::REG_DIFF), true);
                break;
            }
        }
        if constexpr(__Configs::core_type != CoreType::SIMPLE) {
            res.set(static_cast<int>(DiffType::DMEM_WE_DIFF), __dut.__dmem_we != __golden.__dmem_we);
            res.set(static_cast<int>(DiffType::DMEM_WA_DIFF), __golden.__dmem_we && __dut.__dmem_wa != __golden.__dmem_wa);
            res.set(static_cast<int>(DiffType::DMEM_WD_DIFF), __golden.__dmem_we && __dut.__dmem_wd != __golden.__dmem_wd);
            for(int i = 0; i < 1 << __Configs::data_mem_depth; ++i) {
                if(__dut.__data_mem[i] != __golden.__data_mem[i]) {
                    res.set(static_cast<int>(DiffType::DMEM_DIFF), true);
                    break;
                }
            }
        }
        return res;
    }

    inline void __print_difference(const std::bitset<static_cast<int>(DiffType::COUNT)> & _diff) {
        std::cout << "Difference detected:" << std::endl;
        std::cout << "|   Signal   |  Your Core  |  Reference Core  |" << std::endl;
        std::cout << "|------------|-------------|------------------|" << std::endl;
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::HALT_DIFF)) ? "\033[31m" : "\033[32m", "halt", "\033[0m", __dut.__halt ? "true" : "false", __golden.__halt ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::PC_DIFF)) ? "\033[31m" : "\033[32m", "pc", "\033[0m", __dut.__pc, __golden.__pc);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::INSTR_DIFF)) ? "\033[31m" : "\033[32m", "inst", "\033[0m", __dut.__instr, __golden.__instr);
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::REG_WE_DIFF)) ? "\033[31m" : "\033[32m", "reg_we", "\033[0m", __dut.__reg_we ? "true" : "false", __golden.__reg_we ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::REG_WA_DIFF)) ? "\033[31m" : "\033[32m", "reg_wa", "\033[0m", __dut.__reg_wa, __golden.__reg_wa);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::REG_WD_DIFF)) ? "\033[31m" : "\033[32m", "reg_wd", "\033[0m", __dut.__reg_wd, __golden.__reg_wd);
        std::cout << std::format("|{}{:^12}{}|{:^13}|{:^18}|\n", _diff.test(static_cast<int>(DiffType::DMEM_WE_DIFF)) ? "\033[31m" : "\033[32m", "dmem_we", "\033[0m", __dut.__dmem_we ? "true" : "false", __golden.__dmem_we ? "true" : "false");
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::DMEM_WA_DIFF)) ? "\033[31m" : "\033[32m", "dmem_wa", "\033[0m", __dut.__dmem_wa, __golden.__dmem_wa);
        std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::DMEM_WD_DIFF)) ? "\033[31m" : "\033[32m", "dmem_wd", "\033[0m", __dut.__dmem_wd, __golden.__dmem_wd);
        for(auto i = 0; i < 32; ++i) {
            if(__dut.__reg[i] != __golden.__reg[i]) {
                std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::REG_DIFF)) ? "\033[31m" : "\033[32m", std::format("gr[{}]", i), "\033[0m", __dut.__reg[i], __golden.__reg[i]);
            }
        }
        for(auto i = 0; i < 1 << __Configs::data_mem_depth; ++i) {
            if(__dut.__data_mem[i] != __golden.__data_mem[i]) {
                std::cout << std::format("|{}{:^12}{}| 0x{:08X}  |    0x{:08X}    |\n", _diff.test(static_cast<int>(DiffType::DMEM_DIFF)) ? "\033[31m" : "\033[32m", std::format("dmem[{}]", i), "\033[0m", __dut.__data_mem[i], __golden.__data_mem[i]);
            }
        }
        std::cout << std::flush;
    }

    inline void __finish() {
        std::cout << std::format("Simulation finished.\n"
                                 "Time:                           {:<10}\n"
                                 "Instruction count:              {:<10}\n"
                                 "PC:                             0x{:08X}\n"
                                 "Instruction:                    0x{:08X}\n",
                                 __dut.__time,
                                 __dut.__instr_count,
                                 __dut.__pc,
                                 __dut.__instr);
        std::cout << "Registers at the end:\n";
        for(int i = 0; i < 32; i += 4) {
            std::cout << std::format("[{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X} [{:2}] = 0x{:08X}\n",
                                     i, __dut.__reg[i],
                                     i + 1, __dut.__reg[i + 1],
                                     i + 2, __dut.__reg[i + 2],
                                     i + 3, __dut.__reg[i + 3]);
        }
        std::cout << std::flush;
    }

    inline void __abort() {
        std::cout << "\033[31m" << "Simulation aborted." << "\033[0m" << std::endl;
    }

    inline void __win() {
        std::cout << "\033[32m" << "Hit good trap." << "\033[0m" << std::endl;
    }
};

template<typename __Configs>
using Simulator = SimulatorImpl<__Configs, __Configs::difftest_level>;

#endif // __SIMULATOR_HPP__