#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

// Graphics
constexpr int WIDTH = 1080;
constexpr int HEIGHT = 640;

// Interactivity
constexpr float CLICK_TOLERANCE = 5.0f;
constexpr float STEP_SIZE = CLICK_TOLERANCE * 0.9f;

// Physics
constexpr float STIFFNESS = 1.0f;
constexpr float EPS = 0.000001f;
constexpr float TIME_STEP = 0.1f;
constexpr float GRAVITY = 9.81f;
constexpr sf::Vector2f GRAVITY_FORCE(0, GRAVITY);

// Setup
constexpr int ROW = 10;
constexpr int COL = 10;
constexpr float REST_DISTANCE = 30.0f;

#endif  // CONSTANTS_HPP
