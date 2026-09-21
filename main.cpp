#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <limits>

using namespace std;

// ============================================================
// Constants
// ============================================================

constexpr int BOARD_SIZE = 4;
constexpr int NUM_TILES = 16;


// ============================================================
// Move representation
// ============================================================

enum class Move {
    Up,
    Down,
    Left,
    Right
};

string moveToString(Move move)
{
    switch (move)
    {
        case Move::Up:    return "U";
        case Move::Down:  return "D";
        case Move::Left:  return "L";
        case Move::Right: return "R";
    }

    return "?";
}


// ============================================================
// PuzzleState
// ============================================================

class PuzzleState
{
private:
    array<int, NUM_TILES> tiles;

public:

    PuzzleState()
    {
        for (int i = 0; i < NUM_TILES - 1; ++i)
            tiles[i] = i + 1;

        tiles[NUM_TILES - 1] = 0;
    }

    explicit PuzzleState(const array<int, NUM_TILES>& values)
        : tiles(values)
    {
    }

    const array<int, NUM_TILES>& getTiles() const
    {
        return tiles;
    }

    int getBlankIndex() const
    {
        for (int i = 0; i < NUM_TILES; ++i)
        {
            if (tiles[i] == 0)
                return i;
        }

        return -1;
    }

    bool isGoal() const
    {
        for (int i = 0; i < NUM_TILES - 1; ++i)
        {
            if (tiles[i] != i + 1)
                return false;
        }

        return tiles[NUM_TILES - 1] == 0;
    }

    int get(int index) const
    {
        return tiles[index];
    }

    void print() const
    {
        cout << "\n";

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int col = 0; col < BOARD_SIZE; ++col)
            {
                int value = tiles[row * BOARD_SIZE + col];

                if (value == 0)
                    cout << "   ";
                else
                    cout << value << " ";

                if (col < BOARD_SIZE - 1)
                    cout << " ";
            }

            cout << "\n";
        }

        cout << "\n";
    }

    bool operator==(const PuzzleState& other) const
    {
        return tiles == other.tiles;
    }

    bool operator!=(const PuzzleState& other) const
    {
        return !(*this == other);
    }

    // --------------------------------------------------------
    // Generate all legal neighbouring states.
    // --------------------------------------------------------

    vector<pair<PuzzleState, Move>> getNeighbours() const
    {
        vector<pair<PuzzleState, Move>> result;

        int blank = getBlankIndex();

        int row = blank / BOARD_SIZE;
        int col = blank % BOARD_SIZE;

        // Move blank UP
        if (row > 0)
        {
            auto newTiles = tiles;
            swap(
                newTiles[blank],
                newTiles[blank - BOARD_SIZE]
            );

            result.push_back({
                PuzzleState(newTiles),
                Move::Up
            });
        }

        // Move blank DOWN
        if (row < BOARD_SIZE - 1)
        {
            auto newTiles = tiles;
            swap(
                newTiles[blank],
                newTiles[blank + BOARD_SIZE]
            );

            result.push_back({
                PuzzleState(newTiles),
                Move::Down
            });
        }

        // Move blank LEFT
        if (col > 0)
        {
            auto newTiles = tiles;
            swap(
                newTiles[blank],
                newTiles[blank - 1]
            );

            result.push_back({
                PuzzleState(newTiles),
                Move::Left
            });
        }

        // Move blank RIGHT
        if (col < BOARD_SIZE - 1)
        {
            auto newTiles = tiles;
            swap(
                newTiles[blank],
                newTiles[blank + 1]
            );

            result.push_back({
                PuzzleState(newTiles),
                Move::Right
            });
        }

        return result;
    }
};


// ============================================================
// Hash function for PuzzleState
// ============================================================

struct PuzzleStateHash
{
    size_t operator()(const PuzzleState& state) const
    {
        size_t hash = 0;

        for (int value : state.getTiles())
        {
            hash = hash * 31 + static_cast<size_t>(value);
        }

        return hash;
    }
};


