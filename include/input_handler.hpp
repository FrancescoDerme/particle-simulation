#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "constraint.hpp"
#include "math_utils.hpp"
#include "particle.hpp"

class InputHandler {
   public:
    static void handle_mouse_click(const sf::Vector2f& mousePos,
                                   const std::vector<Particle>& particles,
                                   std::vector<Constraint>& constraints) {
        tear_cloth(mousePos.x, mousePos.y, particles, constraints);
    }

   private:
    static Constraint* find_nearest_constraint(
        const float mouse_x, const float mouse_y,
        const std::vector<Constraint>& constraints) {
        Constraint* nearest = nullptr;
        float min_distance = CLICK_TOLERANCE;

        for (const auto& constraint : constraints) {
            float distance = math_utils::point_to_segment_distance(
                mouse_x, mouse_y, constraint.p1->position.x,
                constraint.p1->position.y, constraint.p2->position.x,
                constraint.p2->position.y);

            if (distance < min_distance) {
                min_distance = distance;
                nearest = const_cast<Constraint*>(&constraint);
            }
        }

        return nearest;
    }

    static void tear_cloth(const float mouse_x, const float mouse_y,
                           const std::vector<Particle>& particles,
                           std::vector<Constraint>& constraints) {
        Constraint* nearest =
            find_nearest_constraint(mouse_x, mouse_y, constraints);
        if (nearest) nearest->deactivate();
    }
};

#endif  // INPUT_HANDLER_HPP
