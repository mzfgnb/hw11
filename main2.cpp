#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Пользовательский тип данных
class Student {
private:
    string name;
    int age;
    float averageGrade;
    
public:
    Student(const string& n = "", int a = 0, float g = 0.0f) 
        : name(n), age(a), averageGrade(g) {}
    
    string getName() const { return name; }
    int getAge() const { return age; }
    float getAverageGrade() const { return averageGrade; }
    
    bool operator<(const Student& other) const {
        return averageGrade < other.averageGrade;
    }
    
    string toString() const {
        return name + " (Возраст: " + to_string(age) + 
               ", Оценка: " + to_string(averageGrade).substr(0, 4) + ")";
    }
};

class ProgramManager {
private:
    sf::RenderWindow window;
    int currentProgram;
    string outputText;
    sf::Clock keyClock;
    bool needUpdate;

public:
    ProgramManager() : window(sf::VideoMode(800, 600), "Контейнерные классы"), 
                      currentProgram(1), needUpdate(true) {
        window.setFramerateLimit(30);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (needUpdate) {
                update();
                needUpdate = false;
            }
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    currentProgram = 1;
                    needUpdate = true;
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    currentProgram = 2; 
                    needUpdate = true;
                }
                if (event.key.code == sf::Keyboard::Num3) {
                    currentProgram = 3;
                    needUpdate = true;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
    }

