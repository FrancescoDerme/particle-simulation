#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <SFML/Graphics.hpp>

namespace math_utils {

float point_to_segment_distance(const float px, const float py,
                                const float x1, const float y1,
                                const float x2, const float y2);

sf::Vector2f lerp(const sf::Vector2f start, const sf::Vector2f end,
                  float t);

sf::Color lerpColor(const sf::Color& start, const sf::Color& end,
                    float t);

}  // namespace math_utils

#endif  // MATH_UTILS_HPP
