#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <optional>

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
        pulseFactor = 0.5f * std::sin(PULSE_SPEED * pulseTime) + 1.0f;
    }

    // Построение эллипса с меридианами
    void buildEllipse(int numPoints = 100) {
        segments.clear();
        meridians.clear();

        float currentRadiusX = baseRadiusX * pulseFactor;
        float currentRadiusY = baseRadiusY * pulseFactor;

        for (int i = 0; i < NUM_MERIDIANS; i++) {
            float angle1 = 2 * M_PI * i / NUM_MERIDIANS;
            float angle2 = 2 * M_PI * (i + 1) / NUM_MERIDIANS;

            EllipseSegment segment;
            segment.color = generateRandomColor();
            segment.vertices.setPrimitiveType(sf::TrianglesFan);

            // Центральная точка
            sf::Vertex centerVertex;
            centerVertex.position = center;
            centerVertex.color = segment.color;
            segment.vertices.append(centerVertex);

            // Точки на эллипсе
            for (int j = 0; j <= numPoints / NUM_MERIDIANS; j++) {
                float t = angle1 + (angle2 - angle1) * j / (numPoints / NUM_MERIDIANS);
                float x = center.x + currentRadiusX * std::cos(t);
                float y = center.y + currentRadiusY * std::sin(t);

                sf::Vertex v;
                v.position = {x, y};
                v.color = segment.color;
                segment.vertices.append(v);
            }

            segments.push_back(segment);
        }

        // Создание меридианов
        for (int i = 0; i < NUM_MERIDIANS; i++) {
            float angle = 2 * M_PI * i / NUM_MERIDIANS;
            float x = center.x + currentRadiusX * std::cos(angle);
            float y = center.y + currentRadiusY * std::sin(angle);

            sf::VertexArray meridian(sf::Lines, 2);

            meridian[0].position = center;
            meridian[0].color = sf::Color::White;

            meridian[1].position = {x, y};
            meridian[1].color = sf::Color::White;

            meridians.push_back(meridian);
        }
    }

    // Отрисовка эллипса
    void draw(sf::RenderWindow& window) {
        for (auto& segment : segments) {
            window.draw(segment.vertices);
        }
        for (auto& meridian : meridians) {
            window.draw(meridian);
        }
    }
};

int main() {
    srand(time(nullptr));

    sf::VideoMode mode({WINDOW_WIDTH, WINDOW_HEIGHT});
    sf::RenderWindow window(mode, "Pulsing Ellipse");
    window.setFramerateLimit(60);

    PulsingEllipse ellipse({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f}, 200, 150);
    ellipse.buildEllipse();

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        ellipse.update(deltaTime);
        ellipse.buildEllipse();

        window.clear(sf::Color::Black);
        ellipse.draw(window);
        window.display();
    }

    return 0;
}
