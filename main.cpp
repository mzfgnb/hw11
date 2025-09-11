#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_MERIDIANS = 36; // Количество меридианов
const float PULSE_SPEED = 0.5f; // Скорость пульсации

// Структура для хранения информации о сегменте эллипса
struct EllipseSegment {
    sf::VertexArray vertices;
    sf::Color color;
};

class PulsingEllipse {
private:
    sf::Vector2f center;
    float baseRadiusX, baseRadiusY;
    float pulseFactor;
    float pulseTime;
    std::vector<EllipseSegment> segments;
    std::vector<sf::VertexArray> meridians;

public:
    PulsingEllipse(sf::Vector2f centerPos, float radiusX, float radiusY)
        : center(centerPos), baseRadiusX(radiusX), baseRadiusY(radiusY),
          pulseFactor(1.0f), pulseTime(0.0f) {}

    // Генерация случайного цвета
    sf::Color generateRandomColor() {
        return sf::Color(
            rand() % 200 + 55,  // R
            rand() % 200 + 55,  // G
            rand() % 200 + 55   // B
        );
    }

    // Обновление пульсации
    void update(float deltaTime) {
        pulseTime += deltaTime;
        // Синусоидальная пульсация от 0.5 до 1.5
        pulseFactor = 0.5f * sin(PULSE_SPEED * pulseTime) + 1.0f;
    }

    // Построение эллипса с меридианами
    void buildEllipse(int numPoints = 100) {
        segments.clear();
        meridians.clear();

        float currentRadiusX = baseRadiusX * pulseFactor;
        float currentRadiusY = baseRadiusY * pulseFactor;

        // Создание сегментов между меридианами
        for (int i = 0; i < NUM_MERIDIANS; i++) {
            float angle1 = 2 * M_PI * i / NUM_MERIDIANS;
            float angle2 = 2 * M_PI * (i + 1) / NUM_MERIDIANS;

            EllipseSegment segment;
            segment.color = generateRandomColor();
            segment.vertices.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
            
            // Центральная точка
            segment.vertices.append(sf::Vertex(center, segment.color));
            
            // Точки на эллипсе
            for (int j = 0; j <= numPoints / NUM_MERIDIANS; j++) {
                float t = angle1 + (angle2 - angle1) * j / (numPoints / NUM_MERIDIANS);
                float x = center.x + currentRadiusX * cos(t);
                float y = center.y + currentRadiusY * sin(t);
                segment.vertices.append(sf::Vertex(sf::Vector2f(x, y), segment.color));
            }
            
            segments.push_back(segment);
        }

        // Создание меридианов (линий)
        for (int i = 0; i < NUM_MERIDIANS; i++) {
            float angle = 2 * M_PI * i / NUM_MERIDIANS;
            float x = center.x + currentRadiusX * cos(angle);
            float y = center.y + currentRadiusY * sin(angle);

            sf::VertexArray meridian(sf::PrimitiveType::Lines, 2);
            meridian[0] = sf::Vertex(center, sf::Color::White);
            meridian[1] = sf::Vertex(sf::Vector2f(x, y), sf::Color::White);
            
            meridians.push_back(meridian);
        }
    }

    // Отрисовка эллипса
    void draw(sf::RenderWindow& window) {
        // Рисуем сегменты
        for (auto& segment : segments) {
            window.draw(segment.vertices);
        }
        
        // Рисуем меридианы
        for (auto& meridian : meridians) {
            window.draw(meridian);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pulsing Ellipse");
    window.setFramerateLimit(60);

    // Создаем пульсирующий эллипс в центре окна
    PulsingEllipse ellipse(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 200, 150);
    ellipse.buildEllipse();

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Обновление пульсации
        float deltaTime = clock.restart().asSeconds();
        ellipse.update(deltaTime);
        
        // Перестраиваем эллипс с новыми параметрами
        ellipse.buildEllipse();
        // Отрисовка
        window.clear(sf::Color::Black);
        ellipse.draw(window);
        window.display();
    }

    return 0;
}
