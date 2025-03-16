/**
 * @file argument.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.0.0 Beta
 * @date 2025-03-10
 *
 * @copyright Copyright (c) 2025 __NYA__
 *
 */

#ifndef __ARGUMENT_HPP__
#define __ARGUMENT_HPP__

#include <cstdint>
#include <type_traits>

/**
 * @brief class Argument: Base class for arguments
 *
 * CRTP pattern, static polymorphism
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class Argument {
public:
    inline uint32_t get_value_unsigned() const { return static_cast<const __Derived *>(this)->get_value_unsigned_impl(); }
    inline int32_t get_value_signed() const { return static_cast<const __Derived *>(this)->get_value_signed_impl(); }
    template<typename __Derived_>
    inline uint32_t operator+(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->add_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator-(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->subtract_impl(_rhs); }
    template<typename __Derived_>
    inline uint64_t operator*(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->multiply_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator/(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->divide_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator%(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->modulus_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator&(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->and_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator|(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->or_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator^(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->xor_impl(_rhs); }
    inline uint32_t operator~() const { return static_cast<const __Derived *>(this)->not_impl(); }
    inline uint32_t operator!() const { return static_cast<const __Derived *>(this)->logical_not_impl(); }
    template<typename __Derived_>
    inline uint32_t operator<(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->less_than_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator>(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->greater_than_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator<=(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->less_than_or_equal_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator>=(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->greater_than_or_equal_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator==(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->equal_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator!=(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->not_equal_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator<<(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->shift_left_impl(_rhs); }
    template<typename __Derived_>
    inline uint32_t operator>>(const Argument<__Configs, __Derived_> & _rhs) const { return static_cast<const __Derived *>(this)->shift_right_impl(_rhs); }
    template<typename __Derived_>
    inline __Derived & operator=(const Argument<__Configs, __Derived_> & _rhs) { return static_cast<__Derived &>(static_cast<__Derived *>(this)->assign_impl(_rhs)); }
    inline __Derived & operator=(uint32_t _rhs) { return static_cast<__Derived &>(static_cast<__Derived *>(this)->assign_impl(_rhs)); }
};

/**
 * @brief class CommonArgumentImpl: Implementation of common operations for arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class CommonArgumentImpl {
protected:
    template<typename __Derived_>
    inline uint32_t add_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() + _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t subtract_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() - _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t and_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() & _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t or_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() | _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t xor_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() ^ _rhs.get_value_unsigned();
    }
    inline uint32_t not_impl() const {
        return ~static_cast<const __Derived *>(this)->get_value_unsigned();
    }
    inline uint32_t logical_not_impl() const {
        return !static_cast<const __Derived *>(this)->get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t shift_left_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() << (_rhs.get_value_unsigned() & 0x1F);
    }
};

/**
 * @brief class SignedArgumentImpl: Implementation of functions those differs for signed and unsigned arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class SignedArgumentImpl {
protected:
    template<typename __Derived_>
    inline uint64_t multiply_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<int64_t>(static_cast<const __Derived *>(this)->get_value_signed()) * static_cast<int64_t>(_rhs.get_value_signed());
    }
    template<typename __Derived_>
    inline uint32_t divide_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() / _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t modulus_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() % _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t less_than_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() < _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t greater_than_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() > _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t less_than_or_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() <= _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t greater_than_or_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() >= _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() == _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t not_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() != _rhs.get_value_signed();
    }
    template<typename __Derived_>
    inline uint32_t shift_right_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_signed() >> (_rhs.get_value_unsigned() & 0x1F);
    }
};

/**
 * @brief class UnsignedArgumentImpl: Implementation of functions those differs for signed and unsigned arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class UnsignedArgumentImpl {
protected:
    template<typename __Derived_>
    inline uint64_t multiply_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<uint64_t>(static_cast<const __Derived *>(this)->get_value_unsigned()) * static_cast<uint64_t>(_rhs.get_value_unsigned());
    }
    template<typename __Derived_>
    inline uint32_t divide_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() / _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t modulus_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() % _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t less_than_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() < _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t greater_than_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() > _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t less_than_or_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() <= _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t greater_than_or_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() >= _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() == _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t not_equal_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() != _rhs.get_value_unsigned();
    }
    template<typename __Derived_>
    inline uint32_t shift_right_impl(const Argument<__Configs, __Derived_> & _rhs) const {
        return static_cast<const __Derived *>(this)->get_value_unsigned() >> (_rhs.get_value_unsigned() & 0x1F);
    }
};

/**
 * @brief class ConstantArgumentImpl: Tagging class for constant arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class ConstantArgumentImpl {
private:
    uint32_t __value_holder;

protected:
    using is_constant = std::true_type;

    uint32_t __get_raw_value() const { return __value_holder; }

public:
    inline ConstantArgumentImpl(uint32_t _value) : __value_holder(_value) {}
};

/**
 * @brief class VariableArgumentImpl: Tagging class for variable arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class VariableArgumentImpl {
private:
    uint32_t & __value_holder;

protected:
    using is_constant = std::false_type;

    uint32_t __get_raw_value() const { return __value_holder; }
    uint32_t & __get_raw_reference() { return __value_holder; }

public:
    inline VariableArgumentImpl(uint32_t & _value) : __value_holder(_value) {}
};

/**
 * @brief class PartialArgumentImpl: Implementation of partial arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class PartialArgumentImpl {
private:
    uint32_t __bits, __bit_offset;

protected:
    inline uint32_t get_value_unsigned_impl() const {
        if(__bits == 32) return static_cast<const __Derived *>(this)->__get_raw_value();
        return (static_cast<const __Derived *>(this)->__get_raw_value() >> __bit_offset) & ((1 << __bits) - 1);
    }
    inline int32_t get_value_signed_impl() const {
        if(__bits == 32) return static_cast<const __Derived *>(this)->__get_raw_value();
        return (this->get_value_unsigned_impl() ^ (1 << (__bits - 1))) - (1 << (__bits - 1));
    }
    template<typename __Derived_>
    inline __Derived & assign_impl(const Argument<__Configs, __Derived_> & _rhs) {
        if constexpr(!__Derived::is_constant::value) {
            if(__bits == 32) static_cast<__Derived *>(this)->__get_raw_reference() = _rhs.get_value_unsigned();
            else static_cast<__Derived *>(this)->__get_raw_reference() = (static_cast<const __Derived *>(this)->__get_raw_value() & ~(((1 << __bits) - 1) << __bit_offset)) | ((_rhs.get_value_unsigned() & ((1 << __bits) - 1)) << __bit_offset);
        }
        return *static_cast<__Derived *>(this);
    }
    inline __Derived & assign_impl(uint32_t _rhs) {
        if constexpr(!__Derived::is_constant::value) {
            if(__bits == 32) static_cast<__Derived *>(this)->__get_raw_reference() = _rhs;
            else static_cast<__Derived *>(this)->__get_raw_reference() = (static_cast<const __Derived *>(this)->__get_raw_value() & ~(((1 << __bits) - 1) << __bit_offset)) | ((_rhs & ((1 << __bits) - 1)) << __bit_offset);
        }
        return *static_cast<__Derived *>(this);
    }

public:
    inline PartialArgumentImpl(uint32_t _bits, uint32_t _bit_offset) : __bits(_bits), __bit_offset(_bit_offset) {}
};

/**
 * @brief class CompleteArgumentImpl: Implementation of complete arguments
 *
 * @tparam __Configs
 * @tparam __Derived
 */
