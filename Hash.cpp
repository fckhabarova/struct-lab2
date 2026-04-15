#include "Hash.hpp"

template<typename T>
HashTable<T>::HashTable(int _size)
    : size(_size > 0 ? _size : 1), amount(0) {
    table.resize(size);
}

template<typename T>
int HashTable<T>::word_to_hash(std::string str) {
    int hash = 0;
    for (char c : str) {
        hash = hash * 31 + c; //полиномиальный хэш
    }
    return abs(hash); //модуль для положительного
}

//начальная позиция в таблице
template<typename T>
int HashTable<T>::hash1(std::string key) {
    return word_to_hash(key) % size;
}

//ячейка занята = шаг зависящий от ключа
template<typename T>
int HashTable<T>::hash2(std::string key) {
    if (size <= 1) return 1;
    return 1 + (word_to_hash(key) % (size - 1));
}

template<typename T>
void HashTable<T>::insert(std::string key, T data) {
    if (amount >= size * 0.7) { //больше 70 процентов = увеличиваем таблицу
        rehash();
    }

    int h1 = hash1(key);
    int h2 = hash2(key);

    for (int i = 0; i < size; i++) {
        int index = (h1 + i * h2) % size;

        if (table[index].status == empty || table[index].status == deleted) {
            table[index].data = data;
            table[index].key = key;
            table[index].status = used;
            amount++;
            return;
        }
        else if (table[index].status == used && table[index].key == key) {
            table[index].data = data;
            return;
        }
    }
    std::cout << "Ошибка: таблица переполнена!\n";
}

template<typename T>
void HashTable<T>::remove(std::string key) {
    int index = _find(key);
    if (index != -1) {
        table[index].status = deleted;
        amount--;
        std::cout << "Книга \"" << key << "\" удалена\n";
    } else {
        std::cout << "Книга \"" << key << "\" не найдена\n";
    }
}

template<typename T>
bool HashTable<T>::find(std::string key, T& result) {
    int index = _find(key);
    if (index != -1) {
        result = table[index].data;
        return true;
    }
    return false;
}

template<typename T>
int HashTable<T>::_find(std::string key) {
    int h1 = hash1(key);
    int h2 = hash2(key);

    for (int i = 0; i < size; i++) {
        int index = (h1 + i * h2) % size;

        if (table[index].status == empty) { //пусто = не найдено
            return -1;
        }
        else if (table[index].status == used && table[index].key == key) {
            return index;
        }
    }
    return -1;
}

template<typename T>
void HashTable<T>::rehash() {
    std::vector<Node> oldTable = table; //сохраняем старую
    int oldSize = size;
    
    size = size * 2; //удваиваем размер
    table.clear();
    table.resize(size); //новая пустая таблица
    amount = 0;

    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i].status == used) {
            insert(oldTable[i].key, oldTable[i].data); //вставляем заново
        }
    }
}

//формат файла : название автор год
template<typename T>
bool HashTable<T>::loadFromFile(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл " << filename << " не найден!\n";
        return false;
    }

    std::string line;
    std::string title, author;
    int year;
    int count = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            title = line.substr(0, pos1);
            author = line.substr(pos1 + 1, pos2 - pos1 - 1);
            year = std::stoi(line.substr(pos2 + 1));
            
            Book book(title, author, year);
            insert(title, book);
            count++;
        }
    }

    file.close();
    std::cout << "Загружено книг: " << count << std::endl;
    return true;
}

template<typename T>
void HashTable<T>::printAll() {
    std::cout << "\nСОДЕРЖИМОЕ ХЭШ-ТАБЛИЦЫ\n";
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (i < table.size() && table[i].status == used) {
            std::cout << "[" << i << "] ";
            table[i].data.print();
            std::cout << "\n";
            count++;
        }
    }
    std::cout << "Всего записей: " << count
              << " | Размер таблицы: " << size
              << " | Загруженность: " << (amount * 100 / size) << "%\n";
}

//линейный просмотр всех ячеек
template<typename T>
void HashTable<T>::findByAuthor(std::string author) {
    std::cout << "\nКниги автора \"" << author << "\"\n";
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (table[i].status == used && table[i].data.author == author) {
            std::cout << "  • ";
            table[i].data.print();
            std::cout << std::endl;
            found++;
        }
    }
    if (found == 0) {
        std::cout << "Книги не найдены\n";
    }
}

template<typename T>
void HashTable<T>::findByYear(int year) {
    std::cout << "\nКниги " << year << " года\n";
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (table[i].status == used && table[i].data.year == year) {
            std::cout << "  • ";
            table[i].data.print();
            std::cout << std::endl;
            found++;
        }
    }
    if (found == 0) {
        std::cout << "Книги не найдены\n";
    }
}

template class HashTable<Book>;
