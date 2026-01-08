#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

// Graphics
constexpr std::size_t WIDTH = 1080;
constexpr std::size_t HEIGHT = 720;
constexpr float TEXT_UPDATE_FREQUENCY = 0.5f;
constexpr float TEXT_START_X = 10.0f;
constexpr float TEXT_START_Y = 10.0f;
constexpr float TEXT_SPACING = 40.0f;
constexpr float STRAIN_COLOR_MULTIPLIER = 50.0f;

// Interactivity
constexpr float CLICK_TOLERANCE = 5.0f;
constexpr float STEP_SIZE = CLICK_TOLERANCE * 0.9f;

// Physics
constexpr float STIFFNESS = 1.0f;
constexpr float GRAVITY = 1000.0f;
constexpr sf::Vector2f GRAVITY_FORCE(0, GRAVITY);
constexpr float DAMPING = 0.995f;
constexpr float WALL_FRICTION = 0.05f;

// Simulation
constexpr float EPS = 0.000001f;
constexpr float MAX_TIME_STEP = 0.25f;
constexpr std::size_t MAX_ITERATIONS = 100;
constexpr float ERROR_TOLERANCE = 0.02f;
constexpr sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
constexpr float TIME_PER_FRAME_SEC = TIME_PER_FRAME.asSeconds();

// Setup
constexpr std::size_t ROW = 20;
constexpr std::size_t COL = 10;
constexpr float REST_DISTANCE = 30.0f;
constexpr float STARTING_X = WIDTH / 2.0 - COL * REST_DISTANCE / 2.0;
constexpr float STARTING_Y = HEIGHT / 2.0 - ROW * REST_DISTANCE / 2.0;

#endif  // CONSTANTS_HPP