// ============================================================
// Goal state
// ============================================================

PuzzleState getGoalState()
{
    return PuzzleState();
}


// ============================================================
// Heuristics
// ============================================================

enum class HeuristicType
{
    MisplacedTiles,
    Manhattan
};


// ------------------------------------------------------------
// Misplaced tiles heuristic
// ------------------------------------------------------------

int misplacedTiles(const PuzzleState& state)
{
    int misplaced = 0;

    for (int i = 0; i < NUM_TILES; ++i)
    {
        int value = state.get(i);

        // Do not count blank
        if (value == 0)
            continue;

        if (value != i + 1)
            ++misplaced;
    }

    return misplaced;
}


// ------------------------------------------------------------
// Manhattan distance heuristic
// ------------------------------------------------------------

int manhattanDistance(const PuzzleState& state)
{
    int distance = 0;

    for (int index = 0; index < NUM_TILES; ++index)
    {
        int value = state.get(index);

        // Ignore blank
        if (value == 0)
            continue;

        // Goal position of tile
        int goalIndex = value - 1;

        int currentRow = index / BOARD_SIZE;
        int currentCol = index % BOARD_SIZE;

        int goalRow = goalIndex / BOARD_SIZE;
        int goalCol = goalIndex % BOARD_SIZE;

        distance += abs(currentRow - goalRow);
        distance += abs(currentCol - goalCol);
    }

    return distance;
}


// ------------------------------------------------------------
// General heuristic function
// ------------------------------------------------------------

int heuristic(
    const PuzzleState& state,
    HeuristicType type
)
{
    switch (type)
    {
        case HeuristicType::MisplacedTiles:
            return misplacedTiles(state);

        case HeuristicType::Manhattan:
            return manhattanDistance(state);
    }

    return 0;
}


// ============================================================
// Solvability
// ============================================================

int countInversions(const PuzzleState& state)
{
    int inversions = 0;

    const auto& tiles = state.getTiles();

    for (int i = 0; i < NUM_TILES; ++i)
    {
        if (tiles[i] == 0)
            continue;

        for (int j = i + 1; j < NUM_TILES; ++j)
        {
            if (tiles[j] == 0)
                continue;

            if (tiles[i] > tiles[j])
                ++inversions;
        }
    }

    return inversions;
}


bool isSolvable(const PuzzleState& state)
{
    int inversions = countInversions(state);

    int blankIndex = state.getBlankIndex();

    int blankRowFromBottom =
        BOARD_SIZE - (blankIndex / BOARD_SIZE);

    // For even-width boards:
    //
    // If blank is on an even row from bottom,
    // number of inversions must be odd.
    //
    // If blank is on an odd row from bottom,
    // number of inversions must be even.

    if (BOARD_SIZE % 2 == 0)
    {
        if (blankRowFromBottom % 2 == 0)
            return inversions % 2 == 1;
        else
            return inversions % 2 == 0;
    }

    // For odd-width boards, even inversion count is solvable.
    return inversions % 2 == 0;
}


// ============================================================
// A* search node
// ============================================================

struct SearchNode
{
    PuzzleState state;

    int g;      // Cost from start
    int h;      // Estimated cost to goal
    int f;      // g + h

    shared_ptr<SearchNode> parent;

    optional<Move> move;

    SearchNode(
        const PuzzleState& state,
        int g,
        int h,
        shared_ptr<SearchNode> parent = nullptr,
        optional<Move> move = nullopt
    )
        : state(state),
          g(g),
          h(h),
          f(g + h),
          parent(parent),
          move(move)
    {
    }
};


// ============================================================
// Priority queue comparator
// ============================================================

struct NodeComparator
{
    bool operator()(
        const shared_ptr<SearchNode>& a,
        const shared_ptr<SearchNode>& b
    ) const
    {
        // priority_queue puts the "largest" item first,
        // so reverse the comparison to get smallest f first.

        if (a->f != b->f)
            return a->f > b->f;

        // Tie-break using smaller h.
        return a->h > b->h;
    }
};


