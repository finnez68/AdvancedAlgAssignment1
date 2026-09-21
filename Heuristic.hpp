#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "PuzzleState.hpp"

enum class HeuristicType {
    MisplacedTiles,
    Manhattan
};

int misplacedTiles(const PuzzleState& state);
int manhattanDistance(const PuzzleState& state);

int heuristic(
    const PuzzleState& state,
    HeuristicType type
);

#endif