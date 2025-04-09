/**
 * @file dut.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __DUT_HPP__
#define __DUT_HPP__

#include "VTop.h"
#include "enums.hpp"
#include "verilated_vcd_c.h"
#include <cstdint>
#include <memory>

template<typename __Configs>
class Dut {
    template<typename, DifftestLevel>
    friend class SimulatorImpl;

private:
    class Register {
    private:
        std::shared_ptr<VTop> __dut_ptr;

    public:
        inline Register(std::shared_ptr<VTop> _dut_ptr) : __dut_ptr(_dut_ptr) {}

        inline uint32_t operator[](uint32_t _idx) {
            __dut_ptr->debug_reg_ra = _idx;
            __dut_ptr->eval();
            return __dut_ptr->debug_reg_rd;
        }
    };

    class DataMemory {
    private:
        std::shared_ptr<VTop> __dut_ptr;

    public:
        inline DataMemory(std::shared_ptr<VTop> _dut_ptr) : __dut_ptr(_dut_ptr) {}

        inline uint32_t operator[](uint32_t _idx) {
            __dut_ptr->debug_dmem_ra = _idx;
            __dut_ptr->eval();
            return __dut_ptr->debug_dmem_rd;
        }
    };

private:
    std::shared_ptr<VTop> __dut_ptr;
    std::unique_ptr<VerilatedVcdC> __trace_file_ptr;

    uint32_t __time;
    uint32_t __instr_count;

    decltype(VTop::clk) & __clk;
    decltype(VTop::rst) & __rst;
    decltype(VTop::commit_pc) & __pc;
    decltype(VTop::commit_instr) & __instr;
    decltype(VTop::commit) & __commit;
    decltype(VTop::commit_reg_we) & __reg_we;
    decltype(VTop::commit_reg_wa) & __reg_wa;
    decltype(VTop::commit_reg_wd) & __reg_wd;
    decltype(VTop::commit_dmem_we) & __dmem_we;
    decltype(VTop::commit_dmem_wa) & __dmem_wa;
    decltype(VTop::commit_dmem_wd) & __dmem_wd;
    decltype(VTop::commit_halt) & __halt;

    Register __reg;
    DataMemory __data_mem;

public:
    inline Dut()
        : __dut_ptr(std::make_shared<VTop>()), __trace_file_ptr(nullptr),
          __time(0), __instr_count(0),
          __clk(__dut_ptr->clk), __rst(__dut_ptr->rst), __pc(__dut_ptr->commit_pc), __instr(__dut_ptr->commit_instr), __commit(__dut_ptr->commit),
          __reg_we(__dut_ptr->commit_reg_we), __reg_wa(__dut_ptr->commit_reg_wa), __reg_wd(__dut_ptr->commit_reg_wd),
          __dmem_we(__dut_ptr->commit_dmem_we), __dmem_wa(__dut_ptr->commit_dmem_wa), __dmem_wd(__dut_ptr->commit_dmem_wd),
          __halt(__dut_ptr->commit_halt),
          __reg(__dut_ptr), __data_mem(__dut_ptr) {
        if constexpr(__Configs::dump_waveform) {
            Verilated::traceEverOn(true);
            __trace_file_ptr = std::make_unique<VerilatedVcdC>();
            __dut_ptr->trace(__trace_file_ptr.get(), 99);
            if(auto path_string = std::string(__Configs::dump_path); path_string.find('/') != std::string::npos) {
                std::string command = "mkdir -p " + path_string.substr(0, path_string.find_last_of('/'));
                // to make compiler happy
                (void)system(command.c_str());
            }
            __trace_file_ptr->open(__Configs::dump_path);
        }

        __clk = 0;
        __eval();
        __reset();
    }

    inline ~Dut() {
        if constexpr(__Configs::dump_waveform) {
            __trace_file_ptr->close();
        }
    }

    inline void step() {
        do {
            __step_cycle();
        } while(!__commit);
        __instr_count++;
    }

private:
    inline void __reset() {
        __rst = 1;
        __step_cycle();
        __rst = 0;
    }

    inline void __clock_turnover() {
        __clk = !__clk;
        __eval();
        __dump();
    }

    inline void __step_cycle() {
        __clock_turnover();
        __clock_turnover();
    }

    inline void __eval() {
        __dut_ptr->eval();
    }

    inline void __dump() {
        if constexpr(__Configs::dump_waveform) {
            __trace_file_ptr->dump(__time);
        }
        __time++;
    }
};

#endif // __DUT_HPP__
