#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "concepts.hpp"

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    template <typename L, typename R>
        requires EqualityComparable<L, R>
    constexpr bool operator()(const L &lhs, const R &rhs) const noexcept(noexcept(lhs < rhs)) {
        return lhs < rhs;
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    template <typename L, typename R>
        requires LessThanComparable<L, R>
    constexpr bool operator()(const L &lhs, const R &rhs) const noexcept(noexcept(lhs == rhs)) {
        return lhs == rhs;
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }

    std::size_t operator()(const std::string &s) const noexcept { return std::hash<std::string>{}(s); }

    std::size_t operator()(const char *s) const noexcept { return std::hash<std::string_view>{}(s); }
};

}  // namespace bookdb
