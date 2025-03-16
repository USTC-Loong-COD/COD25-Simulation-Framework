/**
 * @file enums.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.0.0 Beta
 * @date 2025-03-10
 *
 * @copyright Copyright (c) 2025 __NYA__
 *
 */

#ifndef __ENUMS_HPP__
#define __ENUMS_HPP__

#include <cstdint>

// ISA
enum class IsaType : uint8_t {
    RISC_V,
    LOONGARCH
};

// Core type
// SIMPLE: some difftest will be disabled
// COMPLETE: all difftest will be enabled
enum class CoreType : uint8_t {
    SIMPLE,
    COMPLETE
};

// Difftest level
// NONE: no difftest
// COMMIT: compare every commit
// FULL: compare full system status
enum class DifftestLevel : uint8_t {
    NONE,
    COMMIT,
    FULL
};

#endif // __ENUMS_HPP__