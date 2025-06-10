#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    template <class L, class R>
    constexpr bool operator()(L &&lhs, R &&rhs) const noexcept(noexcept(std::forward<L>(lhs) < std::forward<R>(rhs))) 
    {
        return std::forward<L>(lhs) < std::forward<R>(rhs);
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    template <class L, class R>
    constexpr bool operator()(L &&lhs, R &&rhs) const noexcept(noexcept(std::forward<L>(lhs) == std::forward<R>(rhs))) 
    {
        return std::forward<L>(lhs) == std::forward<R>(rhs);
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
    std::size_t operator()(const std::string &s) const noexcept { return std::hash<std::string>{}(s); }
};

}  // namespace bookdb