    void update() {
        system("cls");
        cout << "==============================================" << endl;
        
        if (currentProgram == 1) {
            cout << "ПРОГРАММА 1: Встроенные типы данных (float)" << endl;
            cout << "==============================================" << endl;
            
            // 1. Создаем multimap
            multimap<float, float> multiMap;
            for (int i = 0; i < 5; ++i) {
                float key = static_cast<float>(i);
                float value = static_cast<float>(rand() % 100) / 10.0f;
                multiMap.insert({key, value});
            }
            
            cout << "1. Multimap создан и заполнен:" << endl;
            for (const auto& pair : multiMap) {
                cout << "   Ключ: " << pair.first << " -> Значение: " << pair.second << endl;
            }
            
            // 2. Создаем deque
            deque<float> floatDeque;
            for (int i = 0; i < 6; ++i) {
                floatDeque.push_back(static_cast<float>(rand() % 100) / 10.0f);
            }
            
            cout << "\n2. Deque создан и заполнен:" << endl;
            for (const auto& val : floatDeque) {
                cout << "   " << val;
            }
            cout << endl;
            
            // 3. Изменяем контейнеры
            cout << "\n3. Изменяем контейнеры:" << endl;
            cout << "   - Удаляем элемент с ключом 2 из multimap" << endl;
            cout << "   - Заменяем 2-й элемент в deque на 99.9" << endl;
            
            multiMap.erase(2.0f);
            if (floatDeque.size() > 1) {
                floatDeque[1] = 99.9f;
            }
            
            // 4. Просмотр через итераторы
            cout << "\n4. Multimap через итераторы:" << endl;
            for (auto it = multiMap.begin(); it != multiMap.end(); ++it) {
                cout << "   Ключ: " << it->first << " -> Значение: " << it->second << endl;
            }
            
            // 5. Второй контейнер
            cout << "\n5. Создаем второй deque:" << endl;
            deque<float> secondDeque;
            for (int i = 0; i < 3; ++i) {
                float val = static_cast<float>(rand() % 50) / 10.0f + 50.0f;
                secondDeque.push_back(val);
                cout << "   " << val;
            }
            cout << endl;
            
            // 6. Изменяем первый deque
            cout << "\n6. Изменяем первый deque:" << endl;
            cout << "   - Удаляем 2 элемента после 3-го" << endl;
            cout << "   - Добавляем элементы из второго deque" << endl;
            
            if (floatDeque.size() > 3) {
                auto it = floatDeque.begin();
                advance(it, 3);
                if (distance(it, floatDeque.end()) >= 2) {
                    auto endIt = it;
                    advance(endIt, 2);
                    floatDeque.erase(it, endIt);
                }
            }
            
            floatDeque.insert(floatDeque.end(), secondDeque.begin(), secondDeque.end());
            
            // 7. Итоговый просмотр
            cout << "\n7. Итоговые контейнеры:" << endl;
            cout << "Multimap:" << endl;
            for (const auto& pair : multiMap) {
                cout << "   Ключ: " << pair.first << " -> Значение: " << pair.second << endl;
            }
            
            cout << "Первый deque:" << endl;
            for (const auto& val : floatDeque) {
                cout << "   " << val;
            }
            cout << endl;
            
            cout << "Второй deque:" << endl;
            for (const auto& val : secondDeque) {
                cout << "   " << val;
            }
            cout << endl;
            
        } else if (currentProgram == 2) {
            cout << "ПРОГРАММА 2: Пользовательские типы данных (Student)" << endl;
            cout << "==============================================" << endl;
            
            // 1. Создаем multimap
            multimap<float, Student> studentMap;
            studentMap.insert({85.5f, Student("Иванов", 20, 85.5f)});
            studentMap.insert({92.3f, Student("Петров", 21, 92.3f)});
            studentMap.insert({78.9f, Student("Сидоров", 19, 78.9f)});
            studentMap.insert({85.5f, Student("Кузнецов", 22, 85.5f)});
            
            cout << "1. Multimap студентов создан:" << endl;
            for (const auto& pair : studentMap) {
                cout << "   Оценка: " << pair.first << " - " << pair.second.toString() << endl;
            }
            
            // 2. Создаем deque
            deque<Student> studentDeque;
            studentDeque.push_back(Student("Смирнов", 20, 88.0f));
            studentDeque.push_back(Student("Попов", 21, 76.5f));
            studentDeque.push_back(Student("Волков", 19, 91.2f));
            studentDeque.push_back(Student("Орлов", 22, 83.7f));
            
            cout << "\n2. Deque студентов создан:" << endl;
            for (const auto& student : studentDeque) {
                cout << "   " << student.toString() << endl;
            }
            
            // 3. Изменяем контейнеры
            cout << "\n3. Изменяем контейнеры:" << endl;
            cout << "   - Удаляем студентов с оценкой 85.5" << endl;
            cout << "   - Заменяем 2-го студента" << endl;
            
            studentMap.erase(85.5f);
            if (studentDeque.size() > 1) {
                studentDeque[1] = Student("Новиков", 23, 95.0f);
            }
            
            // 4. Второй контейнер
            cout << "\n4. Создаем второй deque:" << endl;
            deque<Student> secondDeque;
            secondDeque.push_back(Student("Федоров", 20, 87.3f));
            secondDeque.push_back(Student("Никитин", 21, 79.8f));
            
            for (const auto& student : secondDeque) {
                cout << "   " << student.toString() << endl;
            }
            
            // 5. Изменяем первый deque
            cout << "\n5. Изменяем первый deque:" << endl;
            cout << "   - Удаляем 1 элемент после 2-го" << endl;
            cout << "   - Добавляем элементы из второго deque" << endl;
            
            if (studentDeque.size() > 2) {
                auto it = studentDeque.begin();
                advance(it, 2);
                studentDeque.erase(it);
            }
            
            studentDeque.insert(studentDeque.end(), secondDeque.begin(), secondDeque.end());
            
            // 6. Итоговый просмотр
            cout << "\n6. Итоговые контейнеры:" << endl;
            cout << "Multimap:" << endl;
            for (const auto& pair : studentMap) {
                cout << "   Оценка: " << pair.first << " - " << pair.second.toString() << endl;
            }
            
            cout << "Первый deque:" << endl;
            for (const auto& student : studentDeque) {
                cout << "   " << student.toString() << endl;
            }
            
            cout << "Второй deque:" << endl;
            for (const auto& student : secondDeque) {
                cout << "   " << student.toString() << endl;
            }
            
        } else if (currentProgram == 3) {
            cout << "ПРОГРАММА 3: Алгоритмы STL" << endl;
            cout << "==============================================" << endl;
            
            static deque<float> numbers;
            static deque<float> secondContainer;
            static deque<float> thirdContainer;
            static int step = 0;
            
            if (step == 0) {
                numbers.clear();
                for (int i = 0; i < 8; ++i) {
                    numbers.push_back(static_cast<float>(rand() % 100));
                }
            }
            
            cout << "Шаг " << step + 1 << ":" << endl << endl;
            
            switch (step) {
                case 0:
                    cout << "1. Исходный контейнер:" << endl;
                    for (const auto& num : numbers) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 1:
                    sort(numbers.rbegin(), numbers.rend());
                    cout << "2. Отсортировано по убыванию:" << endl;
                    for (const auto& num : numbers) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 2: {
                    auto it = find_if(numbers.begin(), numbers.end(), [](float x) { return x > 70.0f; });
                    cout << "3. Поиск элемента > 70:" << endl;
                    if (it != numbers.end()) cout << "   Найден: " << *it << endl;
                    else cout << "   Элементы > 70 не найдены" << endl;
                    break;
                }
                    
                case 3:
                    secondContainer.clear();
                    copy_if(numbers.begin(), numbers.end(), back_inserter(secondContainer),
                           [](float x) { return x > 70.0f; });
                    cout << "4. Перенесены элементы > 70:" << endl;
                    for (const auto& num : secondContainer) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 4:
                    cout << "5. Второй контейнер:" << endl;
                    for (const auto& num : secondContainer) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 5:
                    sort(numbers.begin(), numbers.end());
                    cout << "6. Первый контейнер по возрастанию:" << endl;
                    for (const auto& num : numbers) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 6:
                    sort(secondContainer.begin(), secondContainer.end());
                    cout << "7. Второй контейнер по возрастанию:" << endl;
                    for (const auto& num : secondContainer) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 7:
                    cout << "8. Оба контейнера:" << endl << "Первый: ";
                    for (const auto& num : numbers) cout << num << " ";
                    cout << endl << "Второй: ";
                    for (const auto& num : secondContainer) cout << num << " ";
                    cout << endl;
                    break;
                    
                case 8:
                    thirdContainer.clear();
                    merge(numbers.begin(), numbers.end(), secondContainer.begin(), secondContainer.end(),
                         back_inserter(thirdContainer));
                    cout << "9. Третий контейнер (слияние):" << endl;
                    for (const auto& num : thirdContainer) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 9:
                    cout << "10. Третий контейнер:" << endl;
                    for (const auto& num : thirdContainer) cout << "   " << num;
                    cout << endl;
                    break;
                    
                case 10: {
                    int count = count_if(thirdContainer.begin(), thirdContainer.end(),
                        [](float x) { return x > 70.0f; });
                    cout << "11. Элементов > 70: " << count << endl;
                    break;
                }
                    
                case 11: {
                    bool exists = any_of(thirdContainer.begin(), thirdContainer.end(),
                        [](float x) { return x > 70.0f; });
                    cout << "12. Элементы > 70 " << (exists ? "есть" : "отсутствуют") << endl;
                    break;
                }
            }
            
            cout << "\nНажмите ПРОБЕЛ для следующего шага" << endl;
            cout << "Нажмите R для перезапуска программы 3" << endl;
            
            // Обработка шагов
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && keyClock.getElapsedTime().asMilliseconds() > 300) {
                step = (step + 1) % 12;
                keyClock.restart();
                needUpdate = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && keyClock.getElapsedTime().asMilliseconds() > 300) {
                step = 0;
                keyClock.restart();
                needUpdate = true;
            }
        }
        
        cout << "==============================================" << endl;
        cout << "Управление: 1-Программа1, 2-Программа2, 3-Программа3, ESC-Выход" << endl;
    }

    void render() {
        window.clear(sf::Color::White);
        
        // Простая графика - показываем только номер текущей программы
        sf::RectangleShape background(sf::Vector2f(200, 60));
        background.setPosition(300, 270);
        background.setFillColor(sf::Color::Blue);
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(2);
        window.draw(background);
        
        window.display();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    cout << "ПРОГРАММА ЗАПУЩЕНА!" << endl;
    cout << "Управление:" << endl;
    cout << "1 - Программа 1 (встроенные типы)" << endl;
    cout << "2 - Программа 2 (пользовательские типы)" << endl;
    cout << "3 - Программа 3 (алгоритмы STL)" << endl;
    cout << "ESC - выход" << endl;
    cout << "==============================================" << endl;
    
    ProgramManager manager;
    manager.run();
    
    return 0;
}
