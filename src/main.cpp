#include <SFML/Graphics.hpp>
#include <vector>

#include "constraint.hpp"
#include "input_handler.hpp"
#include "particle.hpp"

constexpr int WIDTH = 1080;
constexpr int HEIGHT = 640;
constexpr float GRAVITY = 9.81f;
constexpr float TIME_STEP = 0.1f;

constexpr int ROW = 10;
constexpr int COL = 10;
constexpr float REST_DISTANCE = 30.0f;

int main() {
    sf::Vector2u window_dimensions{WIDTH, HEIGHT};
    sf::RenderWindow window(sf::VideoMode(window_dimensions),
                            "Cloth simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    // Initialize particles
    for (std::size_t row = 0; row < ROW; ++row) {
        for (std::size_t col = 0; col < COL; ++col) {
            float x = col * REST_DISTANCE + WIDTH / 3.0;
            float y = row * REST_DISTANCE + HEIGHT / 3.0;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    // Initialize constraints
    for (std::size_t row = 0; row < ROW; ++row) {
        for (std::size_t col = 0; col < COL; ++col) {
            // Horizontal constraint
            if (col < COL - 1)
                constraints.emplace_back(&particles[row * COL + col],
                                         &particles[row * COL + col + 1]);
            // Vertical constraint
            if (row < ROW - 1)
                constraints.emplace_back(
                    &particles[row * COL + col],
                    &particles[(row + 1) * COL + col]);
        }
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed =
                         event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode ==
                    sf::Keyboard::Scancode::Escape)
                    window.close();
            }
            else if (const auto* mouseButtonPressed =
                         event->getIf<sf::Event::MouseButtonPressed>()) {
                // Handle mouse clicks
                InputHandler::handle_mouse_click(
                    *mouseButtonPressed, window, particles, constraints);
            }
        }

        // Apply gravity and update particles
        sf::Vector2f gravity_force(0, GRAVITY);
        for (auto& particle : particles) {
            particle.apply_force(gravity_force);
            particle.update(TIME_STEP);
            particle.constraint_to_bounds(WIDTH, HEIGHT);
        }

        // Apply constraints
        for (std::size_t i = 0; i < 5; ++i)
            for (auto& constraint : constraints) constraint.satisfy();

        // Clear the window with black color
        window.clear(sf::Color::Black);

        /*
        // Draw particles as balls
        for (const auto& particle : particles) {
            sf::CircleShape circle(PARTICLE_RADIUS);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(particle.position);
            window.draw(circle);
        }
        */

        // Draw particles as points
        for (const auto& particle : particles) {
            sf::Vertex point(particle.position, sf::Color::White);
            window.draw(&point, 1, sf::PrimitiveType::Points);
        }

        // Draw constraints as lines
        for (const auto& constraint : constraints) {
            if (!constraint.is_active) continue;

            sf::Vertex line[] = {
                sf::Vertex(constraint.p1->position, sf::Color::White),
                sf::Vertex(constraint.p2->position, sf::Color::White)};

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        window.display();
    }

    return 0;
}
