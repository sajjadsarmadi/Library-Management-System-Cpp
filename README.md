# Library Management System

A console-based Library Management System written in **C++17**, featuring persistent binary storage, transaction logging, and structured exception handling.

---

## Features

| Feature | Description |
|---|---|
| Add Book | Register a new book with ID, title, and author |
| View Books | List all books with availability status |
| Search | Lookup by ID or partial title match |
| Edit Book | Update title and author of an existing book |
| Delete Book | Remove a book by ID |
| Sort | Sort the collection by ID or title |
| Borrow / Return | Track borrowing with borrower name and return date |
| Persistent Storage | Binary file I/O (`books.dat`, `transactions.dat`) |
| Logging | Timestamped operation log to `logs.txt` |
| Error Handling | `std::exception`-based validation with duplicate ID detection |

---

## Project Structure

```
LibraryManagementSystem/
├── src/
│   ├── Book.h           # Book and Transaction structs
│   ├── Library.h        # Library class declaration
│   ├── Library.cpp      # Core CRUD and I/O logic
│   ├── Logger.h         # Logger class declaration
│   ├── Logger.cpp       # Timestamped file logging
│   └── main.cpp         # Entry point and menu loop
├── data/
│   ├── books.dat        # Binary book records (generated at runtime)
│   ├── transactions.dat # Binary transaction records (generated at runtime)
│   └── logs.txt         # Operation log (generated at runtime)
├── CMakeLists.txt
├── .gitignore
└── README.md
```

> **Note:** Files under `data/` are generated at runtime and excluded from version control via `.gitignore`.

---

## Build & Run

### Option 1 — CMake (recommended)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./library        # Linux / macOS
library.exe      # Windows
```

### Option 2 — Direct g++

```bash
g++ -std=c++17 src/*.cpp -o library
./library
```

> The binary **must be run from the project root** so that the relative path `data/` resolves correctly.

---

## Requirements

- C++17 or later
- GCC 7+ / Clang 5+ / MSVC 2017+
- CMake 3.10+ *(optional, for CMake build)*

---

## Usage

On launch, a numbered menu is presented:

```
1  Add Book
2  View Books
3  Search by ID
4  Search by Title
5  Edit Book
6  Delete Book
7  Sort by ID
8  Sort by Title
9  Borrow Book
10 Return Book
11 Count Books
0  Exit
```

Enter the corresponding number and follow the prompts.

---

## Implementation Notes

- **Storage format:** `Book` and `Transaction` are plain POD structs serialized with `fwrite`/`fread` (`sizeof`-based binary I/O). This is fast and simple but not portable across platforms with different struct padding or endianness.
- **In-memory model:** All books are loaded into a `std::vector<Book>` on startup and written back in full on every mutation — acceptable for small collections; not designed for large-scale data.
- **Logging:** `Logger::log()` appends a timestamped entry to `data/logs.txt` using `std::ctime`. Currently only `addBook` is instrumented; other operations can be extended similarly.

---

## Known Limitations

- The `data/` path is hardcoded as a relative path — the binary must be launched from the project root.
- Binary storage is not cross-platform portable (struct padding / endianness).
- `sortById` / `sortByTitle` sort the in-memory vector but do not persist the new order to disk.
- No authentication or multi-user support.
