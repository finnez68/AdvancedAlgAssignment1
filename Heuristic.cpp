#include "Heuristic.hpp"

#include <cstdlib>

using namespace std;

int misplacedTiles(const PuzzleState& state)
{
    int count = 0;

    for (int i = 0; i < 16; i++) {
        int value = state.get(i);

        // Ignore the blank
        if (value != 0 && value != i + 1) {
            count++;
        }
    }

    return count;
}

int manhattanDistance(const PuzzleState& state)
{
    int distance = 0;

    for (int i = 0; i < 16; i++) {
        int value = state.get(i);

        // Ignore blank
        if (value == 0) {
            continue;
        }

        int currentRow = i / 4;
        int currentCol = i % 4;

        int goalIndex = value - 1;

        int goalRow = goalIndex / 4;
        int goalCol = goalIndex % 4;

        distance += abs(currentRow - goalRow);
        distance += abs(currentCol - goalCol);
    }

    return distance;
}

int heuristic(
    const PuzzleState& state,
    HeuristicType type
)
{
    switch (type) {
        case HeuristicType::MisplacedTiles:
            return misplacedTiles(state);

        case HeuristicType::Manhattan:
            return manhattanDistance(state);
    }

    return 0;
}