// ============================================================
// Solver statistics
// ============================================================

struct SolverStats
{
    long long nodesExpanded = 0;
    long long nodesGenerated = 0;

    size_t maximumFrontierSize = 0;

    double runtimeMilliseconds = 0.0;
};


// ============================================================
// Solution
// ============================================================

struct Solution
{
    vector<Move> moves;
    SolverStats stats;
};


// ============================================================
// A* solver
// ============================================================

class AStarSolver
{
private:

    HeuristicType heuristicType;

public:

    explicit AStarSolver(HeuristicType type)
        : heuristicType(type)
    {
    }

    optional<Solution> solve(
        const PuzzleState& start
    )
    {
        SolverStats stats;

        auto startTime =
            chrono::high_resolution_clock::now();

        if (!isSolvable(start))
        {
            return nullopt;
        }

        // Priority queue containing states to explore.
        priority_queue<
            shared_ptr<SearchNode>,
            vector<shared_ptr<SearchNode>>,
            NodeComparator
        > open;

        // Best known g value for each state.
        unordered_map<
            PuzzleState,
            int,
            PuzzleStateHash
        > bestCost;

        int startH =
            heuristic(start, heuristicType);

        auto startNode =
            make_shared<SearchNode>(
                start,
                0,
                startH
            );

        open.push(startNode);

        bestCost[start] = 0;

        stats.maximumFrontierSize = open.size();

        while (!open.empty())
        {
            auto current = open.top();
            open.pop();

            // A stale node may remain in the priority queue
            // after a better path to the same state was found.
            auto bestIt = bestCost.find(current->state);

            if (bestIt != bestCost.end() &&
                current->g > bestIt->second)
            {
                continue;
            }

            ++stats.nodesExpanded;

            // ------------------------------------------------
            // Goal found
            // ------------------------------------------------

            if (current->state.isGoal())
            {
                vector<Move> moves;

                auto node = current;

                while (node->parent != nullptr)
                {
                    if (node->move.has_value())
                        moves.push_back(node->move.value());

                    node = node->parent;
                }

                reverse(moves.begin(), moves.end());

                auto endTime =
                    chrono::high_resolution_clock::now();

                stats.runtimeMilliseconds =
                    chrono::duration<double, milli>(
                        endTime - startTime
                    ).count();

                return Solution{
                    moves,
                    stats
                };
            }

            // ------------------------------------------------
            // Expand neighbours
            // ------------------------------------------------

            auto neighbours =
                current->state.getNeighbours();

            for (const auto& [nextState, move] : neighbours)
            {
                ++stats.nodesGenerated;

                int newG = current->g + 1;

                auto bestIt =
                    bestCost.find(nextState);

                // State has already been reached with
                // an equal or better path.
                if (bestIt != bestCost.end() &&
                    newG >= bestIt->second)
                {
                    continue;
                }

                // We found a better path.
                bestCost[nextState] = newG;

                int newH =
                    heuristic(
                        nextState,
                        heuristicType
                    );

                auto nextNode =
                    make_shared<SearchNode>(
                        nextState,
                        newG,
                        newH,
                        current,
                        move
                    );

                open.push(nextNode);

                stats.maximumFrontierSize =
                    max(
                        stats.maximumFrontierSize,
                        open.size()
                    );
            }
        }

        auto endTime =
            chrono::high_resolution_clock::now();

        stats.runtimeMilliseconds =
            chrono::duration<double, milli>(
                endTime - startTime
            ).count();

        return nullopt;
    }
};


// ============================================================
// Apply a move
// ============================================================

optional<PuzzleState> applyMove(
    const PuzzleState& state,
    Move move
)
{
    for (const auto& [nextState, generatedMove] :
         state.getNeighbours())
    {
        if (generatedMove == move)
            return nextState;
    }

    return nullopt;
}


