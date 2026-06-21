# BookDatabase

> Шаблонная in-memory база данных книг с C++20-концептами, гетерогенным поиском и компонуемыми фильтрами.

In-memory хранилище книг, спроектированное как обобщённый контейнер: тип внутреннего хранилища параметризуется и ограничивается концептом, поиск по строковым ключам выполняется без лишних аллокаций (heterogeneous lookup), а выборка и статистика строятся из компонуемых фильтров и компараторов.

## Возможности

- Хранение и индексирование книг с интерфейсом, близким к STL-контейнеру (итераторы, `begin/end`, `initializer_list`-конструктор).
- **Гетерогенный поиск** по авторам/строкам: запрос по `std::string_view` без создания временной `std::string`.
- Компонуемые **фильтры** и **компараторы** для выборки и сортировки.
- Подсчёт **статистики** по коллекции.

## Технологии и концепции C++

- **C++20 Concepts:** `BookContainerLike` и сопутствующие ограничения параметров шаблона.
- **Heterogeneous lookup:** прозрачные `TransparentStringHash` / `TransparentStringEqual` (`is_transparent`) для `unordered_set`/`unordered_map`, поиск по `string_view` без аллокаций.
- Шаблонное проектирование контейнера, параметризованного типом хранилища (`BookContainer = std::vector<Book>`).
- `constexpr`-конструкторы, STL-совместимый интерфейс, `std::for_each`.
- **C++23** `std::print`.

## Архитектура

```
BookDatabase<BookContainer>     — обобщённый контейнер (концепт BookContainerLike)
  ├─ book.hpp                   — модель Book
  ├─ concepts.hpp               — концепты-ограничения
  ├─ heterogeneous_lookup.hpp   — прозрачные хеш/сравнение для string_view
  ├─ filters.hpp                — компонуемые фильтры
  ├─ comparators.hpp            — компараторы для сортировки
  └─ statistics.hpp             — агрегированная статистика
```

## Стек

`C++23` · `Concepts` · `Conan` · `CMake` · `GoogleTest`

## Сборка

```bash
mkdir build && cd build
conan install --settings=build_type=Debug --build=missing ..
conan build --settings=build_type=Debug ..
```

В VS Code достаточно `F5`.

## Запуск

```bash
cd build
./BookDB
```

## Тесты

```bash
cd build
ctest --output-on-failure
```
