#include "PuzzleState.hpp"
#include "Heuristic.hpp"
#include "Solvability.hpp"
#include "AStar.hpp"
#include "PuzzleGenerator.hpp"

#include <iostream>
#include <string>
#include <memory>

using namespace std;

void printSolution(const Solution& solution)
{
    if (solution.moves.empty()) {
        cout << "No solution found.\n";
        return;
    }

    cout << "\nSolution found!\n";

    cout << "Number of moves: "
         << solution.moves.size()
         << "\n";

    cout << "Moves: ";

    for (Move move : solution.moves) {
        cout << moveToString(move) << " ";
    }

    cout << "\n";

    cout << "Nodes expanded: "
         << solution.stats.nodesExpanded
         << "\n";

    cout << "Nodes generated: "
         << solution.stats.nodesGenerated
         << "\n";

    cout << "Maximum frontier size: "
         << solution.stats.maximumFrontierSize
         << "\n";

    cout << "Runtime: "
         << solution.stats.runtimeMilliseconds
         << " ms\n";
}

PuzzleState readPuzzle()
{
    array<int, 16> tiles;

    cout << "Enter the puzzle row by row.\n";
    cout << "Use 0 for the blank.\n";

    for (int i = 0; i < 16; i++) {
        cin >> tiles[i];
    }

    return PuzzleState(tiles);
}

void printMenu()
{
    cout << "\n";
    cout << "===== 15-PUZZLE SOLVER =====\n";
    cout << "1. Enter puzzle\n";
    cout << "2. Generate random puzzle\n";
    cout << "3. Choose heuristic\n";
    cout << "4. Solve puzzle\n";
    cout << "5. Show current puzzle\n";
    cout << "6. Exit\n";
    cout << "Choice: ";
}

int main()
{
    PuzzleState currentPuzzle;

    HeuristicType heuristicType =
        HeuristicType::Manhattan;

    bool running = true;

    while (running) {

        printMenu();

        int choice;
        cin >> choice;

        switch (choice) {

            case 1:
                currentPuzzle = readPuzzle();

                cout << "\nPuzzle loaded:\n";
                currentPuzzle.print();
                break;

            case 2:
                currentPuzzle =
                    generateRandomPuzzle(30);

                cout << "\nGenerated puzzle:\n";
                currentPuzzle.print();
                break;

            case 3:
            {
                cout << "\n";
                cout << "1. Misplaced tiles\n";
                cout << "2. Manhattan distance\n";
                cout << "3. Manhattan-linear conflict\n";
                cout << "Choice: ";

                int heuristicChoice;
                cin >> heuristicChoice;

                if (heuristicChoice == 1) {
                    heuristicType =
                        HeuristicType::MisplacedTiles;

                    cout << "Using misplaced tiles.\n";
                }
                else if (heuristicChoice == 2) {
                    heuristicType =
                        HeuristicType::Manhattan;

                    cout << "Using Manhattan distance.\n";
                }
                else if (heuristicChoice == 3){
                    heuristicType =
                        HeuristicType::ManhattanLinearConflict;
                    cout << "Using Manhattan-linear conflict.\n";
                }
                else {
                    cout << "Invalid choice.\n";
                }

                break;
            }

            case 4:
            {
                cout << "\nChecking puzzle...\n";

                if (!isSolvable(currentPuzzle)) {
                    cout << "This puzzle is not solvable.\n";
                    break;
                }

                cout << "Solving...\n";

                AStarSolver solver(heuristicType);

                Solution solution =
                    solver.solve(currentPuzzle);

                printSolution(solution);

                break;
            }

            case 5:
                currentPuzzle.print();
                break;

            case 6:
                running = false;
                break;

            default:
                cout << "Invalid choice.\n";
                break;
        }
    }

    return 0;
}