/**
 * @file operation.hpp
 * @author USTC 2025 Spring COD LJL TA Group
 * @version 2.1.0
 * @date 2025-03-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __OPERATION_HPP__
#define __OPERATION_HPP__

#include <functional>
#include <memory>
#include <tuple>
#include <utility>

template<typename __Configs>
class Golden;

template<typename __Configs>
class BaseOperation {
public:
    virtual void operator()(Golden<__Configs> & _golden) const = 0;
    inline virtual ~BaseOperation() {}
};

// __Tps<__Configs> : public ArgumentGenerator<__Configs>
template<typename __Configs, template<typename> class... __Tps>
class Operation : public BaseOperation<__Configs> {
private:
    // std::declval<__Tps<__Configs>>() -> ArgumentGenerator<__Configs>()
    // ArgumentGenerator<__Configs>()(golden) -> Argument<__Configs> &&
    // __func_t -> void (Argument && ... args)
    // parameters of __func_t are rvalue references
    using __func_t = void(decltype(std::declval<__Tps<__Configs>>()(std::declval<Golden<__Configs> &>())) &&...);

    std::tuple<__Tps<__Configs>...> __args;
    std::function<__func_t> __func;

public:
    Operation(__func_t * _func, const __Tps<__Configs> &... _args) : __args(_args...), __func(_func) {}

    void operator()(Golden<__Configs> & _golden) const {
        std::apply([this, &_golden](auto &&... _args) { __func(_args(_golden)...); }, __args);
    }

    // for debugging
    // using func_t = __func_t;
    // using types_t = std::tuple<__Tps<__Configs>...>;
};

template<typename __Configs, template<typename> class... __Tps>
std::unique_ptr<Operation<__Configs, __Tps...>> make_operation(void (*_func)(decltype(std::declval<__Tps<__Configs>>()(std::declval<Golden<__Configs> &>())) &&...), const __Tps<__Configs> &... _args) {
    return std::unique_ptr<Operation<__Configs, __Tps...>>(new Operation(_func, _args...));
}

#endif // __OPERATION_HPP__