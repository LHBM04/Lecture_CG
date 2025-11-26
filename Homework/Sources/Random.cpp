#include "Random.h"

std::random_device Random::rd;

std::mt19937 Random::gen(rd());