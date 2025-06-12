#pragma once

#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual>;
    using iterator = typename BookContainer::iterator;
    using reverse_iterator = typename BookContainer::reverse_iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using value_type = typename BookContainer::value_type;
    using reference = typename BookContainer::reference;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<value_type> iIList) {
        books_.reserve(iIList.size());
        std::for_each(iIList.begin(), iIList.end(), [this](Book &&iBook) { PushBack(std::move(iBook)); });
    }

    // Standard container interface methods
    iterator begin() noexcept { return books_.begin(); }
    iterator end() noexcept { return books_.end(); }
    const_iterator begin() const noexcept { return books_.cbegin(); }
    const_iterator end() const noexcept { return books_.cend(); }
    const_iterator cbegin() const noexcept { return books_.cbegin(); }
    const_iterator cend() const noexcept { return books_.cend(); }
    reverse_iterator rbegin() noexcept { return books_.rbegin(); }
    reverse_iterator rend() noexcept { return books_.rend(); }
    bool empty() const noexcept { return books_.empty(); }
    reference front() noexcept { return books_.front(); }
    reference back() noexcept { return books_.back(); }
    size_t size() const noexcept { return books_.size(); }
    const AuthorContainer &GetAuthors() const noexcept { return authors_; }
    const BookContainer &GetBooks() const noexcept { return books_; }

    constexpr void PushBack(const value_type &iElem) {
        books_.push_back(iElem);
        RegAuthor(books_.back());
    }
    constexpr void PushBack(value_type &&iElem) noexcept(noexcept(books_.push_back(std::move(iElem)))) {
        books_.push_back(std::move(iElem));
        RegAuthor(books_.back());
    }
    template <typename... Args>
    constexpr reference EmplaceBack(Args &&...iArgs) {
        reference ref = books_.emplace_back(std::forward<Args>(iArgs)...);
        RegAuthor(ref);
        return ref;
    }
    void Clear() {
        books_.clear();
        authors_.clear();
    }

private:
    constexpr bool RegAuthor(value_type &iRef) {
        auto [it, trig] = authors_.emplace(iRef.author);
        if (trig)
            iRef.author = *it;
        return trig;
    }

    BookContainer books_;
    AuthorContainer authors_;
};  // end class BookDatabase
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
