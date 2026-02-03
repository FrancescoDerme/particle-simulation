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
constexpr float SECONDARY_CONSTRAINT_OPACITY = 0.75f;

// Interactivity
constexpr float CLICK_TOLERANCE = 5.0f;
constexpr float STEP_SIZE = CLICK_TOLERANCE * 0.9f;

// Physics
constexpr float DILATION_STIFFNESS = 1.0f;
constexpr float CONTRACTION_STIFFNESS = 1.0f;
constexpr float SECONDARY_DILATION_STIFFNESS = 0.0001f;
constexpr float SECONDARY_CONTRACTION_STIFFNESS = 0.0001f;
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
constexpr std::size_t MAX_ITERATIONS = 5;
constexpr float ERROR_TOLERANCE_PIXELS = 0.5f;
constexpr std::size_t FRAMES_PER_SECOND = 60.f;
constexpr sf::Time TIME_PER_FRAME = sf::seconds(1.f / FRAMES_PER_SECOND);
constexpr float TIME_PER_FRAME_SEC = TIME_PER_FRAME.asSeconds();
constexpr float MAX_TIME_STEP = 0.125f;
constexpr std::size_t WARMUP_CYCLES = 2;

// File system
constexpr std::string_view FONT = "../resources/arial-font/arial.ttf";
constexpr std::string_view WARM_START_CACHE = "../warm_start.cache";
constexpr std::string_view SOURCE_FOLDER = "../src";
constexpr std::string_view INCLUDE_FOLDER = "../include";

#endif  // CONSTANTS_HPP
