#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <SFML/Graphics.hpp>

namespace math_utils {

float point_to_segment_distance(const float px, const float py,
                                const float x1, const float y1,
                                const float x2, const float y2);

template <typename T>
T lerp(const T& start, const T& end, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return start + (end - start) * t;
}

inline sf::Color lerp(const sf::Color& start, const sf::Color& end,
                      float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    return sf::Color(
        static_cast<std::uint8_t>(start.r + (end.r - start.r) * t),
        static_cast<std::uint8_t>(start.g + (end.g - start.g) * t),
        static_cast<std::uint8_t>(start.b + (end.b - start.b) * t),
        static_cast<std::uint8_t>(start.a + (end.a - start.a) * t));
}

}  // namespace math_utils

#endif  // MATH_UTILS_HPP
