#ifndef STATUSHUD_HPP
#define STATUSHUD_HPP

#include <SFML/Graphics.hpp>
#include <constants.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class StatusLine { FPS, Iterations, Error, Count };

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

        // Initialize formatting
        for (int i = 0; i < totalLines; ++i) {
            text_lines.emplace_back(sf::Text(font));
            text_lines[i].setCharacterSize(36);
            text_lines[i].setFillColor(sf::Color::White);
            text_lines[i].setPosition(
                {TEXT_START_X, TEXT_START_Y + (i * TEXT_SPACING)});
        }

        return true;
    }

    template <typename T>
    void update(StatusLine line, const std::string& label,
                const T& value) {
        std::size_t index = static_cast<std::size_t>(line);
        std::ostringstream oss;
        oss << label << ": " << value;
        text_lines[index].setString(oss.str());
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& text : text_lines) {
            window.draw(text);
        }
    }

   private:
    sf::Font font;
    std::vector<sf::Text> text_lines;
};

#endif  // STATUSHUD_HPP
