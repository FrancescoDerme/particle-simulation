#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

// Graphics
constexpr std::size_t WIDTH = 1080;
constexpr std::size_t HEIGHT = 720;
constexpr float TEXT_UPDATE_FREQUENCY = 0.5f;

// Interactivity
constexpr float CLICK_TOLERANCE = 5.0f;
constexpr float STEP_SIZE = CLICK_TOLERANCE * 0.9f;

// Physics
constexpr float STIFFNESS = 1.0f;
constexpr float GRAVITY = 9.81f;
constexpr sf::Vector2f GRAVITY_FORCE(0, GRAVITY);

// Simulation
constexpr float EPS = 0.000001f;
constexpr float TIME_STEP = 0.1f;
constexpr std::size_t MAX_ITERATIONS = 200;
constexpr float ERROR_TOLERANCE = 0.01f;

// Setup
constexpr std::size_t ROW = 20;
constexpr std::size_t COL = 10;
constexpr float REST_DISTANCE = 10.0f;
constexpr float STARTING_X = WIDTH / 2.0 - COL * REST_DISTANCE / 2.0;
constexpr float STARTING_Y = HEIGHT / 2.0 - ROW * REST_DISTANCE / 2.0;

#endif  // CONSTANTS_HPP
