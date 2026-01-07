#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "constants.hpp"
#include "constraint.hpp"
#include "input_handler.hpp"
#include "math_utils.hpp"
#include "particle.hpp"

int main() {
    sf::Vector2u window_dimensions{WIDTH, HEIGHT};
    sf::RenderWindow window(sf::VideoMode(window_dimensions),
                            "Cloth simulation");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.openFromFile("../resources/arial-font/arial.ttf")) {
        return -1;
    }

    // Setup text
    sf::Text fpsText(font), iterationText(font), errorText(font);
    fpsText.setCharacterSize(36);
    iterationText.setCharacterSize(36);
    errorText.setCharacterSize(36);
    fpsText.setPosition({10.0f, 10.0f});
    iterationText.setPosition({10.0f, 50.0f});
    errorText.setPosition({10.0f, 90.0f});
    fpsText.setFillColor(sf::Color::White);
    iterationText.setFillColor(sf::Color::White);
    errorText.setFillColor(sf::Color::White);

    sf::Clock clock;
    float timer = 0.0f, max_accepted_violation = 0.0f;
    std::size_t frameCounter = 0, iterationCounter = 0;

    std::vector<Particle> particles;
    particles.reserve(ROW * COL);

    std::vector<Constraint> constraints;

    // Initialize particles
    for (std::size_t row = 0; row < ROW; ++row) {
        for (std::size_t col = 0; col < COL; ++col) {
            float x = col * REST_DISTANCE + STARTING_X;
            float y = row * REST_DISTANCE + STARTING_Y;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, GRAVITY_FORCE, pinned);
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

    bool holding_left_click = false, first_frame_of_click = false;
    sf::Vector2f lastMousePos, currentMousePos;

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
            else if (event->is<sf::Event::Resized>()) {
                // Update view to fix stretching
                sf::View view({WIDTH / 2.0f, HEIGHT / 2.0f},
                              sf::Vector2f{window.getSize()});
                window.setView(view);
            }
            else if (const auto* mouseButtonPressed =
                         event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button ==
                    sf::Mouse::Button::Left) {
                    holding_left_click = true;
                    first_frame_of_click = true;
                }
            }
            else if (const auto* mouseButtonReleased =
                         event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button ==
                    sf::Mouse::Button::Left) {
                    holding_left_click = false;
                }
            }
        }

        if (holding_left_click) {
            // Convert from pixel position to world coordinates
            currentMousePos =
                window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (first_frame_of_click) {
                InputHandler::handle_mouse_click(currentMousePos,
                                                 particles, constraints);
                first_frame_of_click = false;
            }
            else {
                sf::Vector2f difference = currentMousePos - lastMousePos;
                float distance = std::sqrt(difference.x * difference.x +
                                           difference.y * difference.y);

                if (distance > CLICK_TOLERANCE) {
                    sf::Vector2f direction = difference / distance;

                    for (float i = 0; i < distance; i += STEP_SIZE) {
                        sf::Vector2f intermediatePos =
                            lastMousePos + direction * i;
                        InputHandler::handle_mouse_click(
                            intermediatePos, particles, constraints);
                    }
                }

                InputHandler::handle_mouse_click(currentMousePos,
                                                 particles, constraints);
            }

            lastMousePos = currentMousePos;
        }

        // Update the particles
        for (auto& particle : particles) {
            particle.update(TIME_STEP);
            particle.constraint_to_bounds(WIDTH, HEIGHT);
        }

        // Apply constraints
        float max_violation;
        for (std::size_t i = 0; i < MAX_ITERATIONS; ++i) {
            max_violation = 0.0f;
            for (auto& constraint : constraints) {
                max_violation =
                    std::max(max_violation, constraint.satisfy());
            }

            ++iterationCounter;
            if (max_violation < ERROR_TOLERANCE) break;
        }

        max_accepted_violation =
            std::max(max_accepted_violation, max_violation);

        // Calculate time
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        timer += dt;
        frameCounter++;

        if (timer >= TEXT_UPDATE_FREQUENCY) {
            float fps = frameCounter / timer;
            float cips = iterationCounter / timer;
            fpsText.setString("FPS: " +
                              std::to_string(static_cast<int>(fps)));

            iterationText.setString(
                "CIPS: " + std::to_string(static_cast<int>(cips)));

            errorText.setString("MAE: " +
                                std::to_string(max_accepted_violation));

            timer = 0.0f;
            frameCounter = 0;
            iterationCounter = 0;
        }

        // Rendering
        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Draw particles as points
        sf::VertexArray particlePoints(sf::PrimitiveType::Points);
        for (const auto& particle : particles) {
            particlePoints.append(
                sf::Vertex(particle.position, sf::Color::White));
        }

        window.draw(particlePoints);

        // Draw constraints as lines
        sf::VertexArray constraintLines(sf::PrimitiveType::Lines);
        for (const auto& constraint : constraints) {
            if (!constraint.is_active) continue;

            // Calculate strain color
            float t = 35.0f * std::abs(constraint.compute_strain());
            sf::Color strain_color = math_utils::lerpColor(
                sf::Color::White, sf::Color::Red, t);

            constraintLines.append(
                sf::Vertex(constraint.p1->position, strain_color));
            constraintLines.append(
                sf::Vertex(constraint.p2->position, strain_color));
        }

        window.draw(constraintLines);
        window.draw(fpsText);
        window.draw(iterationText);
        window.draw(errorText);
        window.display();
    }

    return 0;
}
