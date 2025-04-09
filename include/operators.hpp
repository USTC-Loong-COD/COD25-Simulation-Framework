/**
 * @file operators.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __OPERATORS_HPP__
#define __OPERATORS_HPP__

#include "argument.hpp"

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void add(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs + _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void subtract(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs - _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void multiply(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs * _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void multiply_high(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = (_lhs * _rhs) >> 32;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void divide(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs / _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void modulus(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs % _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void bitwise_and(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs & _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void bitwise_or(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs | _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void bitwise_xor(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs ^ _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src>
void bitwise_not(__Dest<__Configs> && _dest, __Src<__Configs> && _lhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src<__Configs>>::value, "Invalid arguments");
    _dest = ~_lhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src>
void logical_not(__Dest<__Configs> && _dest, __Src<__Configs> && _lhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src<__Configs>>::value, "Invalid arguments");
    _dest = !_lhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void less_than(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs < _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void greater_than(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs > _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void less_than_or_equal(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs <= _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void greater_than_or_equal(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs >= _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void equal_to(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs == _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void not_equal_to(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs != _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void shift_left(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs << _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void shift_right(__Dest<__Configs> && _dest, __Src1<__Configs> && _lhs, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    _dest = _lhs >> _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src>
void assign(__Dest<__Configs> && _dest, __Src<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src<__Configs>>::value, "Invalid arguments");
    _dest = _rhs;
}

template<typename __Configs, template<typename> typename __Dest, template<typename> typename __Src1, template<typename> typename __Src2>
void conditional_assign(__Dest<__Configs> && _dest, __Src1<__Configs> && _cond, __Src2<__Configs> && _rhs) {
    static_assert(is_argument<__Configs, __Dest<__Configs>>::value && is_constant_argument<__Configs, __Src1<__Configs>>::value && is_constant_argument<__Configs, __Src2<__Configs>>::value, "Invalid arguments");
    if(!_cond) return;
    _dest = _rhs;
}

#endif // __OPERATORS_HPP__