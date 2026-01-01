#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>

#include "SFML/System/Vector2.hpp"

class Particle {
   public:
    sf::Vector2f position, previous_position, acceleration;

    Particle(float x, float y)
        : position{x, y}, previous_position(x, y), acceleration(0, 0) {}

    void apply_force(const sf::Vector2f& force) { acceleration += force; }

    void update(float time_step) {
        // Verlet intehration: r_(n+1) = 2r_n - r_(n-1) + a_n * dt^2
        sf::Vector2f velocity = position - previous_position;
        previous_position = position;
        position += velocity + acceleration * time_step * time_step;
        acceleration = sf::Vector2f(0, 0);
    }

    void constraint_to_bounds(float width, float height, float radius) {
        const float diameter = 2 * radius;

        if (position.x < 0)
            position.x = 0;
        else if (position.x > width - diameter)
            position.x = width - diameter;

        if (position.y < 0)
            position.y = 0;
        else if (position.y > height - diameter)
            position.y = height - diameter;
    }
};

#endif  // PARTICLE_HPP
