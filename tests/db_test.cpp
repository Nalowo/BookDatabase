#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    BookDatabase<std::vector<Book>> db;

    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    }
};

TEST_F(BookDatabaseTest, PushBackFunctionality) {
    const size_t initialSize = db.size();

    Book newBook{"New Book", "Author", 2023, Genre::SciFi, 4.5, 100};
    db.PushBack(newBook);

    EXPECT_EQ(db.size(), initialSize + 1);
    EXPECT_FALSE(db.GetBooks().empty());
    EXPECT_EQ(db.GetBooks().back().title, "New Book");
}

TEST_F(BookDatabaseTest, EmplaceBackFunctionality) {
    const size_t initialSize = db.size();

    auto &book = db.EmplaceBack("Emplaced", "Author", 2023, Genre::Mystery, 4.7, 150);

    EXPECT_EQ(db.size(), initialSize + 1);
    EXPECT_EQ(book.title, "Emplaced");
    EXPECT_EQ(db.GetBooks().back().author, "Author");
}

TEST_F(BookDatabaseTest, AuthorRegistration) {
    db.Clear();

    db.EmplaceBack("Book1", "Author A", 2000, Genre::Fiction, 4.0, 100);
    db.EmplaceBack("Book2", "Author B", 2001, Genre::SciFi, 4.5, 200);
    db.EmplaceBack("Book3", "Author A", 2002, Genre::Mystery, 4.2, 150);  // Дубликат автора

    const auto &authors = db.GetAuthors();
    ASSERT_EQ(authors.size(), 2);  // Должно быть 2 уникальных автора
    EXPECT_TRUE(authors.contains("Author A"));
    EXPECT_TRUE(authors.contains("Author B"));
}

TEST(EmptyDatabaseTest, EmptyStateHandling) {
    BookDatabase<std::vector<Book>> emptyDb;

    EXPECT_TRUE(emptyDb.empty());
    EXPECT_EQ(emptyDb.size(), 0);
    EXPECT_TRUE(emptyDb.GetAuthors().empty());
    EXPECT_TRUE(emptyDb.GetBooks().empty());
}

TEST_F(BookDatabaseTest, InvalidInputHandling) {
    // Некорректный жанр
    db.EmplaceBack("Invalid Genre", "Author", 2023, static_cast<Genre>(999), 2.0, 5);
    EXPECT_EQ(db.GetBooks().back().genre, static_cast<Genre>(999));
    EXPECT_EQ(GenreToString(db.GetBooks().back().genre), "Unknown");

    // Пустой автор
    db.EmplaceBack("No Author", "", 2023, Genre::Unknown, 3.5, 10);
    EXPECT_EQ(db.GetBooks().back().author, "");

    // Отрицательный рейтинг
    db.EmplaceBack("Negative Rating", "Author", 2023, Genre::Mystery, -1.0, 5);
    EXPECT_DOUBLE_EQ(db.GetBooks().back().rating, -1.0);
}

TEST_F(BookDatabaseTest, AuthorHistogramCalculation) {
    auto histogram = buildAuthorHistogramFlat(db);

    ASSERT_EQ(histogram.size(), 2);
    EXPECT_EQ(histogram["George Orwell"], 2);
    EXPECT_EQ(histogram["F. Scott Fitzgerald"], 1);
}

TEST_F(BookDatabaseTest, GenreRatingCalculation) {
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());

    EXPECT_DOUBLE_EQ(genreRatings[Genre::SciFi], 4.0);
    EXPECT_DOUBLE_EQ(genreRatings[Genre::Fiction], (4.4 + 4.5) / 2);
    EXPECT_DOUBLE_EQ(genreRatings[Genre::Unknown], 0.0);
}

TEST_F(BookDatabaseTest, AverageRatingCalculation) {
    const double expected = (4.0 + 4.4 + 4.5) / 3;
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), expected);
}

TEST_F(BookDatabaseTest, TopNSelection) {
    auto topBooks = getTopNBy(db, 2, comp::GreaterByRating{});

    ASSERT_EQ(topBooks.size(), 2);
    EXPECT_EQ(topBooks[0].title, "The Great Gatsby");  // 4.5
    EXPECT_EQ(topBooks[1].title, "Animal Farm");       // 4.4
}

TEST_F(BookDatabaseTest, RandomSampling) {
    auto sampled = sampleRandomBooks(db, 2);
    ASSERT_EQ(sampled.size(), 2);

    // Проверяем что все элементы из исходной БД
    for (const auto &bookRef : sampled) {
        auto it = std::find_if(db.begin(), db.end(), [&](const Book &b) { return b.title == bookRef.get().title; });
        EXPECT_NE(it, db.end());
    }
}

TEST_F(BookDatabaseTest, SingleFilterApplication) {
    auto filtered = filterBooks(db.cbegin(), db.cend(), YearBetween(1940, 1950));

    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "1984");
    EXPECT_EQ(filtered[1].get().title, "Animal Farm");
}

TEST_F(BookDatabaseTest, CompositeFilterApplication) {
    auto filter = all_of(YearBetween(1900, 2000), RatingAbove(4.3), GenreIs(Genre::Fiction));

    auto filtered = filterBooks(db.cbegin(), db.cend(), filter);

    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "Animal Farm");
    EXPECT_EQ(filtered[1].get().title, "The Great Gatsby");
}

TEST_F(BookDatabaseTest, PredicateFactories) {
    // Тест фабрики RatingAbove
    auto highRated = filterBooks(db.cbegin(), db.cend(), RatingAbove(4.4));
    ASSERT_EQ(highRated.size(), 1);
    EXPECT_EQ(highRated[0].get().title, "The Great Gatsby");

    // Тест фабрики GenreIs
    auto sciFi = filterBooks(db.cbegin(), db.cend(), GenreIs(Genre::SciFi));
    ASSERT_EQ(sciFi.size(), 1);
    EXPECT_EQ(sciFi[0].get().title, "1984");

    // Тест фабрики YearBetween
    auto years = filterBooks(db.cbegin(), db.cend(), YearBetween(1940, 1950));
    ASSERT_EQ(years.size(), 2);
    EXPECT_EQ(years[0].get().year, 1949);
    EXPECT_EQ(years[1].get().year, 1945);
}