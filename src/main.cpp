#include <SFML/Graphics.hpp>
#include <vector>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "particle.hpp"

constexpr int WIDTH = 1080;
constexpr int HEIGHT = 720;
constexpr float PARTICLE_RADIUS = 30.0f;
constexpr float GRAVITY = 9.8f;
constexpr float TIME_STEP = 0.01f;

int main() {
    sf::Vector2u window_dimensions{WIDTH, HEIGHT};
    sf::RenderWindow window(sf::VideoMode(window_dimensions),
                            "Cloth simulation");

    std::vector<Particle> particles;
    particles.emplace_back(WIDTH / 2, HEIGHT / 2);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed =
                         event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        sf::Vector2f gravity_force(0, GRAVITY);
        for (auto& particle : particles) {
            particle.apply_force(gravity_force);
            particle.update(TIME_STEP);
        }

        window.clear(sf::Color::Black);

        for (const auto& particle : particles) {
            sf::CircleShape circle(PARTICLE_RADIUS);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(particle.position);
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
