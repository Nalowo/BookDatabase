#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &iCont) 
{
    std::flat_map<std::string, size_t, Comparator> map;
    map.reserve(iCont.size());
    for (const auto &a : iCont)
        if (auto [it, flag] = map.emplace(std::string(a.author), 1); !flag)
            it->second++;
    return map;
}

template <BookIterator T, typename Comparator = std::less<Genre>>
auto calculateGenreRatings(T iItBegin, T iItEnd) 
{

}
// calculateAverageRating
// sampleRandomBooks
// getTopNBy
}  // namespace bookdb