// ============================================================
// Replay solution
// ============================================================

void replaySolution(
    const PuzzleState& start,
    const vector<Move>& moves
)
{
    PuzzleState current = start;

    cout << "\nInitial state:";
    current.print();

    for (size_t i = 0; i < moves.size(); ++i)
    {
        auto next = applyMove(
            current,
            moves[i]
        );

        if (!next.has_value())
        {
            cout << "ERROR: invalid solution move.\n";
            return;
        }

        current = next.value();

        cout << "Move "
             << i + 1
             << "/"
             << moves.size()
             << ": "
             << moveToString(moves[i])
             << "\n";

        current.print();
    }
}


// ============================================================
// Random puzzle generation
// ============================================================

PuzzleState generateRandomPuzzle(
    int scrambleMoves
)
{
    PuzzleState current = getGoalState();

    random_device rd;
    mt19937 generator(rd());

    optional<Move> previousMove;

    for (int i = 0; i < scrambleMoves; ++i)
    {
        auto neighbours =
            current.getNeighbours();

        // Avoid immediately undoing the previous move.
        vector<pair<PuzzleState, Move>> candidates;

        for (const auto& neighbour : neighbours)
        {
            Move move = neighbour.second;

            bool inverse = false;

            if (previousMove.has_value())
            {
                Move previous = previousMove.value();

                if ((previous == Move::Up &&
                     move == Move::Down) ||
                    (previous == Move::Down &&
                     move == Move::Up) ||
                    (previous == Move::Left &&
                     move == Move::Right) ||
                    (previous == Move::Right &&
                     move == Move::Left))
                {
                    inverse = true;
                }
            }

            if (!inverse)
                candidates.push_back(neighbour);
        }

        uniform_int_distribution<int> distribution(
            0,
            static_cast<int>(candidates.size()) - 1
        );

        auto selected =
            candidates[distribution(generator)];

        current = selected.first;
        previousMove = selected.second;
    }

    return current;
}


// ============================================================
// Input puzzle
// ============================================================

optional<PuzzleState> readPuzzle()
{
    array<int, NUM_TILES> values;

    cout << "\nEnter 16 numbers separated by spaces.\n";
    cout << "Use 0 for the blank.\n\n";

    for (int i = 0; i < NUM_TILES; ++i)
    {
        if (!(cin >> values[i]))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid input.\n";
            return nullopt;
        }
    }

    // Check values are in range.
    for (int value : values)
    {
        if (value < 0 || value >= NUM_TILES)
        {
            cout << "Values must be between 0 and 15.\n";
            return nullopt;
        }
    }

    // Check uniqueness.
    array<bool, NUM_TILES> seen{};

    for (int value : values)
    {
        if (seen[value])
        {
            cout << "Duplicate tile detected.\n";
            return nullopt;
        }

        seen[value] = true;
    }

    return PuzzleState(values);
}


// ============================================================
// Print solution
// ============================================================

void printSolution(
    const Solution& solution
)
{
    cout << "\n====================================\n";
    cout << "              SOLUTION\n";
    cout << "====================================\n\n";

    cout << "Solution length:      "
         << solution.moves.size()
         << "\n";

    cout << "Nodes expanded:       "
         << solution.stats.nodesExpanded
         << "\n";

    cout << "Nodes generated:      "
         << solution.stats.nodesGenerated
         << "\n";

    cout << "Maximum frontier:     "
         << solution.stats.maximumFrontierSize
         << "\n";

    cout << "Runtime:              "
         << solution.stats.runtimeMilliseconds
         << " ms\n";

    cout << "\nMoves:\n";

    for (const Move move : solution.moves)
    {
        cout << moveToString(move) << " ";
    }

    cout << "\n";
}


// ============================================================
// Main menu
// ============================================================

