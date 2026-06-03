cmake_minimum_required(VERSION 3.15)
project(LibraryManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# پیدا کردن مستقیم فایل‌ها در صفحه اصلی مخزن
add_executable(LibraryManagementSystem 
    main.cpp 
    Library.cpp 
    Library.h 
    Book.h 
    Logger.cpp 
    Logger.h
)
