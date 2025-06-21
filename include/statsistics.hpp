#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <random>
#include <span>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

namespace bookdb {
namespace details {
template <HasKeyAndMapped C>
void ReserveSpaceInFlatCont(C &cont, size_t iSize) {
    if (iSize == 0)
        return;

    using KeyCont = typename C::key_container_type;
    using ValueCont = typename C::mapped_container_type;

    const KeyCont &keys_ref = cont.keys();
    const ValueCont &vals_ref = cont.values();

    const_cast<KeyCont &>(keys_ref).reserve(iSize);
    const_cast<ValueCont &>(vals_ref).reserve(iSize);
}
}  // namespace details

template <typename Comparator>
using HistogramFlatCont = std::flat_map<std::string, size_t, Comparator>;
template <typename Comparator>
using GenreRatingsFlatCont = std::flat_map<Genre, double, Comparator>;

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &iCont) {
    HistogramFlatCont<Comparator> res;
    details::ReserveSpaceInFlatCont(res, iCont.GetAuthors().size());
    for (const auto &a : iCont)
        ++res[std::string(a.author)];
    return res;
}

template <ConstBookIterator T, typename Comparator = std::less<Genre>>
auto calculateGenreRatings(T iItBegin, T iItEnd) {
    GenreRatingsFlatCont<Comparator> res;
    details::ReserveSpaceInFlatCont(res, static_cast<size_t>(Genre::Unknown) + 1);

    std::array<std::pair<double, size_t>, static_cast<size_t>(Genre::Unknown) + 1> tmp{};
    for (auto it = iItBegin; it != iItEnd; ++it) {
        auto &[totalRating, count] = tmp[static_cast<size_t>(it->genre)];
        ++count;
        totalRating += it->rating;
    }

    for (size_t i = 0; i < tmp.size(); ++i) {
        auto [totalRating, count] = tmp[i];
        res[static_cast<Genre>(i)] = count ? totalRating / count : 0.;
    }

    return res;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    const double sum = std::transform_reduce(cont.cbegin(), cont.cend(), 0.0, std::plus<>(),
                                             [](const auto &item) { return item.rating; });
    return !cont.empty() ? sum / cont.size() : 0.0;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &iCont, size_t N) {
    std::vector<std::reference_wrapper<const Book>> res;
    if (iCont.size() < N)
        throw std::runtime_error{"iCont.size() < N"};

    res.reserve(N);
    std::sample(iCont.begin(), iCont.end(), std::back_inserter(res), N, std::mt19937{std::random_device{}()});
    return res;
}

template <BookContainerLike T, BookComparator Comparator>
std::span<const typename BookDatabase<T>::value_type> getTopNBy(BookDatabase<T> &iCont, size_t N, Comparator comp) {
    if (iCont.size() < N)
        throw std::runtime_error{"iCont.size() < N"};

    std::nth_element(iCont.begin(), iCont.begin() + N, iCont.end(), comp);
    auto tmp = std::span<typename BookDatabase<T>::value_type>(iCont.begin(), iCont.begin() + N);
    std::sort(tmp.begin(), tmp.end(), comp);
    return tmp;
}
}  // namespace bookdb