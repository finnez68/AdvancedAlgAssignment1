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
 
// Counts linear conflicts: pairs of tiles that share their goal row (or
// goal column) with their current row (or column), but are ordered in
// reverse relative to their goal order. Each such pair needs at least
// 2 extra moves beyond Manhattan distance, since one tile must step out
// of the row/column, let the other pass, then step back in.
static int countLineConflicts(
    const PuzzleState& state,
    bool rows
)
{
    int conflicts = 0;
 
    for (int line = 0; line < 4; line++) {
        // Collect goal-positions (along the axis we're checking) of tiles
        // that belong in this row/column AND are currently in it.
        int goalPosInLine[4];
        int count = 0;
 
        for (int pos = 0; pos < 4; pos++) {
            int index = rows ? (line * 4 + pos) : (pos * 4 + line);
            int value = state.get(index);
 
            if (value == 0) {
                continue;
            }
 
            int goalIndex = value - 1;
            int goalRow = goalIndex / 4;
            int goalCol = goalIndex % 4;
            int goalLine = rows ? goalRow : goalCol;
            int goalPos = rows ? goalCol : goalRow;
 
            // Only tiles whose goal row/column matches the row/column
            // they're currently sitting in can be in conflict.
            if (goalLine == line) {
                goalPosInLine[count++] = goalPos;
            }
        }
 
        // Any pair that's out of order contributes one conflict.
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                if (goalPosInLine[i] > goalPosInLine[j]) {
                    conflicts++;
                }
            }
        }
    }
 
    return conflicts;
}
 
int manhattanLinearConflict(const PuzzleState& state)
{
    int distance = manhattanDistance(state);
 
    int rowConflicts = countLineConflicts(state, true);
    int colConflicts = countLineConflicts(state, false);
 
    // Each conflicting pair costs 2 extra moves.
    return distance + 2 * (rowConflicts + colConflicts);
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
 
        case HeuristicType::ManhattanLinearConflict:
            return manhattanLinearConflict(state);
    }
 
    return 0;
}