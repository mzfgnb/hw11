#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>

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
               ", Оценка: " + to_string(averageGrade) + ")";
    }
};

class ProgramManager {
private:
    sf::RenderWindow window;
    int currentProgram;
    string outputText;
    sf::Clock keyClock;

public:
    ProgramManager() : window(sf::VideoMode(1000, 700), "Контейнерные классы и алгоритмы STL"), currentProgram(1) {}

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
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
                if (event.key.code == sf::Keyboard::Num1) currentProgram = 1;
                if (event.key.code == sf::Keyboard::Num2) currentProgram = 2;
                if (event.key.code == sf::Keyboard::Num3) currentProgram = 3;
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }
    }

    void update() {
        switch (currentProgram) {
            case 1: updateProgram1(); break;
            case 2: updateProgram2(); break;
            case 3: updateProgram3(); break;
        }
    }

    void updateProgram1() {
        stringstream ss;
        ss << "ПРОГРАММА 1: Встроенные типы данных (float)\n";
        ss << "============================================\n\n";
        
        // 1. Создаем и заполняем multimap
        ss << "1. Создаем multimap<float, float>:\n";
        multimap<float, float> multiMap;
        for (int i = 0; i < 5; ++i) {
            float key = static_cast<float>(i);
            float value = static_cast<float>(rand() % 100) / 10.0f;
            multiMap.insert({key, value});
            ss << "   Ключ: " << key << " -> Значение: " << value << "\n";
        }
        
        // 2. Создаем и заполняем deque
        ss << "\n2. Создаем deque<float>:\n";
        deque<float> floatDeque;
        for (int i = 0; i < 6; ++i) {
            float val = static_cast<float>(rand() % 100) / 10.0f;
            floatDeque.push_back(val);
            ss << "   " << val;
        }
        
        // 3. Изменяем контейнеры
        ss << "\n\n3. Изменяем контейнеры:\n";
        ss << "   - Удаляем элемент с ключом 2 из multimap\n";
        ss << "   - Заменяем 2-й элемент в deque на 99.9\n";
        
        multiMap.erase(2.0f);
        if (floatDeque.size() > 1) {
            floatDeque[1] = 99.9f;
        }
        
        // 4. Просматриваем через итераторы
        ss << "\n4. Multimap через итераторы:\n";
        for (auto it = multiMap.begin(); it != multiMap.end(); ++it) {
            ss << "   Ключ: " << it->first << " -> Значение: " << it->second << "\n";
        }
        
        // 5. Создаем второй контейнер
        ss << "\n5. Создаем второй deque:\n";
        deque<float> secondDeque;
        for (int i = 0; i < 3; ++i) {
            float val = static_cast<float>(rand() % 50) / 10.0f + 50.0f;
            secondDeque.push_back(val);
            ss << "   " << val;
        }
        
        // 6. Изменяем первый deque
        ss << "\n\n6. Изменяем первый deque:\n";
        ss << "   - Удаляем 2 элемента после 3-го\n";
        ss << "   - Добавляем элементы из второго deque\n";
        
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
        
        // 7. Просматриваем результаты
        ss << "\n7. Итоговый multimap:\n";
        for (const auto& pair : multiMap) {
            ss << "   Ключ: " << pair.first << " -> Значение: " << pair.second << "\n";
        }
        
        ss << "\nИтоговый deque:\n";
        for (const auto& val : floatDeque) {
            ss << "   " << val;
        }
        
        ss << "\n\nВторой deque:\n";
        for (const auto& val : secondDeque) {
            ss << "   " << val;
        }
        
        ss << "\n\nНажмите 2 для перехода к Программе 2";
        ss << "\nНажмите 3 для перехода к Программе 3";
        
        outputText = ss.str();
    }

    void updateProgram2() {
        stringstream ss;
        ss << "ПРОГРАММА 2: Пользовательские типы данных (Student)\n";
        ss << "====================================================\n\n";
        
        // 1. Создаем и заполняем multimap
        ss << "1. Создаем multimap<float, Student>:\n";
        multimap<float, Student> studentMap;
        studentMap.insert({85.5f, Student("Иванов", 20, 85.5f)});
        studentMap.insert({92.3f, Student("Петров", 21, 92.3f)});
        studentMap.insert({78.9f, Student("Сидоров", 19, 78.9f)});
        studentMap.insert({85.5f, Student("Кузнецов", 22, 85.5f)});
        
        for (const auto& pair : studentMap) {
            ss << "   Оценка: " << pair.first << " - " << pair.second.toString() << "\n";
        }
        
        // 2. Создаем и заполняем deque
        ss << "\n2. Создаем deque<Student>:\n";
        deque<Student> studentDeque;
        studentDeque.push_back(Student("Смирнов", 20, 88.0f));
        studentDeque.push_back(Student("Попов", 21, 76.5f));
        studentDeque.push_back(Student("Волков", 19, 91.2f));
        studentDeque.push_back(Student("Орлов", 22, 83.7f));
        
        for (const auto& student : studentDeque) {
            ss << "   " << student.toString() << "\n";
        }
        
        // 3. Изменяем контейнеры
        ss << "\n3. Изменяем контейнеры:\n";
        ss << "   - Удаляем студентов с оценкой 85.5 из multimap\n";
        ss << "   - Заменяем 2-го студента в deque\n";
        
        studentMap.erase(85.5f);
        if (studentDeque.size() > 1) {
            studentDeque[1] = Student("Новиков", 23, 95.0f);
        }
        
        // 4. Создаем второй deque
        ss << "\n4. Создаем второй deque<Student>:\n";
        deque<Student> secondDeque;
        secondDeque.push_back(Student("Федоров", 20, 87.3f));
        secondDeque.push_back(Student("Никитин", 21, 79.8f));
        
        for (const auto& student : secondDeque) {
            ss << "   " << student.toString() << "\n";
        }
        
        // 5. Изменяем первый deque
        ss << "\n5. Изменяем первый deque:\n";
        ss << "   - Удаляем 1 элемент после 2-го\n";
        ss << "   - Добавляем элементы из второго deque\n";
        
        if (studentDeque.size() > 2) {
            auto it = studentDeque.begin();
            advance(it, 2);
            if (it != studentDeque.end()) {
                studentDeque.erase(it);
            }
        }
        
        studentDeque.insert(studentDeque.end(), secondDeque.begin(), secondDeque.end());
        
        // 6. Просматриваем результаты
        ss << "\n6. Итоговый multimap:\n";
        for (const auto& pair : studentMap) {
            ss << "   Оценка: " << pair.first << " - " << pair.second.toString() << "\n";
        }
        
        ss << "\nИтоговый deque:\n";
        for (const auto& student : studentDeque) {
            ss << "   " << student.toString() << "\n";
        }
        
        ss << "\nВторой deque:\n";
        for (const auto& student : secondDeque) {
            ss << "   " << student.toString() << "\n";
        }
        
        ss << "\n\nНажмите 1 для перехода к Программе 1";
        ss << "\nНажмите 3 для перехода к Программе 3";
        
        outputText = ss.str();
    }

    void updateProgram3() {
        static deque<float> numbers;
        static deque<float> secondContainer;
        static deque<float> thirdContainer;
        static int step = 0;
        
        stringstream ss;
        ss << "ПРОГРАММА 3: Алгоритмы STL\n";
        ss << "===========================\n\n";
        
        if (step == 0) {
            // Инициализация при первом запуске
            numbers.clear();
            for (int i = 0; i < 8; ++i) {
                numbers.push_back(static_cast<float>(rand() % 100));
            }
            secondContainer.clear();
            thirdContainer.clear();
        }
        
        ss << "Шаг " << step + 1 << ":\n\n";
        
        switch (step) {
            case 0:
                ss << "1. Исходный контейнер:\n";
                for (const auto& num : numbers) {
                    ss << "   " << num;
                }
                break;
                
            case 1:
                sort(numbers.rbegin(), numbers.rend());
                ss << "2. Отсортировано по убыванию:\n";
                for (const auto& num : numbers) {
                    ss << "   " << num;
                }
                break;
                
            case 2: {
                ss << "3. Поиск элемента > 70:\n";
                auto it = find_if(numbers.begin(), numbers.end(),
                    [](float x) { return x > 70.0f; });
                if (it != numbers.end()) {
                    ss << "   Найден: " << *it;
                } else {
                    ss << "   Элементы > 70 не найдены";
                }
                break;
            }
                
            case 3:
                copy_if(numbers.begin(), numbers.end(),
                       back_inserter(secondContainer),
                       [](float x) { return x > 70.0f; });
                ss << "4. Перенесены элементы > 70 во второй контейнер:\n";
                for (const auto& num : secondContainer) {
                    ss << "   " << num;
                }
                break;
                
            case 4:
                ss << "5. Второй контейнер:\n";
                for (const auto& num : secondContainer) {
                    ss << "   " << num;
                }
                break;
                
            case 5:
                sort(numbers.begin(), numbers.end());
                ss << "6. Первый контейнер отсортирован по возрастанию:\n";
                for (const auto& num : numbers) {
                    ss << "   " << num;
                }
                break;
                
            case 6:
                sort(secondContainer.begin(), secondContainer.end());
                ss << "7. Второй контейнер отсортирован по возрастанию:\n";
                for (const auto& num : secondContainer) {
                    ss << "   " << num;
                }
                break;
                
            case 7:
                ss << "8. Оба контейнера:\nПервый: ";
                for (const auto& num : numbers) ss << num << " ";
                ss << "\nВторой: ";
                for (const auto& num : secondContainer) ss << num << " ";
                break;
                
            case 8:
                thirdContainer.clear();
                merge(numbers.begin(), numbers.end(),
                     secondContainer.begin(), secondContainer.end(),
                     back_inserter(thirdContainer));
                ss << "9. Третий контейнер (слияние):\n";
                for (const auto& num : thirdContainer) {
                    ss << "   " << num;
                }
                break;
                
            case 9:
                ss << "10. Третий контейнер:\n";
                for (const auto& num : thirdContainer) {
                    ss << "   " << num;
                }
                break;
                
            case 10: {
                int count = count_if(thirdContainer.begin(), thirdContainer.end(),
                    [](float x) { return x > 70.0f; });
                ss << "11. Элементов > 70 в третьем контейнере: " << count;
                break;
            }
                
            case 11: {
                bool exists = any_of(thirdContainer.begin(), thirdContainer.end(),
                    [](float x) { return x > 70.0f; });
                ss << "12. В третьем контейнере " << (exists ? "есть" : "нет") << " элементов > 70";
                break;
            }
        }
        
        ss << "\n\nУправление:";
        ss << "\nПробел - следующий шаг";
        ss << "\nR - перезапуск программы";
        ss << "\n1,2,3 - переключение между программами";
        
        outputText = ss.str();
        
        // Обработка нажатия пробела
        if (keyClock.getElapsedTime().asMilliseconds() > 300) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                step = (step + 1) % 12;
                keyClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                step = 0;
                keyClock.restart();
            }
        }
    }

    void render() {
        window.clear(sf::Color::White);
        
        // Простой вывод текста без шрифта (через консоль + фигуры)
        sf::RectangleShape background(sf::Vector2f(980, 680));
        background.setPosition(10, 10);
        background.setFillColor(sf::Color(240, 240, 240));
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(2);
        
        window.draw(background);
        
        // Выводим информацию о текущей программе
        sf::Text programInfo;
        programInfo.setString("Текущая программа: " + to_string(currentProgram));
        programInfo.setCharacterSize(20);
        programInfo.setFillColor(sf::Color::Blue);
        programInfo.setPosition(20, 20);
        
        // Для простоты выводим текст в консоль
        cout << outputText << "\n---\n";
        
        window.display();
    }
};

int main() {
    cout << "Запуск программы. Используйте клавиши 1,2,3 для переключения между программами\n";
    ProgramManager manager;
    manager.run();
    return 0;
}
