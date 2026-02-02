#ifndef WARM_START_HPP
#define WARM_START_HPP

#include "particle.hpp"

void saveWarmStart(const std::vector<Particle>& points);

bool loadWarmStart(std::vector<Particle>& points);

#endif  // WARM_START_HPP
