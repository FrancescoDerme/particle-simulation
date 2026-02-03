#ifndef STATUSHUD_HPP
#define STATUSHUD_HPP

#include <SFML/Graphics.hpp>
#include <constants.hpp>
#include <iostream>

enum class StatusLine { FPS, Iterations, Error, Baking, Count };

class StatusHUD {
   public:
    bool init(const std::string& fontPath) {
        if (!font.openFromFile(fontPath)) {
            std::cerr << "Failed to load font: " << fontPath << std::endl;
            return false;
        }

        std::size_t totalLines =
            static_cast<std::size_t>(StatusLine::Count);
        text_lines.reserve(totalLines);
        lifespans.reserve(totalLines);

        // Initialize formatting
        for (int i = 0; i < totalLines; ++i) {
            text_lines.emplace_back(sf::Text(font));
            text_lines[i].setCharacterSize(36);
            text_lines[i].setFillColor(sf::Color::White);
            text_lines[i].setPosition(
                {TEXT_START_X, TEXT_START_Y + (i * TEXT_SPACING)});

            lifespans.push_back(-1.f);
        }

        return true;
    }

    template <typename T>
    void update(StatusLine line, const std::string& label, const T& value,
                const float duration = -1.f) {
        std::size_t index = static_cast<std::size_t>(line);
        std::ostringstream oss;
        oss << label << ": " << value;
        text_lines[index].setString(oss.str());

        lifespans[index] = duration;
    }

    void update(StatusLine line, const std::string& label,
                const float duration = -1.f) {
        std::size_t index = static_cast<std::size_t>(line);
        text_lines[index].setString(label);

        lifespans[index] = duration;
    }

    void refresh(const sf::Time deltaTime) {
        for (std::size_t i = 0; i < lifespans.size(); ++i) {
            // Only count down if lifespan is greater than 0
            if (lifespans[i] > 0.0f) {
                lifespans[i] -= deltaTime.asSeconds();

                // If time ran out, clear the string
                if (lifespans[i] <= 0.0f) {
                    text_lines[i].setString("");
                    lifespans[i] = -1.0f;
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        for (const auto& text : text_lines) {
            window.draw(text);
        }
    }

   private:
    sf::Font font;
    std::vector<sf::Text> text_lines;
    std::vector<float> lifespans;
};

#endif  // STATUSHUD_HPP
