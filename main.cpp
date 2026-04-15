#include "Hash.hpp"
#include <iostream>

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    HashTable<Book> ht(10); //таблица 10 ячеек

    ht.loadFromFile("books.txt");
    ht.printAll();

    return 0;
}
