#pragma once

struct Book {
    int id;
    char title[100];
    char author[100];
    bool borrowed;
    char borrower[100];
    char returnDate[20];
};

struct Transaction {
    int bookId;
    char borrower[100];
    char returnDate[20];
};