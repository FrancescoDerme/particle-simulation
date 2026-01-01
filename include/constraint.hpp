#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include <cmath>

#include "SFML/System/Vector2.hpp"
#include "particle.hpp"

class Constraint {
   public:
    Particle *p1, *p2;
    float initial_length;
    bool is_active;

    Constraint(Particle* p1, Particle* p2, bool is_active = true)
        : p1{p1},
          p2{p2},
          initial_length{std::hypot(p2->position.x - p1->position.x,
                                    p2->position.y - p1->position.y)},
          is_active{is_active} {};

    float compute_strain() const {
        sf::Vector2f delta = p2->position - p1->position;
        float current_length = std::hypot(delta.x, delta.y);
        return (current_length - initial_length) / initial_length;
    }

    void satisfy() {
        if (!is_active) return;

        sf::Vector2f delta = p2->position - p1->position;
        float current_length = std::hypot(delta.x, delta.y);

        // Divide by current_length to normalize the delta inside
        // correction
        float diff = (current_length - initial_length) / current_length;
        sf::Vector2f correction = delta * 0.5f * diff;

        if (!p1->is_pinned) p1->position += correction;
        if (!p2->is_pinned) p2->position -= correction;
    }

    void deactivate() { is_active = false; }
};

#endif  // CONSTRAINT_HPP