void printMenu()
{
    cout << "\n";
    cout << "====================================\n";
    cout << "         15-PUZZLE SOLVER\n";
    cout << "====================================\n";
    cout << "1. Enter puzzle\n";
    cout << "2. Generate random puzzle\n";
    cout << "3. Choose heuristic\n";
    cout << "4. Solve puzzle\n";
    cout << "5. Show current puzzle\n";
    cout << "6. Exit\n";
    cout << "====================================\n";
    cout << "Choice: ";
}


// ============================================================
// Main
// ============================================================

int main()
{
    PuzzleState currentPuzzle = getGoalState();

    HeuristicType selectedHeuristic =
        HeuristicType::Manhattan;

    optional<Solution> lastSolution;

    while (true)
    {
        printMenu();

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Invalid choice.\n";
            continue;
        }

        // ----------------------------------------------------
        // Enter puzzle
        // ----------------------------------------------------

        if (choice == 1)
        {
            auto puzzle = readPuzzle();

            if (puzzle.has_value())
            {
                currentPuzzle = puzzle.value();
                lastSolution.reset();

                cout << "\nPuzzle loaded:\n";
                currentPuzzle.print();

                if (isSolvable(currentPuzzle))
                    cout << "This puzzle is solvable.\n";
                else
                    cout << "This puzzle is NOT solvable.\n";
            }
        }

        // ----------------------------------------------------
        // Random puzzle
        // ----------------------------------------------------

        else if (choice == 2)
        {
            int scrambleMoves;

            cout << "\nScramble length: ";
            cin >> scrambleMoves;

            if (scrambleMoves < 1)
            {
                cout << "Scramble length must be positive.\n";
                continue;
            }

            currentPuzzle =
                generateRandomPuzzle(
                    scrambleMoves
                );

            lastSolution.reset();

            cout << "\nGenerated puzzle:\n";
            currentPuzzle.print();
        }

        // ----------------------------------------------------
        // Choose heuristic
        // ----------------------------------------------------

        else if (choice == 3)
        {
            cout << "\n";
            cout << "1. Misplaced tiles\n";
            cout << "2. Manhattan distance\n";
            cout << "Choice: ";

            int heuristicChoice;
            cin >> heuristicChoice;

            if (heuristicChoice == 1)
            {
                selectedHeuristic =
                    HeuristicType::MisplacedTiles;

                cout << "Using misplaced tiles.\n";
            }
            else if (heuristicChoice == 2)
            {
                selectedHeuristic =
                    HeuristicType::Manhattan;

                cout << "Using Manhattan distance.\n";
            }
            else
            {
                cout << "Invalid heuristic.\n";
            }
        }

        // ----------------------------------------------------
        // Solve
        // ----------------------------------------------------

        else if (choice == 4)
        {
            cout << "\nCurrent puzzle:";
            currentPuzzle.print();

            if (!isSolvable(currentPuzzle))
            {
                cout << "This puzzle is unsolvable.\n";
                continue;
            }

            cout << "Solving...\n";

            AStarSolver solver(selectedHeuristic);

            lastSolution =
                solver.solve(currentPuzzle);

            if (!lastSolution.has_value())
            {
                cout << "No solution found.\n";
            }
            else
            {
                printSolution(
                    lastSolution.value()
                );

                cout << "\nReplay solution? (y/n): ";

                char replay;
                cin >> replay;

                if (replay == 'y' ||
                    replay == 'Y')
                {
                    replaySolution(
                        currentPuzzle,
                        lastSolution->moves
                    );
                }
            }
        }

        // ----------------------------------------------------
        // Show puzzle
        // ----------------------------------------------------

        else if (choice == 5)
        {
            cout << "\nCurrent puzzle:";
            currentPuzzle.print();
        }

        // ----------------------------------------------------
        // Exit
        // ----------------------------------------------------

        else if (choice == 6)
        {
            cout << "Goodbye.\n";
            break;
        }

        else
        {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}