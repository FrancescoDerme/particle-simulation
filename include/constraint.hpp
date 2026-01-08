#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include <cmath>

#include "constants.hpp"
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

    float satisfy() {
        if (!is_active) return 0.0;

        float p1_mass = p1->is_pinned ? 0.0f : 1.0f;
        float p2_mass = p2->is_pinned ? 0.0f : 1.0f;
        float mass_sum = p1_mass + p2_mass;
        if (mass_sum == 0.0f) return 0.0;

        sf::Vector2f delta = p2->position - p1->position;
        float current_length = std::hypot(delta.x, delta.y);
        float diff = current_length - initial_length;
        if (current_length < EPS) return std::abs(diff);

        /*
        if (current_length < CRITICAL_LENGTH) {
            float fix =
                (current_length - CRITICAL_LENGTH) / current_length;
            sf::Vector2f correction = delta * fix;

            if (!p1->is_pinned)
                p1->position += correction * (p1_mass / mass_sum);
            if (!p2->is_pinned)
                p2->position -= correction * (p2_mass / mass_sum);

            return ERROR_TOLERANCE_PIXELS + 1.0f;
        };
        */

        sf::Vector2f correction =
            delta * (diff / current_length * STIFFNESS);

        if (!p1->is_pinned)
            p1->position += correction * (p1_mass / mass_sum);
        if (!p2->is_pinned)
            p2->position -= correction * (p2_mass / mass_sum);

        return std::abs(diff);
    }

    void deactivate() { is_active = false; }
};

#endif  // CONSTRAINT_HPP
