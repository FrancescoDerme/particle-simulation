#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include <cmath>
#include <limits>

#include "SFML/System/Vector2.hpp"
#include "particle.hpp"

class Constraint {
   public:
    Particle *p1, *p2;
    float initial_length;

    Constraint(Particle* p1, Particle* p2)
        : p1{p1},
          p2{p2},
          initial_length{std::hypot(p2->position.x - p1->position.x,
                                    p2->position.y - p1->position.y)} {};

    void satisfy() {
        sf::Vector2f delta = p2->position - p1->position;
        float current_length = std::hypot(delta.x, delta.y);

        // Divide by current_length to normalize the delta inside
        // correction
        float diff = (current_length - initial_length) / current_length;
        sf::Vector2f correction = delta * 0.5f * diff;

        p1->position += correction;
        p2->position -= correction;
    }
};

#endif  // CONSTRAINT_HPP
