#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

struct Book {
    std::string title;     // название (ключ)
    std::string author;    // автор
    int year;              // год издания

    Book() : year(0) {}
    Book(std::string t, std::string a, int y)
        : title(t), author(a), year(y) {}
    
    void print() const {
        std::cout << "\"" << title << "\" - " << author
                  << " (" << year << " г.)";
    }
};

template<typename T>
class HashTable {
private:
    enum Status { empty, used, deleted };

    struct Node {
        T data; //книга
        Status status; //статус ячейки
        std::string key; //ключ
        Node() : status(empty) {}
    };

    std::vector<Node> table; //массив ячеек
    int size; //размер таблицы
    int amount; //кол занятых ячеек

    int word_to_hash(std::string str); //вычисление хэша строки
    int hash1(std::string key); //1 хэш-функ
    int hash2(std::string key); //2 хэш-функ
    int _find(std::string key); //приватный поиск
    void rehash(); //расширение таблицы

public:
    HashTable(int _size);
    void insert(std::string key, T data); //добавление элемента
    void remove(std::string key); //удаление
    bool find(std::string key, T& result);
    bool loadFromFile(std::string filename); //загрузка из файла
    void printAll();
    
//поиск по полям
    void findByAuthor(std::string author);
    void findByYear(int year);
};
