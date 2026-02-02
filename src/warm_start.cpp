#include <fstream>
#include <iostream>
#include <warm_start.hpp>

#include "constants.hpp"

void saveWarmStart(const std::vector<Particle>& particles) {
    std::ofstream out(WARM_START_CACHE.data(), std::ios::binary);

    // Save the serialized positions for the warm start
    size_t count = particles.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& p : particles) {
        out.write(reinterpret_cast<const char*>(&p.position),
                  sizeof(p.position));
    }
}

bool loadWarmStart(std::vector<Particle>& particles) {
    std::ifstream in(WARM_START_CACHE.data(), std::ios::binary);
    if (!in.is_open()) return false;

    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    // Ensure cache size matches current grid size
    if (count != particles.size()) return false;

    for (auto& p : particles) {
        in.read(reinterpret_cast<char*>(&p.position), sizeof(p.position));

        // Reset old_position to kill velocity
        p.previous_position = p.position;
    }

    return true;
}
