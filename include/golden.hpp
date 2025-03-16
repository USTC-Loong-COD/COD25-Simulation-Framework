/**
 * @file golden.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.0.0 Beta
 * @date 2025-03-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __GOLDEN_HPP__
#define __GOLDEN_HPP__

#include "enums.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <vector>

template<typename __Configs>
class Isa;

template<typename __Configs>
class Golden {
    template<typename>
    friend class ArgumentGenerator;

    template<typename, DifftestLevel>
    friend class SimulatorImpl;

private:
    std::array<uint32_t, 32> __reg;
    std::vector<uint32_t> __instr_mem;
    std::vector<uint32_t> __data_mem;

    uint32_t __pc;
    uint32_t __npc;
    uint32_t __instr;

    uint32_t __reg_we;
    uint32_t __reg_wa;
    uint32_t __reg_wd;

    uint32_t __dmem_we;
    uint32_t __dmem_wa;
    uint32_t __dmem_wd;

    uint32_t __halt;

public:
    inline Golden()
        : __instr_mem(1 << __Configs::instr_mem_depth), __data_mem(1 << __Configs::data_mem_depth),
          __pc(__Configs::instr_mem_start), __npc(__Configs::instr_mem_start), __instr(0),
          __reg_we(false), __reg_wa(0), __reg_wd(0),
          __dmem_we(false), __dmem_wa(0), __dmem_wd(0), __halt(false) {
        for(uint32_t i = 0; i < 32; i++) {
            __reg[i] = 0;
        }

        auto ifs = std::ifstream(__Configs::instr_mem_ini);
        if(!ifs.is_open()) throw std::runtime_error("Failed to open file: " + std::string(__Configs::instr_mem_ini));
        for(uint32_t i = 0; i < (1 << __Configs::instr_mem_depth) && !ifs.eof(); i++) {
            ifs >> std::hex >> __instr_mem[i];
        }

        ifs = std::ifstream(__Configs::data_mem_ini);
        if(!ifs.is_open()) throw std::runtime_error("Failed to open file: " + std::string(__Configs::instr_mem_ini));
        for(uint32_t i = 0; i < (1 << __Configs::data_mem_depth) && !ifs.eof(); i++) {
            ifs >> std::hex >> __data_mem[i];
        }
    }

    inline void step() {
        if(__halt) return;
        __pc = __npc;
        __instr = __instr_mem[(__pc - __Configs::instr_mem_start) >> 2];
        __npc += 4;
        __reg_we = false;
        __dmem_we = false;
        // __halt = false;
        for(auto & op : Isa<__Configs>::decode(__instr)) {
            (*op)(*this);
        }
    }

    inline bool halted() const {
        return __halt;
    }
};

#endif // __GOLDEN_HPP__