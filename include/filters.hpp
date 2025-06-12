#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
auto YearBetween(int iStart, int iEnd) {
    return [iStart, iEnd](const Book &book) { return iStart <= book.year && book.year <= iEnd; };
}

auto RatingAbove(double iMinRating) {
    return [iMinRating](const Book &book) { return book.rating > iMinRating; };
}

auto GenreIs(Genre iGenre) {
    return [iGenre](const Book &book) { return book.genre == iGenre; };
}

template <typename... Predicates>
auto all_of(Predicates &&...preds) {
    return [preds...](const Book &book) { return (preds(book) && ...); };
}

template <typename... Predicates>
auto any_of(Predicates &&...preds) {
    return [preds...](const Book &book) { return (preds(book) || ...); };
}

template <ConstBookIterator T>
std::vector<std::reference_wrapper<const Book>> filterBooks(T begin, T end, auto &&cmp) {

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(std::distance(begin, end));

    std::copy_if(begin, end, std::back_inserter(result), [&cmp](const auto &book) { return cmp(book); });
    return result;
}
}  // namespace bookdb