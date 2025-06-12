#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.author < rhv.author; }
};

struct LessByGenre {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.genre < rhv.genre; }
};

struct GreaterByRating {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.rating > rhv.rating; }
};

struct GreaterByReadCount {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.read_count > rhv.read_count; }
};

struct LessByRating {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.rating < rhv.rating; }
};

struct LessByPopularity {
    bool operator()(const Book &lhv, const Book &rhv) const { return lhv.read_count < rhv.read_count; }
};
}  // namespace bookdb::comp