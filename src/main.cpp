#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "constants.hpp"
#include "constraint.hpp"
#include "input_handler.hpp"
#include "math_utils.hpp"
#include "particle.hpp"
#include "statushud.hpp"

int main() {
    sf::Vector2u window_dimensions{WIDTH, HEIGHT};
    sf::RenderWindow window(sf::VideoMode(window_dimensions),
                            "Cloth simulation");

    // Setup text HUD
    StatusHUD hud;
    if (!hud.init("../resources/arial-font/arial.ttf")) {
        return -1;
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
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

    sf::VertexArray particlePoints(sf::PrimitiveType::Points, ROW * COL);
    sf::VertexArray constraintLines(sf::PrimitiveType::Lines,
                                    constraints.size() * 2);

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

        // Calculate delta time
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        // Clamp large spikes
        // If the game freezes for too much time, don't try to simulate
        // physics all at once
        if (timeSinceLastUpdate > sf::seconds(MAX_TIME_STEP)) {
            timeSinceLastUpdate = sf::seconds(MAX_TIME_STEP);
        }

        timer += dt.asSeconds();
        frameCounter++;

        while (timeSinceLastUpdate >= TIME_PER_FRAME) {
            // Update the particles
            for (auto& particle : particles) {
                particle.update();
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

            timeSinceLastUpdate -= TIME_PER_FRAME;
        }

        if (timer >= TEXT_UPDATE_FREQUENCY) {
            float fps = frameCounter / timer;
            float cips = iterationCounter / timer;
            hud.update(StatusLine::FPS, "FPS",
                       static_cast<std::size_t>(fps));
            hud.update(StatusLine::Iterations, "CIPS", cips);
            hud.update(StatusLine::Error, "MAE", max_accepted_violation),

                timer = 0.0f;
            frameCounter = 0;
            iterationCounter = 0;
        }

        // Rendering
        float alpha =
            timeSinceLastUpdate.asSeconds() / TIME_PER_FRAME_SEC;

        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Draw particles as points
        for (std::size_t i = 0; i < particles.size(); ++i) {
            sf::Vector2f visualPos =
                math_utils::lerp(particles[i].previous_position,
                                 particles[i].position, alpha);

            particlePoints[i].position = visualPos;
            particlePoints[i].color = sf::Color::White;
        }

        window.draw(particlePoints);

        // Draw constraints as lines
        std::size_t lineIdx = 0;
        for (const auto& constraint : constraints) {
            if (!constraint.is_active) {
                constraintLines[lineIdx].color = sf::Color::Transparent;
                constraintLines[lineIdx + 1].color =
                    sf::Color::Transparent;
                lineIdx += 2;
                continue;
            }

            // Calculate strain color
            float t = STRAIN_COLOR_MULTIPLIER *
                      std::abs(constraint.compute_strain());
            sf::Color strain_color =
                math_utils::lerp(sf::Color::White, sf::Color::Red, t);

            sf::Vector2f v1 =
                math_utils::lerp(constraint.p1->previous_position,
                                 constraint.p1->position, alpha);
            sf::Vector2f v2 =
                math_utils::lerp(constraint.p2->previous_position,
                                 constraint.p2->position, alpha);

            constraintLines[lineIdx].position = v1;
            constraintLines[lineIdx].color = strain_color;
            constraintLines[lineIdx + 1].position = v2;
            constraintLines[lineIdx + 1].color = strain_color;

            lineIdx += 2;
        }

        window.draw(constraintLines);
        hud.draw(window);
        window.display();
    }

    return 0;
}
