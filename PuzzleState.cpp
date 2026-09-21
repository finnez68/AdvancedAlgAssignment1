#include "PuzzleState.hpp"

#include <iostream>
#include <functional>

using namespace std;

const char* moveToString(Move move)
{
    switch (move) {
        case Move::Up:
            return "U";
        case Move::Down:
            return "D";
        case Move::Left:
            return "L";
        case Move::Right:
            return "R";
    }

    return "?";
}

PuzzleState::PuzzleState()
{
    for (int i = 0; i < 15; i++) {
        tiles[i] = i + 1;
    }

    tiles[15] = 0;
}

PuzzleState::PuzzleState(const array<int, 16>& tiles)
    : tiles(tiles)
{
}

const array<int, 16>& PuzzleState::getTiles() const
{
    return tiles;
}

int PuzzleState::getBlankIndex() const
{
    for (int i = 0; i < 16; i++) {
        if (tiles[i] == 0) {
            return i;
        }
    }

    return -1;
}

int PuzzleState::get(int index) const
{
    return tiles[index];
}

bool PuzzleState::isGoal() const
{
    return *this == getGoalState();
}

void PuzzleState::print() const
{
    cout << "\n";

    for (int i = 0; i < 16; i++) {
        if (tiles[i] == 0) {
            cout << "   ";
        }
        else {
            cout << tiles[i] << " ";
        }

        if ((i + 1) % 4 == 0) {
            cout << "\n";
        }
    }

    cout << "\n";
}

bool PuzzleState::operator==(const PuzzleState& other) const
{
    return tiles == other.tiles;
}

bool PuzzleState::operator!=(const PuzzleState& other) const
{
    return !(*this == other);
}

vector<pair<PuzzleState, Move>> PuzzleState::getNeighbours() const
{
    vector<pair<PuzzleState, Move>> neighbours;

    int blank = getBlankIndex();

    int row = blank / 4;
    int col = blank % 4;

    // Move blank up
    if (row > 0) {
        array<int, 16> newTiles = tiles;
        swap(newTiles[blank], newTiles[blank - 4]);

        neighbours.push_back({
            PuzzleState(newTiles),
            Move::Up
        });
    }

    // Move blank down
    if (row < 3) {
        array<int, 16> newTiles = tiles;
        swap(newTiles[blank], newTiles[blank + 4]);

        neighbours.push_back({
            PuzzleState(newTiles),
            Move::Down
        });
    }

    // Move blank left
    if (col > 0) {
        array<int, 16> newTiles = tiles;
        swap(newTiles[blank], newTiles[blank - 1]);

        neighbours.push_back({
            PuzzleState(newTiles),
            Move::Left
        });
    }

    // Move blank right
    if (col < 3) {
        array<int, 16> newTiles = tiles;
        swap(newTiles[blank], newTiles[blank + 1]);

        neighbours.push_back({
            PuzzleState(newTiles),
            Move::Right
        });
    }

    return neighbours;
}

size_t PuzzleStateHash::operator()(const PuzzleState& state) const
{
    size_t hash = 0;

    for (int tile : state.getTiles()) {
        hash = hash * 31 + std::hash<int>{}(tile);
    }

    return hash;
}

PuzzleState getGoalState()
{
    return PuzzleState();
}