#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <optional>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_MERIDIANS = 36;
const float PULSE_SPEED = 0.5f;
constexpr float PI = 3.14159265358979323846f;

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
          pulseFactor(1.f), pulseTime(0.f) {}

    sf::Color generateRandomColor() {
        return sf::Color(
            rand() % 200 + 55,
            rand() % 200 + 55,
            rand() % 200 + 55
        );
    }

    void update(float deltaTime) {
        pulseTime += deltaTime;
        pulseFactor = 0.5f * std::sin(PULSE_SPEED * pulseTime) + 1.0f;
    }

    void buildEllipse(int numPoints = 100) {
        if (numPoints < NUM_MERIDIANS) numPoints = NUM_MERIDIANS;
        segments.clear();
        meridians.clear();

        float currentRadiusX = baseRadiusX * pulseFactor;
        float currentRadiusY = baseRadiusY * pulseFactor;

        // Создаём сегменты (через TriangleFan — один фан на сегмент)
        for (int i = 0; i < NUM_MERIDIANS; ++i) {
            float angle1 = 2.f * PI * i / NUM_MERIDIANS;
            float angle2 = 2.f * PI * (i + 1) / NUM_MERIDIANS;

            EllipseSegment segment;
            segment.color = generateRandomColor();
            segment.vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);

            // центр
            sf::Vertex vCenter;
            vCenter.position = center;
            vCenter.color = segment.color;
            segment.vertices.append(vCenter);

            int pointsPerSeg = numPoints / NUM_MERIDIANS;
            for (int j = 0; j <= pointsPerSeg; ++j) {
                float t = angle1 + (angle2 - angle1) * (float)j / (float)pointsPerSeg;
                float x = center.x + currentRadiusX * std::cos(t);
                float y = center.y + currentRadiusY * std::sin(t);

                sf::Vertex v;
                v.position = { x, y };
                v.color = segment.color;
                segment.vertices.append(v);
            }

            segments.push_back(std::move(segment));
        }

        // Меридианы — простые линии (каждая как VertexArray с 2 вершинами)
        for (int i = 0; i < NUM_MERIDIANS; ++i) {
            float angle = 2.f * PI * i / NUM_MERIDIANS;
            float x = center.x + currentRadiusX * std::cos(angle);
            float y = center.y + currentRadiusY * std::sin(angle);

            sf::VertexArray meridian(sf::PrimitiveType::Lines, 2);

            meridian[0].position = center;
            meridian[0].color = sf::Color::White;

            meridian[1].position = { x, y };
            meridian[1].color = sf::Color::White;

            meridians.push_back(std::move(meridian));
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& seg : segments) window.draw(seg.vertices);
        for (auto& m : meridians) window.draw(m);
    }
};

int main() {
    srand((unsigned)time(nullptr));

    sf::VideoMode vm({ WINDOW_WIDTH, WINDOW_HEIGHT });
    sf::RenderWindow window(vm, "Pulsing Ellipse");
    window.setFramerateLimit(60);

    PulsingEllipse ellipse({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f }, 200.f, 150.f);
    ellipse.buildEllipse();

    sf::Clock clock;
    while (window.isOpen()) {
        // SFML3: pollEvent возвращает std::optional<sf::Event>
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        float dt = clock.restart().asSeconds();
        ellipse.update(dt);
        ellipse.buildEllipse();

        window.clear(sf::Color::Black);
        ellipse.draw(window);
        window.display();
    }
    return 0;
}
