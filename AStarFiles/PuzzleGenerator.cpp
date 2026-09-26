#include "PuzzleGenerator.hpp"

#include <random>

using namespace std;

PuzzleState generateRandomPuzzle(int numberOfMoves)
{
    PuzzleState puzzle = getGoalState();

    random_device rd;
    mt19937 generator(rd());

    Move previousMove;
    bool hasPreviousMove = false;

    for (int i = 0; i < numberOfMoves; i++) {

        vector<pair<PuzzleState, Move>> neighbours =
            puzzle.getNeighbours();

        vector<pair<PuzzleState, Move>> validMoves;

        for (const auto& neighbour : neighbours) {

            Move move = neighbour.second;

            /*
                Don't immediately undo the previous move.
                This produces better scrambles.
            */
            if (hasPreviousMove) {

                bool inverse =
                    (previousMove == Move::Up &&
                     move == Move::Down) ||

                    (previousMove == Move::Down &&
                     move == Move::Up) ||

                    (previousMove == Move::Left &&
                     move == Move::Right) ||

                    (previousMove == Move::Right &&
                     move == Move::Left);

                if (inverse) {
                    continue;
                }
            }

            validMoves.push_back(neighbour);
        }

        uniform_int_distribution<int> distribution(
            0,
            static_cast<int>(validMoves.size()) - 1
        );

        auto chosen = validMoves[distribution(generator)];

        puzzle = chosen.first;
        previousMove = chosen.second;
        hasPreviousMove = true;
    }

    return puzzle;
}