/**
 * @file isa.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-09
 *
 * @cOperationyright COperationyright (c) 2025
 *
 */

#ifndef __ISA_HPP__
#define __ISA_HPP__

#include "argument_generator.hpp"
#include "enums.hpp"
#include "operation.hpp"
#include "operators.hpp"
#include <cstdint>
#include <memory>
#include <vector>

template<typename __Configs, IsaType __isa_type>
class IsaImpl;

template<typename __Configs>
class IsaImpl<__Configs, IsaType::LOONGARCH> {
    template<typename>
    friend class Isa;

private:
    template<uint32_t __mask>
    inline static bool check(uint32_t _instr) {
        return _instr & __mask;
    }

    template<uint32_t __bits, uint32_t __offset>
    inline static uint32_t extract(uint32_t _instr) {
        return (_instr >> __offset) & ((1 << __bits) - 1);
    }

    template<uint32_t __bits>
    inline static uint32_t sign_extend(uint32_t _value) {
        return (_value ^ (1 << (__bits - 1))) - (1 << (__bits - 1));
    }

    inline static std::vector<std::unique_ptr<BaseOperation<__Configs>>> decode(uint32_t _instr) {
        auto res = std::vector<std::unique_ptr<BaseOperation<__Configs>>>();
        if(check<0x80000000>(_instr)) {
            // halt
            res.push_back(make_operation(assign, HaltGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            return res;
        }
        if(check<0x40000000>(_instr)) {
            // B & J
            switch(extract<6, 26>(_instr)) {
            case 0B010011: {
                // jirl
                auto rd = extract<5, 0>(_instr);
                auto rs = extract<5, 5>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(add, NpcGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(4)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B010100: {
                // b
                auto offset = sign_extend<26>(extract<16, 10>(_instr) | (extract<10, 0>(_instr) << 16)) << 2;
                res.push_back(make_operation(add, NpcGenerator<__Configs>(), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                return res;
            }
            case 0B010101: {
                // bl
                auto rd = 1U;
                auto offset = sign_extend<26>(extract<16, 10>(_instr) | (extract<10, 0>(_instr) << 16)) << 2;
                res.push_back(make_operation(add, NpcGenerator<__Configs>(), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(4)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B010110: {
                // beq
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(equal_to, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B010111: {
                // bne
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(not_equal_to, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B011000: {
                // blt
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(less_than, UnsignedTempGenerator<__Configs>(0), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B011001: {
                // bge
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(greater_than_or_equal, UnsignedTempGenerator<__Configs>(0), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B011010: {
                // bltu
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(less_than, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B011011: {
                // bgeu
                auto rs1 = extract<5, 5>(_instr);
                auto rs2 = extract<5, 0>(_instr);
                auto offset = sign_extend<16>(extract<16, 10>(_instr)) << 2;
                res.push_back(make_operation(greater_than_or_equal, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            }
            return res;
        }
        if(check<0x20000000>(_instr)) {
            // S & L
            auto rs1 = extract<5, 5>(_instr);
            auto rs2 = extract<5, 0>(_instr);
            auto offset = sign_extend<12>(extract<12, 10>(_instr));
            switch(extract<7, 22>(_instr)) {
            case 0B0100000: {
                // ld.b
                res.push_back(make_operation(assign, SignedRegisterGenerator<__Configs>(rs2), ConstSignedDataMemGenerator<__Configs>(rs1, offset, 8)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstSignedRegisterGenerator<__Configs>(rs2)));
                return res;
            }
            case 0B0100001: {
                // ld.h
                res.push_back(make_operation(assign, SignedRegisterGenerator<__Configs>(rs2), ConstSignedDataMemGenerator<__Configs>(rs1, offset, 16)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstSignedRegisterGenerator<__Configs>(rs2)));
                return res;
            }
            case 0B0100010: {
                // ld.w
                res.push_back(make_operation(assign, SignedRegisterGenerator<__Configs>(rs2), ConstSignedDataMemGenerator<__Configs>(rs1, offset, 32)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstSignedRegisterGenerator<__Configs>(rs2)));
                return res;
            }
            case 0B0101000: {
                // ld.bu
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rs2), ConstUnsignedDataMemGenerator<__Configs>(rs1, offset, 8)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstSignedRegisterGenerator<__Configs>(rs2)));
                return res;
            }
            case 0B0101001: {
                // ld.hu
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rs2), ConstUnsignedDataMemGenerator<__Configs>(rs1, offset, 16)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstSignedRegisterGenerator<__Configs>(rs2)));
                return res;
            }
            case 0B0100100: {
                // st.b
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, offset, 8), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, offset, 32)));
                return res;
            }
            case 0B0100101: {
                // st.h
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, offset, 16), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, offset, 32)));
                return res;
            }
            case 0B0100110: {
                // st.w
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, offset, 32), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(offset)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, offset, 32)));
                return res;
            }
            }
            return res;
        }
        if(check<0x10000000>(_instr)) {
            // U
            auto rd = extract<5, 0>(_instr);
            auto imm = extract<20, 5>(_instr) << 12;

            switch(extract<2, 26>(_instr)) {
            case 0B01: {
                // lu12i.w
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), UnsignedImmGenerator<__Configs>(imm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(imm)));
                return res;
            }
            case 0B11: {
                // pcaddu12i
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(imm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
            return res;
        }
        if(check<0x02000000>(_instr)) {
            // I
            auto simm = sign_extend<12>(extract<12, 10>(_instr));
            auto uimm = extract<12, 10>(_instr);
            auto rs = extract<5, 5>(_instr);
            auto rd = extract<5, 0>(_instr);
            switch(extract<3, 22>(_instr)) {
            case 0B000: {
                // slti
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B001: {
                // sltui
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B010: {
                // addi.w
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B101: {
                // andi
                res.push_back(make_operation(bitwise_and, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(uimm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B110: {
                // ori
                res.push_back(make_operation(bitwise_or, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(uimm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B111: {
                // xori
                res.push_back(make_operation(bitwise_xor, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(uimm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
        }
        if(check<0x00400000>(_instr)) {
            // I (shift)
            auto shift_amount = extract<5, 10>(_instr);
            auto rs = extract<5, 5>(_instr);
            auto rd = extract<5, 0>(_instr);

            switch(extract<2, 18>(_instr)) {
            case 0B00: {
                // slli.w
                res.push_back(make_operation(shift_left, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(shift_amount)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01: {
                // srli.w
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(shift_amount)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B10: {
                // srai.w
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs), UnsignedImmGenerator<__Configs>(shift_amount)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
        }
        if(check<0x00200000>(_instr)) {
            // R (divide)
            auto rs1 = extract<5, 5>(_instr);
            auto rs2 = extract<5, 10>(_instr);
            auto rd = extract<5, 0>(_instr);

            switch(extract<2, 15>(_instr)) {
            case 0B00: {
                // div.w
                res.push_back(make_operation(divide, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01: {
                // mod.w
                res.push_back(make_operation(modulus, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B10: {
                // div.wu
                res.push_back(make_operation(divide, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B11: {
                // mod.wu
                res.push_back(make_operation(modulus, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
            return res;
        }
        if(check<0x00100000>(_instr)) {
            // R

            auto rs1 = extract<5, 5>(_instr);
            auto rs2 = extract<5, 10>(_instr);
            auto rd = extract<5, 0>(_instr);

            switch(extract<5, 15>(_instr)) {
            case 0B00000: {
                // add.w
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00010: {
                // sub.w
                res.push_back(make_operation(subtract, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00100: {
                // slt
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00101: {
                // sltu
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01000: {
                // nor
                res.push_back(make_operation(bitwise_or, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(bitwise_not, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedTempGenerator<__Configs>(0)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01001: {
                // and
                res.push_back(make_operation(bitwise_and, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01010: {
                // or
                res.push_back(make_operation(bitwise_or, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01011: {
                // xor
                res.push_back(make_operation(bitwise_xor, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01110: {
                // sll.w
                res.push_back(make_operation(shift_left, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01111: {
                // srl.w
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B10000: {
                // sra.w
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B11000: {
                // mul.w
                res.push_back(make_operation(multiply, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B11001: {
                // mulh.w
                res.push_back(make_operation(multiply_high, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B11010: {
                // mulh.wu
                res.push_back(make_operation(multiply_high, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
            return res;
        }
        return res;
    }
};

template<typename __Configs>
class IsaImpl<__Configs, IsaType::RISC_V> {
    template<typename>
    friend class Isa;

private:
    template<uint32_t __mask, uint32_t __value>
    inline static bool check(uint32_t _instr) {
        return (_instr & __mask) == __value;
    }

    template<uint32_t __bits, uint32_t __offset>
    inline static uint32_t extract(uint32_t _instr) {
        return (_instr >> __offset) & ((1 << __bits) - 1);
    }

    template<uint32_t __bits>
    inline static uint32_t sign_extend(uint32_t _value) {
        return (_value ^ (1 << (__bits - 1))) - (1 << (__bits - 1));
    }

    inline static std::vector<std::unique_ptr<BaseOperation<__Configs>>> decode(uint32_t _instr) {
        auto res = std::vector<std::unique_ptr<BaseOperation<__Configs>>>();
        if(check<0xFFFFFFFF, 0x00100073>(_instr)) {
            // halt
            // halt = 1

            // halt = 1
            res.push_back(make_operation(assign, HaltGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            return res;
        }
        if(check<0x0000007F, 0x00000033>(_instr)) {
            // R

            auto rd = extract<5, 7>(_instr);
            auto rs1 = extract<5, 15>(_instr);
            auto rs2 = extract<5, 20>(_instr);
            auto alu_op = (extract<1, 30>(_instr) << 4) | (extract<1, 25>(_instr) << 3) | (extract<3, 12>(_instr));

            switch(alu_op) {
            case 0B00000: {
                // add
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B10000: {
                // sub
                res.push_back(make_operation(subtract, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00001: {
                // sll
                res.push_back(make_operation(shift_left, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00101: {
                // srl
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B10101: {
                // sra
                res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00010: {
                // slt
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00011: {
                // sltu
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00100: {
                // xor
                res.push_back(make_operation(bitwise_xor, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00110: {
                // or
                res.push_back(make_operation(bitwise_or, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B00111: {
                // and
                res.push_back(make_operation(bitwise_and, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01000: {
                // mul
                res.push_back(make_operation(multiply, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01001: {
                // mulh
                res.push_back(make_operation(multiply_high, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01011: {
                // mulhu
                res.push_back(make_operation(multiply_high, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01100: {
                // div
                res.push_back(make_operation(divide, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01101: {
                // divu
                res.push_back(make_operation(divide, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01110: {
                // rem
                res.push_back(make_operation(modulus, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B01111: {
                // remu
                res.push_back(make_operation(modulus, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
            return res;
        }
        if(check<0x0000007F, 0x00000013>(_instr)) {
            // I

            auto rd = extract<5, 7>(_instr);
            auto rs1 = extract<5, 15>(_instr);
            auto uimm = extract<12, 20>(_instr);
            auto simm = sign_extend<12>(uimm);
            auto alu_op = extract<3, 12>(_instr);

            switch(alu_op) {
            case 0B000: {
                // addi
                res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B001: {
                // slli
                res.push_back(make_operation(shift_left, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(uimm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B010: {
                // slti
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B011: {
                // sltiu
                res.push_back(make_operation(less_than, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B110: {
                // ori
                res.push_back(make_operation(bitwise_or, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B111: {
                // andi
                res.push_back(make_operation(bitwise_and, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B100: {
                // xori
                res.push_back(make_operation(bitwise_xor, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B101: {
                if(check<0xFE000000, 0x40000000>(_instr)) {
                    // srai
                    res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstSignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(uimm)));
                    res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                    res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                    res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                }
                else {
                    // srli
                    res.push_back(make_operation(shift_right, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(uimm)));
                    res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                    res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                    res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                }
                return res;
            }
            }
            return res;
        }
        if(check<0x0000007F, 0x00000003>(_instr)) {
            // I (load)

            auto rd = extract<5, 7>(_instr);
            auto rs1 = extract<5, 15>(_instr);
            auto simm = sign_extend<12>(extract<12, 20>(_instr));
            auto load_op = extract<3, 12>(_instr);

            switch(load_op) {
            case 0B000: {
                // lb
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 8)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B001: {
                // lh
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 16)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B010: {
                // lw
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 32)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B100: {
                // lbu
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 8)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            case 0B101: {
                // lhu
                res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 16)));
                res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
                res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
                return res;
            }
            }
        }
        if(check<0x0000007F, 0x00000023>(_instr)) {
            // S

            auto rs1 = extract<5, 15>(_instr);
            auto rs2 = extract<5, 20>(_instr);
            auto simm = sign_extend<12>((extract<7, 25>(_instr) << 5) | extract<5, 7>(_instr));
            auto store_op = extract<3, 12>(_instr);

            switch(store_op) {
            // sb sh sw
            case 0B000: {
                // sb
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, simm, 8), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 32)));
                return res;
            }
            case 0B001: {
                // sh
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, simm, 16), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 32)));
                return res;
            }
            case 0B010: {
                // sw
                res.push_back(make_operation(assign, UnsignedDataMemGenerator<__Configs>(rs1, simm, 32), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(assign, MemWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(subtract, UnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(__Configs::data_mem_start)));
                res.push_back(make_operation(shift_right, MemWaGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(2)));
                res.push_back(make_operation(assign, MemWdGenerator<__Configs>(), ConstUnsignedDataMemGenerator<__Configs>(rs1, simm, 32)));
                return res;
            }
            }
            return res;
        }
        if(check<0x0000007F, 0x00000037>(_instr)) {
            // U

            auto rd = extract<5, 7>(_instr);
            auto uimm = extract<20, 12>(_instr);

            // lui
            res.push_back(make_operation(assign, UnsignedRegisterGenerator<__Configs>(rd), UnsignedImmGenerator<__Configs>(uimm << 12)));
            res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
            res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
            return res;
        }
        if(check<0x0000007F, 0x00000017>(_instr)) {
            // U

            auto rd = extract<5, 7>(_instr);
            auto uimm = extract<20, 12>(_instr);

            // auipc
            res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(uimm << 12)));
            res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
            res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
            return res;
        }
        if(check<0x0000007F, 0x00000063>(_instr)) {
            // B

            auto rs1 = extract<5, 15>(_instr);
            auto rs2 = extract<5, 20>(_instr);
            auto simm = sign_extend<13>((extract<1, 31>(_instr) << 12) | (extract<1, 7>(_instr) << 11) | (extract<6, 25>(_instr) << 5) | (extract<4, 8>(_instr) << 1));
            auto branch_op = extract<3, 12>(_instr);

            switch(branch_op) {
            case 0B000: {
                // beq
                res.push_back(make_operation(equal_to, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B001: {
                // bne
                res.push_back(make_operation(not_equal_to, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B100: {
                // blt
                res.push_back(make_operation(less_than, UnsignedTempGenerator<__Configs>(0), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B101: {
                // bge
                res.push_back(make_operation(greater_than_or_equal, UnsignedTempGenerator<__Configs>(0), ConstSignedRegisterGenerator<__Configs>(rs1), ConstSignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B110: {
                // bltu
                res.push_back(make_operation(less_than, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            case 0B111: {
                // bgeu
                res.push_back(make_operation(greater_than_or_equal, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), ConstUnsignedRegisterGenerator<__Configs>(rs2)));
                res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(1), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
                res.push_back(make_operation(conditional_assign, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), ConstUnsignedTempGenerator<__Configs>(1)));
                return res;
            }
            }
        }
        if(check<0x0000007F, 0x00000067>(_instr)) {
            // I (jalr)

            auto rd = extract<5, 7>(_instr);
            auto rs1 = extract<5, 15>(_instr);
            auto simm = sign_extend<12>(extract<12, 20>(_instr));

            // jalr
            res.push_back(make_operation(add, UnsignedTempGenerator<__Configs>(0), ConstUnsignedRegisterGenerator<__Configs>(rs1), SignedImmGenerator<__Configs>(simm)));
            res.push_back(make_operation(bitwise_and, NpcGenerator<__Configs>(), ConstUnsignedTempGenerator<__Configs>(0), UnsignedImmGenerator<__Configs>(~1)));
            res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(4)));
            res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
            res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
            return res;
        }
        if(check<0x0000007F, 0x0000006F>(_instr)) {
            // J

            auto rd = extract<5, 7>(_instr);
            auto simm = sign_extend<21>((extract<1, 31>(_instr) << 20) | (extract<8, 12>(_instr) << 12) | (extract<1, 20>(_instr) << 11) | (extract<10, 21>(_instr) << 1));

            // jal
            res.push_back(make_operation(add, UnsignedRegisterGenerator<__Configs>(rd), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(4)));
            res.push_back(make_operation(add, NpcGenerator<__Configs>(), ConstPcGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(simm)));
            res.push_back(make_operation(assign, RegWeGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(1)));
            res.push_back(make_operation(assign, RegWaGenerator<__Configs>(), UnsignedImmGenerator<__Configs>(rd)));
            res.push_back(make_operation(assign, RegWdGenerator<__Configs>(), ConstUnsignedRegisterGenerator<__Configs>(rd)));
            return res;
        }
        return res;
    }
};

template<typename __Configs>
class Isa {
    using Impl = IsaImpl<__Configs, __Configs::isa_type>;

public:
    inline static std::vector<std::unique_ptr<BaseOperation<__Configs>>> decode(uint32_t _instr) {
        return Impl::decode(_instr);
    }
};

#endif // __ISA_HPP__
