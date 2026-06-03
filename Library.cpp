#include "Library.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>

int Library::readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number.\n";
    }
}

Library::Library() { load(); }

void Library::load() {
    books.clear();
    std::ifstream in("data/books.dat", std::ios::binary);
    if (!in) return; // کامپایلر در اولین اجرا بدون خطا عبور می‌کند
    Book b;
    while (in.read(reinterpret_cast<char*>(&b), sizeof(Book)))
        books.push_back(b);
}

void Library::save() {
    std::ofstream out("data/books.dat", std::ios::binary | std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open data/books.dat for writing");
    for (const auto& b : books)
        out.write(reinterpret_cast<const char*>(&b), sizeof(Book));
}

void Library::addBook() {
    Book b{};
    b.id = readInt("ID: ");
    if (b.id <= 0) throw std::invalid_argument("ID must be a positive integer");

    for (const auto& x : books)
        if (x.id == b.id) throw std::runtime_error("Duplicate ID");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Title: ";  std::cin.getline(b.title,  100);
    std::cout << "Author: "; std::cin.getline(b.author, 100);
    b.borrowed = false;

    books.push_back(b);
    save();
    Logger::log("ADD_BOOK", "ID=" + std::to_string(b.id), "SUCCESS");
    std::cout << "Book added successfully.\n";
}

void Library::viewBooks() {
    if (books.empty()) { std::cout << "No books in the library.\n"; return; }
    for (const auto& b : books) {
        std::cout << "\nID:     " << b.id
                  << "\nTitle:  " << b.title
                  << "\nAuthor: " << b.author
                  << "\nStatus: " << (b.borrowed ? "Borrowed" : "Available");
        if (b.borrowed) {
            std::cout << " -> " << b.borrower << " (due: " << b.returnDate << ")";
        }
        std::cout << "\n";
    }
}

void Library::searchById() {
    int id = readInt("ID: ");
    for (const auto& b : books) {
        if (b.id == id) { 
            std::cout << b.title << " - " << b.author << "\n"; 
            return; 
        }
    }
    std::cout << "No book found with ID " << id << ".\n";
}

void Library::searchByTitle() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    char q[100];
    std::cout << "Title: "; std::cin.getline(q, 100);
    bool found = false;
    for (const auto& b : books) {
        if (std::string(b.title).find(q) != std::string::npos) {
            std::cout << b.id << "  " << b.title << " - " << b.author << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No books matched.\n";
}

void Library::editBook() {
    int id = readInt("ID: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (auto& b : books) {
        if (b.id == id) {
            if (b.borrowed) throw std::runtime_error("Cannot edit a borrowed book");
            std::cout << "New Title:  "; std::cin.getline(b.title,  100);
            std::cout << "New Author: "; std::cin.getline(b.author, 100);
            save();
            Logger::log("EDIT_BOOK", "ID=" + std::to_string(id), "SUCCESS");
            std::cout << "Book updated.\n";
            return;
        }
    }
    std::cout << "No book found with ID " << id << ".\n";
}

void Library::deleteBook() {
    int id = readInt("ID: ");
    for (const auto& b : books)
        if (b.id == id && b.borrowed)
            throw std::runtime_error("Cannot delete a borrowed book");

    auto before = books.size();
    books.erase(
        std::remove_if(books.begin(), books.end(), [id](const Book& b) { return b.id == id; }),
        books.end()
    );
    
    if (books.size() == before) {
        std::cout << "No book found with ID " << id << ".\n";
        return;
    }
    save();
    Logger::log("DELETE_BOOK", "ID=" + std::to_string(id), "SUCCESS");
    std::cout << "Book deleted.\n";
}

void Library::sortById() {
    std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) { return a.id < b.id; });
    save();
    std::cout << "Sorted by ID and saved to disk.\n";
}

void Library::sortByTitle() {
    std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
        return std::string(a.title) < std::string(b.title);
    });
    save();
    std::cout << "Sorted by title and saved to disk.\n";
}

void Library::borrowBook() {
    int id = readInt("ID: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (auto& b : books) {
        if (b.id == id) {
            if (b.borrowed) throw std::runtime_error("Already borrowed");
            b.borrowed = true;
            std::cout << "Borrower: ";    std::cin.getline(b.borrower,   100);
            std::cout << "Return Date: "; std::cin.getline(b.returnDate,  20);

            Transaction t{};
            t.bookId = id;
            
            // رفع باگ امنیتی strncpy با تضمین وجود Null-terminator در انتهای رشته
            std::strncpy(t.borrower, b.borrower, sizeof(t.borrower) - 1);
            t.borrower[sizeof(t.borrower) - 1] = '\0';
            
            std::strncpy(t.returnDate, b.returnDate, sizeof(t.returnDate) - 1);
            t.returnDate[sizeof(t.returnDate) - 1] = '\0';

            std::ofstream tr("data/transactions.dat", std::ios::binary | std::ios::app);
            if (!tr) throw std::runtime_error("Cannot open transactions.dat");
            tr.write(reinterpret_cast<const char*>(&t), sizeof(Transaction));

            save();
            Logger::log("BORROW_BOOK", "ID=" + std::to_string(id) + " BY=" + b.borrower, "SUCCESS");
            std::cout << "Book borrowed successfully.\n";
            return;
        }
    }
    std::cout << "No book found with ID " << id << ".\n";
}

void Library::returnBook() {
    int id = readInt("ID: ");
    for (auto& b : books) {
        if (b.id == id) {
            if (!b.borrowed) throw std::runtime_error("Book is not currently borrowed");
            b.borrowed      = false;
            b.borrower[0]   = '\0';
            b.returnDate[0] = '\0';
            save();
            Logger::log("RETURN_BOOK", "ID=" + std::to_string(id), "SUCCESS");
            std::cout << "Book returned.\n";
            return;
        }
    }
    std::cout << "No book found with ID " << id << ".\n";
}

void Library::countBooks() {
    int total = static_cast<int>(books.size());
    int borrowed = static_cast<int>(std::count_if(books.begin(), books.end(), [](const Book& b){ return b.borrowed; }));
    std::cout << "Total:     " << total            << "\n"
              << "Available: " << (total - borrowed) << "\n"
              << "Borrowed:  " << borrowed          << "\n";
}