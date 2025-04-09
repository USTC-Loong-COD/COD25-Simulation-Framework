/**
 * @file argument_generator.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __ARGUMENT_GENERATOR_HPP__
#define __ARGUMENT_GENERATOR_HPP__

#include "argument.hpp"
#include "golden.hpp"
#include <cstdint>

template<typename __Configs>
class ArgumentGenerator {
private:
    inline static uint32_t __temp[32];

    // for those writing to gr[0]
    inline static uint32_t __void = 0;

protected:

    inline static uint32_t & get_register_unsigned(uint32_t _idx, Golden<__Configs> & _golden) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
        return _golden.__reg[_idx];
    }
    inline static uint32_t & get_register_signed(uint32_t _idx, Golden<__Configs> & _golden) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
        return _golden.__reg[_idx];
    }
    inline static uint32_t get_register_unsigned_const(uint32_t _idx, Golden<__Configs> & _golden) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
        return _golden.__reg[_idx];
    }
    inline static uint32_t get_register_signed_const(uint32_t _idx, Golden<__Configs> & _golden) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
        return _golden.__reg[_idx];
    }
    inline static uint32_t & get_void(Golden<__Configs> & _golden) {
        return __void;
    }

    inline static uint32_t & get_data_mem_unsigned(uint32_t _addr, Golden<__Configs> & _golden) {
        _addr -= __Configs::data_mem_start;
        return _golden.__data_mem[_addr >> 2];
    }
    inline static uint32_t & get_data_mem_signed(uint32_t _addr, Golden<__Configs> & _golden) {
        _addr -= __Configs::data_mem_start;
        return _golden.__data_mem[_addr >> 2];
    }
    inline static uint32_t get_data_mem_unsigned_const(uint32_t _addr, Golden<__Configs> & _golden) {
        _addr -= __Configs::data_mem_start;
        return _golden.__data_mem[_addr >> 2];
    }
    inline static uint32_t get_data_mem_signed_const(uint32_t _addr, Golden<__Configs> & _golden) {
        _addr -= __Configs::data_mem_start;
        return _golden.__data_mem[_addr >> 2];
    }

    inline static uint32_t get_pc_const(Golden<__Configs> & _golden) {
        return _golden.__pc;
    }
    inline static uint32_t & get_npc(Golden<__Configs> & _golden) {
        return _golden.__npc;
    }

    inline static uint32_t & get_halt(Golden<__Configs> & _golden) {
        return _golden.__halt;
    }

    inline static uint32_t & get_reg_we(Golden<__Configs> & _golden) {
        return _golden.__reg_we;
    }
    inline static uint32_t & get_reg_wa(Golden<__Configs> & _golden) {
        return _golden.__reg_wa;
    }
    inline static uint32_t & get_reg_wd(Golden<__Configs> & _golden) {
        return _golden.__reg_wd;
    }

    inline static uint32_t & get_mem_we(Golden<__Configs> & _golden) {
        return _golden.__dmem_we;
    }
    inline static uint32_t & get_mem_wa(Golden<__Configs> & _golden) {
        return _golden.__dmem_wa;
    }
    inline static uint32_t & get_mem_wd(Golden<__Configs> & _golden) {
        return _golden.__dmem_wd;
    }

    inline static uint32_t get_imm_unsigned(uint32_t _imm) {
        return _imm;
    }
    inline static uint32_t get_imm_signed(uint32_t _imm) {
        return static_cast<int32_t>(_imm);
    }

    inline static uint32_t & get_temp_unsigned(uint32_t _idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid temp index: " + std::to_string(_idx));
        return __temp[_idx];
    }
    inline static uint32_t & get_temp_signed(uint32_t _idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid temp index: " + std::to_string(_idx));
        return __temp[_idx];
    }
    inline static uint32_t get_temp_unsigned_const(uint32_t _idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid temp index: " + std::to_string(_idx));
        return __temp[_idx];
    }
    inline static uint32_t get_temp_signed_const(uint32_t _idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid temp index: " + std::to_string(_idx));
        return __temp[_idx];
    }
};

template<typename __Configs>
class UnsignedRegisterGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline UnsignedRegisterGenerator(uint32_t _idx) : __idx(_idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        if(__idx == 0) return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_void(_golden));
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_register_unsigned(__idx, _golden));
    }
};

template<typename __Configs>
class SignedRegisterGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline SignedRegisterGenerator(uint32_t _idx) : __idx(_idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline VariableCompleteSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        if(__idx == 0) return VariableCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_void(_golden));
        return VariableCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_register_signed(__idx, _golden));
    }
};

template<typename __Configs>
class ConstUnsignedRegisterGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline ConstUnsignedRegisterGenerator(uint32_t _idx) : __idx(_idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline ConstantCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_register_unsigned_const(__idx, _golden));
    }
};

template<typename __Configs>
class ConstSignedRegisterGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline ConstSignedRegisterGenerator(uint32_t _idx) : __idx(_idx) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline ConstantCompleteSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_register_signed_const(__idx, _golden));
    }
};

template<typename __Configs>
class UnsignedDataMemGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;
    uint32_t __offset;
    uint32_t __bits;

public:
    inline UnsignedDataMemGenerator(uint32_t _idx, uint32_t _offset, uint32_t _bits)
        : __idx(_idx), __offset(_offset), __bits(_bits) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline VariablePartialUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        uint32_t _addr = ArgumentGenerator<__Configs>::get_register_unsigned(__idx, _golden) + __offset;
        if(_addr < __Configs::data_mem_start || _addr >= __Configs::data_mem_start + (1 << (__Configs::data_mem_depth + 2))) {
            throw std::runtime_error("Invalid data memory address: " + std::to_string(_addr));
        }
        uint32_t bit_offset = (_addr << 3) & (32 - __bits);
        return VariablePartialUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_data_mem_unsigned(_addr, _golden), __bits, bit_offset);
    }
};

template<typename __Configs>
class SignedDataMemGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;
    uint32_t __offset;
    uint32_t __bits;

public:
    inline SignedDataMemGenerator(uint32_t _idx, uint32_t _offset, uint32_t _bits)
        : __idx(_idx), __offset(_offset), __bits(_bits) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline VariablePartialSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        uint32_t _addr = ArgumentGenerator<__Configs>::get_register_unsigned(__idx, _golden) + __offset;
        if(_addr < __Configs::data_mem_start || _addr >= __Configs::data_mem_start + (1 << (__Configs::data_mem_depth + 2))) {
            throw std::runtime_error("Invalid data memory address: " + std::to_string(_addr));
        }
        uint32_t bit_offset = (_addr << 3) & (32 - __bits);
        return VariablePartialSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_data_mem_signed(_addr, _golden), __bits, bit_offset);
    }
};

template<typename __Configs>
class ConstUnsignedDataMemGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;
    uint32_t __offset;
    uint32_t __bits;

public:
    inline ConstUnsignedDataMemGenerator(uint32_t _idx, uint32_t _offset, uint32_t _bits)
        : __idx(_idx), __offset(_offset), __bits(_bits) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline ConstantPartialUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        uint32_t _addr = ArgumentGenerator<__Configs>::get_register_unsigned(__idx, _golden) + __offset;
        if(_addr < __Configs::data_mem_start || _addr >= __Configs::data_mem_start + (1 << (__Configs::data_mem_depth + 2))) {
            throw std::runtime_error("Invalid data memory address: " + std::to_string(_addr));
        }
        uint32_t bit_offset = (_addr << 3) & (32 - __bits);
        return ConstantPartialUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_data_mem_unsigned_const(_addr, _golden), __bits, bit_offset);
    }
};

template<typename __Configs>
class ConstSignedDataMemGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;
    uint32_t __offset;
    uint32_t __bits;

public:
    inline ConstSignedDataMemGenerator(uint32_t _idx, uint32_t _offset, uint32_t _bits)
        : __idx(_idx), __offset(_offset), __bits(_bits) {
        if(_idx >= 32) throw std::runtime_error("Invalid register index: " + std::to_string(_idx));
    }
    inline ConstantPartialSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        uint32_t _addr = ArgumentGenerator<__Configs>::get_register_unsigned(__idx, _golden) + __offset;
        if(_addr < __Configs::data_mem_start || _addr >= __Configs::data_mem_start + (1 << (__Configs::data_mem_depth + 2))) {
            throw std::runtime_error("Invalid data memory address: " + std::to_string(_addr));
        }
        uint32_t bit_offset = (_addr << 3) & (32 - __bits);
        return ConstantPartialSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_data_mem_signed_const(_addr, _golden), __bits, bit_offset);
    }
};

template<typename __Configs>
class ConstPcGenerator : public ArgumentGenerator<__Configs> {
public:
    inline ConstantCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_pc_const(_golden));
    }
};

template<typename __Configs>
class NpcGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_npc(_golden));
    }
};

template<typename __Configs>
class HaltGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_halt(_golden));
    }
};

template<typename __Configs>
class RegWeGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_reg_we(_golden));
    }
};

template<typename __Configs>
class RegWaGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_reg_wa(_golden));
    }
};

template<typename __Configs>
class RegWdGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_reg_wd(_golden));
    }
};

template<typename __Configs>
class MemWeGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_mem_we(_golden));
    }
};

template<typename __Configs>
class MemWaGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_mem_wa(_golden));
    }
};

template<typename __Configs>
class MemWdGenerator : public ArgumentGenerator<__Configs> {
public:
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_mem_wd(_golden));
    }
};

template<typename __Configs>
class UnsignedImmGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __imm;

public:
    inline UnsignedImmGenerator(uint32_t _imm) : __imm(_imm) {}
    inline ConstantCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_imm_unsigned(__imm));
    }
};

template<typename __Configs>
class SignedImmGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __imm;

public:
    inline SignedImmGenerator(uint32_t _imm) : __imm(_imm) {}
    inline ConstantCompleteSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_imm_signed(__imm));
    }
};

template<typename __Configs>
class UnsignedTempGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline UnsignedTempGenerator(uint32_t _idx) : __idx(_idx) {
        if(__idx > 1) throw std::runtime_error("Invalid temp index: " + std::to_string(__idx));
    }
    inline VariableCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_temp_unsigned(__idx));
    }
};

template<typename __Configs>
class SignedTempGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline SignedTempGenerator(uint32_t _idx) : __idx(_idx) {
        if(__idx > 1) throw std::runtime_error("Invalid temp index: " + std::to_string(__idx));
    }
    inline VariableCompleteSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return VariableCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_temp_signed(__idx));
    }
};

template<typename __Configs>
class ConstUnsignedTempGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline ConstUnsignedTempGenerator(uint32_t _idx) : __idx(_idx) {
        if(__idx > 1) throw std::runtime_error("Invalid temp index: " + std::to_string(__idx));
    }
    inline ConstantCompleteUnsignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteUnsignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_temp_unsigned_const(__idx));
    }
};

template<typename __Configs>
class ConstSignedTempGenerator : public ArgumentGenerator<__Configs> {
private:
    uint32_t __idx;

public:
    inline ConstSignedTempGenerator(uint32_t _idx) : __idx(_idx) {
        if(__idx > 1) throw std::runtime_error("Invalid temp index: " + std::to_string(__idx));
    }
    inline ConstantCompleteSignedArgument<__Configs> operator()(Golden<__Configs> & _golden) const {
        return ConstantCompleteSignedArgument<__Configs>(ArgumentGenerator<__Configs>::get_temp_signed_const(__idx));
    }
};

#endif // __ARGUMENT_GENERATOR_HPP__