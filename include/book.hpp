#pragma once

#include <flat_map>
#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {
using namespace std::literals;

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь
constexpr Genre GenreFromString(std::string_view iS) {
    static const std::flat_map<std::string_view, bookdb::Genre> map = {{"Fiction"sv, Genre::Fiction},
                                                                       {"NonFiction"sv, Genre::NonFiction},
                                                                       {"SciFi"sv, Genre::SciFi},
                                                                       {"Biography"sv, Genre::Biography},
                                                                       {"Mystery"sv, Genre::Mystery}};
    if (auto it = map.find(iS); it != map.end())
        return it->second;
    return Genre::Unknown;
}

constexpr std::string GenreToString(Genre iG) {
    std::string res;
    switch (iG) {
    case Genre::Fiction:
        res = "Fiction";
        break;
    case Genre::Mystery:
        res = "Mystery";
        break;
    case Genre::NonFiction:
        res = "NonFiction";
        break;
    case Genre::SciFi:
        res = "SciFi"sv;
        break;
    case Genre::Biography:
        res = "Biography";
        break;
    case Genre::Unknown:
    default:
        res = "Unknown";
        break;
    }
    return res;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string title;
    std::string_view author;

    double rating;
    int year;
    int read_count;
    Genre genre;

    constexpr Book(const std::string &iTitle, std::string_view iAuthor, int iYear, Genre iGenre, double iRating,
                   int iReadCount)
        : title(iTitle), author(iAuthor), rating(iRating), year(iYear), read_count(iReadCount), genre(iGenre) {}

    constexpr Book(const std::string &iTitle, std::string_view iAuthor, int iYear, std::string_view iGenre,
                   double iRating, int iReadCount)
        : Book(iTitle, iAuthor, iYear, GenreFromString(iGenre), iRating, iReadCount) {}

    auto operator<=>(const Book &rhv) const = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        const std::string genre_str = bookdb::GenreToString(g);
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &iBook, FormatContext &fc) const {
        constexpr auto fmt("title : {}, Author : {}, year : {}, genre : {}, rating : {}, read_count : {}");

        return format_to(fc.out(), fmt, iBook.title, iBook.author, iBook.year, GenreToString(iBook.genre), iBook.rating,
                         iBook.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
