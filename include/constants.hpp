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
constexpr float STRAIN_COLOR_MULTIPLIER = 15.0f;

// Interactivity
constexpr float CLICK_TOLERANCE = 5.0f;
constexpr float STEP_SIZE = CLICK_TOLERANCE * 0.9f;

// Physics
constexpr float STIFFNESS = 1.0f;
constexpr float GRAVITY = 1000.0f;
constexpr sf::Vector2f GRAVITY_FORCE(0, GRAVITY);
constexpr float DAMPING = 0.995f;
constexpr float WALL_FRICTION = 0.05f;

// Setup
constexpr std::size_t ROW = 30;
constexpr std::size_t COL = 20;
constexpr float REST_DISTANCE = 20.0f;
constexpr float STARTING_X = WIDTH / 2.0 - COL * REST_DISTANCE / 2.0;
constexpr float STARTING_Y = HEIGHT / 2.0 - ROW * REST_DISTANCE / 2.0;

// Simulation
constexpr float EPS = 0.000001f;
constexpr float MAX_TIME_STEP = 0.25f;
constexpr std::size_t MAX_ITERATIONS = 100;
constexpr float ERROR_TOLERANCE_PIXELS = 0.5f;
constexpr sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
constexpr float TIME_PER_FRAME_SEC = TIME_PER_FRAME.asSeconds();
// constexpr float CRITICAL_LENGTH = 1.44f * REST_DISTANCE / 2.0f;

#endif  // CONSTANTS_HPP
