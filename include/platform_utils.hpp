#ifndef PLATFORM_UTILS_HPP
#define PLATFORM_UTILS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <filesystem>

namespace fs = std::filesystem;

void toggleFullscreen(sf::RenderWindow& window);

fs::file_time_type getLatestFolderTime(const fs::path& path);

#endif  // PLATFORM_UTILS_HPP
