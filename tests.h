#pragma once
#include "movegen.h"
#include <cstdint>

uint64_t perft(Board& board, Color color, int depth);
void runTests();
