#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <deque>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

// Установка русской кодировки для консоли
void setRussian() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
}

// Функция для разделения текста на строки
vector<string> splitString(const string& text, size_t maxLineLength = 60) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    
    while (getline(ss, line)) {
        if (line.length() > maxLineLength) {
            size_t pos = 0;
            while (pos < line.length()) {
                size_t end = min(pos + maxLineLength, line.length());
                lines.push_back(line.substr(pos, end - pos));
                pos = end;
            }
        } else {
            lines.push_back(line);
        }
    }
    return lines;
}

// === Программа 1: multimap для float ===
string program1() {
    stringstream result;
    result << "=== ПРОГРАММА 1: multimap с float ===\n\n";
    
    // 1. Создание и заполнение multimap
    multimap<float, float> container1;
    for (int i = 0; i < 10; ++i) {
        float key = i * 1.1f;
        float value = i * 2.2f;
        container1.insert({key, value});
    }
    
    // 2. Просмотр контейнера
    result << "Исходный контейнер:\n";
    for (const auto& pair : container1) {
        result << "Ключ: " << pair.first << " -> Значение: " << pair.second << "\n";
    }
    
    // 3. Изменение контейнера
    auto it = container1.find(2.2f);
    if (it != container1.end()) {
        container1.erase(it);
    }
    
    // 4. Просмотр с итераторами
    result << "\nПосле изменений:\n";
    for (auto it = container1.begin(); it != container1.end(); ++it) {
        result << "Ключ: " << it->first << " -> Значение: " << it->second << "\n";
    }
    
    // 5. Создание второго контейнера
    multimap<float, float> container2;
    for (int i = 5; i < 8; ++i) {
        container2.insert({i * 3.3f, i * 4.4f});
    }
    
    // 6. Изменение первого контейнера
    auto pos = container1.find(3.3f);
    if (pos != container1.end()) {
        int n = 2;
        auto next = std::next(pos);
        for (int i = 0; i < n && next != container1.end(); ++i) {
            next = container1.erase(next);
        }
        container1.insert(container2.begin(), container2.end());
    }
    
    // 7. Просмотр результатов
    result << "\nПервый контейнер после объединения:\n";
    for (const auto& pair : container1) {
        result << "Ключ: " << pair.first << " -> Значение: " << pair.second << "\n";
    }
    
    result << "\nВторой контейнер:\n";
    for (const auto& pair : container2) {
        result << "Ключ: " << pair.first << " -> Значение: " << pair.second << "\n";
    }
    
    return result.str();
}

// === Пользовательский тип данных ===
class MyData {
public:
    float x;
    float y;
    
    MyData(float x = 0, float y = 0) : x(x), y(y) {}
    
    bool operator<(const MyData& other) const {
        return (x*x + y*y) < (other.x*other.x + other.y*other.y);
    }
    
    friend ostream& operator<<(ostream& os, const MyData& data) {
        return os << "(" << data.x << ", " << data.y << ")";
    }
};

// === Программа 2: multimap для пользовательских типов ===
string program2() {
    stringstream result;
    result << "=== ПРОГРАММА 2: multimap с пользовательскими данными ===\n\n";
    
    // 1. Создание и заполнение
    multimap<float, MyData> container1;
    for (int i = 0; i < 8; ++i) {
        float key = i * 1.5f;
        MyData value(i * 2.0f, i * 3.0f);
        container1.insert({key, value});
    }
    
    // 2. Просмотр
    result << "Исходный контейнер:\n";
    for (const auto& pair : container1) {
        result << "Ключ: " << pair.first << " -> Значение: " << pair.second << "\n";
    }
    
    return result.str();
}

// === Программа 3: Алгоритмы STL с deque ===
string program3() {
    stringstream result;
    result << "=== ПРОГРАММА 3: deque с алгоритмами STL ===\n\n";
    
    // 1. Создание контейнера
    deque<int> container1 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // 2. Сортировка по убыванию
    sort(container1.rbegin(), container1.rend());
    
    // 3. Просмотр
    result << "После сортировки по убыванию:\n";
    for (const auto& item : container1) {
        result << item << " ";
    }
    result << "\n";
    
    // 4. Поиск элемента
    auto findCondition = [](int value) { return value > 5; };
    auto found = find_if(container1.begin(), container1.end(), findCondition);
    
    if (found != container1.end()) {
        result << "\nНайден элемент: " << *found << "\n";
    }
    
    // 5. Перемещение в другой контейнер
    deque<int> container2;
    copy_if(container1.begin(), container1.end(), back_inserter(container2), findCondition);
    
    // 6. Просмотр второго контейнера
    result << "\nВторой контейнер (элементы > 5):\n";
    for (const auto& item : container2) {
        result << item << " ";
    }
    
    return result.str();
}

int main() {
    setRussian();
    
    // Создаем SFML окно
    sf::RenderWindow window(sf::VideoMode(1200, 800), "STL Programs - Press 1, 2, 3");
    
    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Шрифт не найден, используется стандартный" << endl;
    }
    
    // Текст меню
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(20, 20);
    menuText.setString("STL PROGRAMS WITH SFML\n\nPress:\n1 - Multimap with float\n2 - Multimap with custom data\n3 - Deque with algorithms\n\nESC - Exit");
    
    // Текст для результатов
    sf::Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(18);
    resultText.setFillColor(sf::Color::Green);
    resultText.setPosition(20, 200);
    resultText.setString("Press 1, 2 or 3 to run programs");
    
    // Основной цикл SFML
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                string result;
                switch (event.key.code) {
                    case sf::Keyboard::Num1:
                        result = program1();
                        break;
                    case sf::Keyboard::Num2:
                        result = program2();
                        break;
                    case sf::Keyboard::Num3:
                        result = program3();
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                }
                
                // Обновляем текст результатов
                if (!result.empty()) {
                    resultText.setString(result);
                }
            }
        }
        
        // Отрисовка
        window.clear(sf::Color::Black);
        window.draw(menuText);
        window.draw(resultText);
        window.display();
        
        // Небольшая задержка
        sf::sleep(sf::milliseconds(16));
    }
    
    return 0;
}