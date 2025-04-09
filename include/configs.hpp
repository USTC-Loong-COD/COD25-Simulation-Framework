/**
 * @file configs.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __CONFIGS_HPP__
#define __CONFIGS_HPP__

#include "enums.hpp"
#include <cstdint>

// Memory configs
// instruction memory: [ instr_mem_start, instr_mem_start + (1 << instr_mem_depth) -1 ]
// data memory: [ data_mem_start, data_mem_start + (1 << data_mem_depth) -1 ]
template<IsaType __isa_type>
class MemoryConfigs;
// Default configs for RISC-V core
template<>
class MemoryConfigs<IsaType::RISC_V> {
public:
    inline constexpr static uint32_t instr_mem_start = 0x00400000;
    inline constexpr static uint32_t instr_mem_depth = 16;
    inline constexpr static const char * instr_mem_ini = "mem/instr.ini";
    inline constexpr static uint32_t data_mem_start = 0x10010000;
    inline constexpr static uint32_t data_mem_depth = 16;
    inline constexpr static const char * data_mem_ini = "mem/data.ini";
};
// Default configs for LoongArch core
template<>
class MemoryConfigs<IsaType::LOONGARCH> {
public:
    inline constexpr static uint32_t instr_mem_start = 0x1C000000;
    inline constexpr static uint32_t instr_mem_depth = 16;
    inline constexpr static const char * instr_mem_ini = "mem/instr.ini";
    inline constexpr static uint32_t data_mem_start = 0x1C800000;
    inline constexpr static uint32_t data_mem_depth = 16;
    inline constexpr static const char * data_mem_ini = "mem/data.ini";
};

class Configs {
public:
    // Core type
    inline constexpr static IsaType isa_type = IsaType::RISC_V;
    inline constexpr static CoreType core_type = CoreType::COMPLETE;

    // Difftest configs
    inline constexpr static DifftestLevel difftest_level = DifftestLevel::NONE;
    inline constexpr static bool dump_waveform = false;
    inline constexpr static const char * dump_path = "waveform/waveform.vcd";

    // aliases
    inline constexpr static uint32_t instr_mem_start = MemoryConfigs<isa_type>::instr_mem_start;
    inline constexpr static uint32_t instr_mem_depth = MemoryConfigs<isa_type>::instr_mem_depth;
    inline constexpr static const char * instr_mem_ini = MemoryConfigs<isa_type>::instr_mem_ini;
    inline constexpr static uint32_t data_mem_start = MemoryConfigs<isa_type>::data_mem_start;
    inline constexpr static uint32_t data_mem_depth = MemoryConfigs<isa_type>::data_mem_depth;
    inline constexpr static const char * data_mem_ini = MemoryConfigs<isa_type>::data_mem_ini;
};

#endif // __CONFIGS_HPP__