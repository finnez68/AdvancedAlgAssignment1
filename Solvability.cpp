#include "Solvability.hpp"

int countInversions(const PuzzleState& state)
{
    int inversions = 0;

    for (int i = 0; i < 16; i++) {
        for (int j = i + 1; j < 16; j++) {

            int a = state.get(i);
            int b = state.get(j);

            // Ignore blank
            if (a == 0 || b == 0) {
                continue;
            }

            if (a > b) {
                inversions++;
            }
        }
    }

    return inversions;
}

bool isSolvable(const PuzzleState& state)
{
    int inversions = countInversions(state);

    int blankIndex = state.getBlankIndex();

    // Row of blank counted from bottom.
    // Bottom row = 1, next row = 2, etc.
    int blankRowFromBottom = 4 - (blankIndex / 4);

    /*
        For a 4x4 puzzle:

        Blank on odd row from bottom:
            inversions must be even

        Blank on even row from bottom:
            inversions must be odd
    */

    if (blankRowFromBottom % 2 == 1) {
        return inversions % 2 == 0;
    }
    else {
        return inversions % 2 == 1;
    }
}