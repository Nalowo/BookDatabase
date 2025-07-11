#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {
using ContainedType = bookdb::Book;

template <typename T>
concept BookIterator = std::same_as<ContainedType, typename T::value_type> && std::bidirectional_iterator<T>;

template <typename Iter>
concept ConstBookIterator = std::bidirectional_iterator<Iter> && requires(Iter it) {
    {
        static_cast<const typename std::iter_value_t<Iter> &>(*it)
    } -> std::same_as<const typename std::iter_value_t<Iter> &>;
};

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P pred) {
    { pred(std::declval<ContainedType>()) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comp) {
    { comp(std::declval<ContainedType>(), std::declval<ContainedType>()) } -> std::convertible_to<bool>;
};

template <typename T>
concept BookContainerLike = requires(T cont) {
    requires std::same_as<ContainedType, typename T::value_type>;
    requires BookIterator<typename T::iterator>;
    requires ConstBookIterator<typename T::const_iterator>;
    typename T::reference;
    typename T::reverse_iterator;
    { cont.begin() } -> std::same_as<typename T::iterator>;
    { cont.end() } -> std::same_as<typename T::iterator>;
    { cont.cbegin() } -> std::same_as<typename T::const_iterator>;
    { cont.cend() } -> std::same_as<typename T::const_iterator>;
    { cont.emplace_back(std::declval<typename T::value_type>()) } -> std::same_as<typename T::reference>;
    { cont.empty() } -> std::convertible_to<bool>;
    { cont.size() } -> std::convertible_to<size_t>;
    cont.push_back(std::declval<typename T::value_type>());
};

template <typename C>
concept HasKeyAndMapped = requires {
    typename C::key_container_type;
    typename C::mapped_container_type;
};

template <typename T, typename U>
concept EqualityComparable = requires(const T &a, const U &b) {
    { a == b } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept LessThanComparable = requires(const T &a, const U &b) {
    { a < b } -> std::convertible_to<bool>;
};
}  // namespace bookdb