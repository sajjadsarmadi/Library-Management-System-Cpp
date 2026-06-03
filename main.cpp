#include "Library.h"
#include <iostream>
#include <limits>

// Helper: read a validated integer from stdin.
// Clears failbit + discards leftover chars if the user types non-numeric input.
static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number.\n";
    }
}

int main() {
    Library lib;
    int c;
    do {
        std::cout << "\n1  Add Book\n"
                     "2  View Books\n"
                     "3  Search by ID\n"
                     "4  Search by Title\n"
                     "5  Edit Book\n"
                     "6  Delete Book\n"
                     "7  Sort by ID\n"
                     "8  Sort by Title\n"
                     "9  Borrow Book\n"
                     "10 Return Book\n"
                     "11 Count Books\n"
                     "0  Exit\n"
                     "Choice: ";

        c = readInt("");
        try {
            switch (c) {
                case 1:  lib.addBook();      break;
                case 2:  lib.viewBooks();    break;
                case 3:  lib.searchById();   break;
                case 4:  lib.searchByTitle();break;
                case 5:  lib.editBook();     break;
                case 6:  lib.deleteBook();   break;
                case 7:  lib.sortById();     break;
                case 8:  lib.sortByTitle();  break;
                case 9:  lib.borrowBook();   break;
                case 10: lib.returnBook();   break;
                case 11: lib.countBooks();   break;
                case 0:  break;
                default: std::cout << "Invalid choice.\n"; break;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    } while (c != 0);

    return 0;
}