template<typename __Configs, typename __Derived>
class CompleteArgumentImpl {
protected:
    inline uint32_t get_value_unsigned_impl() const {
        return static_cast<const __Derived *>(this)->__get_raw_value();
    }
    inline int32_t get_value_signed_impl() const {
        return static_cast<const __Derived *>(this)->__get_raw_value();
    }
    template<typename __Derived_>
    inline __Derived & assign_impl(const Argument<__Configs, __Derived_> & _rhs) {
        if constexpr(!__Derived::is_constant::value) static_cast<__Derived *>(this)->__get_raw_reference() = _rhs.get_value_unsigned();
        return *static_cast<__Derived *>(this);
    }
    inline __Derived & assign_impl(uint32_t _rhs) {
        if constexpr(!__Derived::is_constant::value) static_cast<__Derived *>(this)->__get_raw_reference() = _rhs;
        return *static_cast<__Derived *>(this);
    }
};

template<typename __Configs>
class ConstantCompleteUnsignedArgument
    : public Argument<__Configs, ConstantCompleteUnsignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>,
      public UnsignedArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>,
      public ConstantArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>,
      public CompleteArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>> {
    friend Argument<__Configs, ConstantCompleteUnsignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>;
    // friend UnsignedArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>;
    // friend ConstantArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>;
    friend CompleteArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>;

public:
    inline ConstantCompleteUnsignedArgument(uint32_t _value) : ConstantArgumentImpl<__Configs, ConstantCompleteUnsignedArgument<__Configs>>(_value) {}

    using Argument<__Configs, ConstantCompleteUnsignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class VariableCompleteUnsignedArgument
    : public Argument<__Configs, VariableCompleteUnsignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>,
      public UnsignedArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>,
      public VariableArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>,
      public CompleteArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>> {
    friend Argument<__Configs, VariableCompleteUnsignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>;
    // friend UnsignedArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>;
    // friend VariableArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>;
    friend CompleteArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>;

public:
    inline VariableCompleteUnsignedArgument(uint32_t & _value) : VariableArgumentImpl<__Configs, VariableCompleteUnsignedArgument<__Configs>>(_value) {}

    using Argument<__Configs, VariableCompleteUnsignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class ConstantPartialUnsignedArgument
    : public Argument<__Configs, ConstantPartialUnsignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>,
      public UnsignedArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>,
      public ConstantArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>,
      public PartialArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>> {
    friend Argument<__Configs, ConstantPartialUnsignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>;
    // friend UnsignedArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>;
    // friend ConstantArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>;
    friend PartialArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>;

public:
    inline ConstantPartialUnsignedArgument(uint32_t _value, uint32_t _bits, uint32_t _bit_offset) : ConstantArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>(_value), PartialArgumentImpl<__Configs, ConstantPartialUnsignedArgument<__Configs>>(_bits, _bit_offset) {}

    using Argument<__Configs, ConstantPartialUnsignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class VariablePartialUnsignedArgument
    : public Argument<__Configs, VariablePartialUnsignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>,
      public UnsignedArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>,
      public VariableArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>,
      public PartialArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>> {
    friend Argument<__Configs, VariablePartialUnsignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>;
    // friend UnsignedArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>;
    // friend VariableArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>;
    friend PartialArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>;

public:
    inline VariablePartialUnsignedArgument(uint32_t & _value, uint32_t _bits, uint32_t _bit_offset) : VariableArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>(_value), PartialArgumentImpl<__Configs, VariablePartialUnsignedArgument<__Configs>>(_bits, _bit_offset) {}

    using Argument<__Configs, VariablePartialUnsignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class ConstantCompleteSignedArgument
    : public Argument<__Configs, ConstantCompleteSignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>,
      public SignedArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>,
      public ConstantArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>,
      public CompleteArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>> {
    friend Argument<__Configs, ConstantCompleteSignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>;
    // friend SignedArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>;
    // friend ConstantArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>;
    friend CompleteArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>;

public:
    inline ConstantCompleteSignedArgument(uint32_t _value) : ConstantArgumentImpl<__Configs, ConstantCompleteSignedArgument<__Configs>>(_value) {}

    using Argument<__Configs, ConstantCompleteSignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class VariableCompleteSignedArgument
    : public Argument<__Configs, VariableCompleteSignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>,
      public SignedArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>,
      public VariableArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>,
      public CompleteArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>> {
    friend Argument<__Configs, VariableCompleteSignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>;
    // friend SignedArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>;
    // friend VariableArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>;
    friend CompleteArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>;

public:
    inline VariableCompleteSignedArgument(uint32_t & _value) : VariableArgumentImpl<__Configs, VariableCompleteSignedArgument<__Configs>>(_value) {}

    using Argument<__Configs, VariableCompleteSignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class ConstantPartialSignedArgument
    : public Argument<__Configs, ConstantPartialSignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>,
      public SignedArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>,
      public ConstantArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>,
      public PartialArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>> {
    friend Argument<__Configs, ConstantPartialSignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>;
    // friend SignedArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>;
    // friend ConstantArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>;
    friend PartialArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>;

public:
    inline ConstantPartialSignedArgument(uint32_t _value, uint32_t _bits, uint32_t _bit_offset) : ConstantArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>(_value), PartialArgumentImpl<__Configs, ConstantPartialSignedArgument<__Configs>>(_bits, _bit_offset) {}

    using Argument<__Configs, ConstantPartialSignedArgument<__Configs>>::operator=;
};

template<typename __Configs>
class VariablePartialSignedArgument
    : public Argument<__Configs, VariablePartialSignedArgument<__Configs>>,
      public CommonArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>,
      public SignedArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>,
      public VariableArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>,
      public PartialArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>> {
    friend Argument<__Configs, VariablePartialSignedArgument<__Configs>>;
    // friend CommonArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>;
    // friend SignedArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>;
    // friend VariableArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>;
    friend PartialArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>;

public:
    inline VariablePartialSignedArgument(uint32_t _value, uint32_t _bits, uint32_t _bit_offset) : VariableArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>(_value), PartialArgumentImpl<__Configs, VariablePartialSignedArgument<__Configs>>(_bits, _bit_offset) {}

    using Argument<__Configs, VariablePartialSignedArgument<__Configs>>::operator=;
};

template<typename __Configs, typename __Tp>
using is_argument = std::bool_constant<
    std::is_same_v<__Tp, ConstantCompleteUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, VariableCompleteUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantPartialUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, VariablePartialUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantCompleteSignedArgument<__Configs>>
    || std::is_same_v<__Tp, VariableCompleteSignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantPartialSignedArgument<__Configs>>
    || std::is_same_v<__Tp, VariablePartialSignedArgument<__Configs>>>;

template<typename __Configs, typename __Tp>
using is_constant_argument = std::bool_constant<
    std::is_same_v<__Tp, ConstantCompleteUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantPartialUnsignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantCompleteSignedArgument<__Configs>>
    || std::is_same_v<__Tp, ConstantPartialSignedArgument<__Configs>>>;

#endif // __ARGUMENT_HPP__