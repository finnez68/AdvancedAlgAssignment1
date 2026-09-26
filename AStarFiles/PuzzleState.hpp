#ifndef PUZZLE_STATE_H
#define PUZZLE_STATE_H

#include <array>
#include <vector>

enum class Move {
    Up,
    Down,
    Left,
    Right
};

const char* moveToString(Move move);

class PuzzleState {
private:
    std::array<int, 16> tiles;

public:
    PuzzleState();
    PuzzleState(const std::array<int, 16>& tiles);

    const std::array<int, 16>& getTiles() const;

    int getBlankIndex() const;
    int get(int index) const;

    bool isGoal() const;

    void print() const;

    bool operator==(const PuzzleState& other) const;
    bool operator!=(const PuzzleState& other) const;

    std::vector<std::pair<PuzzleState, Move>> getNeighbours() const;
};

struct PuzzleStateHash {
    std::size_t operator()(const PuzzleState& state) const;
};

PuzzleState getGoalState();

#endif