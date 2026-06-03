#pragma once
#include "Book.h"
#include <string>
#include <vector>

class Library {
    std::vector<Book> books;
    void load();
    void save();
    // Helper: read a validated integer from stdin
    static int readInt(const std::string& prompt);
public:
    Library();
    void addBook();
    void viewBooks();
    void searchById();
    void searchByTitle();
    void editBook();
    void deleteBook();
    void sortById();
    void sortByTitle();
    void borrowBook();
    void returnBook();
    void countBooks();
};
