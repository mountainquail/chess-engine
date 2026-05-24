#include "board.h"
#include "attacks.h"
#include "movegen.h"
#include "tests.h"
#include <iostream>

int main() {
    generateKnightAttacks();
    generateKingAttacks();
    runTests();
    return 0;
}
