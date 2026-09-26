# AdvancedAlgAssignment1
# A* 15-Puzzle Solver

## Overview

This project implements an **A* search algorithm** to solve the 15-puzzle.

The 15-puzzle consists of 15 numbered tiles and one blank space arranged on a 4 × 4 board. The objective is to move the tiles until they reach the goal configuration:

```text
 1  2  3  4
 5  6  7  8
 9 10 11 12
13 14 15
```

The blank space is represented by `0`.

The project was developed as an algorithm implementation project and focuses on implementing A*, different heuristic functions, puzzle solvability checking, and a command-line interface.

---

## Features

* A* search algorithm
* 4 × 4 15-puzzle representation
* Two heuristic functions:

  * Misplaced tiles
  * Manhattan distance
* Solvability checking using inversion count and blank-row position
* Random solvable puzzle generation
* Manual puzzle input
* Solution reconstruction
* Search statistics
* Command-line interface
* Multiple source files separating different components of the implementation

The solver reports:

* Solution length
* Moves required
* Number of nodes expanded
* Number of nodes generated
* Maximum frontier size
* Runtime

---

## Project Structure

```text
AdvancedAlgAssignment1/
├── AStarFiles/
│   ├── main.cpp
│   ├── PuzzleState.h
│   ├── PuzzleState.cpp
│   ├── Heuristic.h
│   ├── Heuristic.cpp
│   ├── Solvability.h
│   ├── Solvability.cpp
│   ├── AStar.h
│   ├── AStar.cpp
│   ├── PuzzleGenerator.h
│   └── PuzzleGenerator.cpp
├── README.md
└── AStar.exe
```

### `PuzzleState`

Represents a puzzle configuration and is responsible for:

* Storing the board
* Finding the blank tile
* Checking whether the puzzle is solved
* Printing the board
* Generating legal neighbouring states
* Providing hashing and equality operations

The puzzle is represented using:

```cpp
std::array<int, 16>
```

where `0` represents the blank space.

### `Heuristic`

Contains the heuristic functions used by A*:

* Misplaced tiles
* Manhattan distance

### `Solvability`

Determines whether a given 15-puzzle configuration can be solved.

This uses the number of inversions and the position of the blank tile.

### `AStar`

Contains the main A* search implementation.

The algorithm uses:

```text
f(n) = g(n) + h(n)
```

where:

* `g(n)` is the cost of reaching the current state
* `h(n)` is the estimated cost to the goal
* `f(n)` is the total priority used by A*

### `PuzzleGenerator`

Generates random puzzles by starting from the solved configuration and performing a number of legal moves.

Because the generated puzzle is reached through legal moves from the goal, it is guaranteed to be solvable.

### `main`

Provides the command-line interface and allows the user to:

1. Enter a puzzle
2. Generate a random puzzle
3. Select a heuristic
4. Solve the puzzle
5. Display the current puzzle
6. Exit

---

## Requirements

The project requires:

* C++17 or later
* A C++ compiler such as MinGW `g++`

The implementation uses standard C++ libraries and does not require external dependencies.

---

## Compilation

Can run AStar.exe directly if you do not want to compile files.

Otherwise:
From the project root directory, compile all `.cpp` files with:

```bash
g++ -std=c++17 -O2 src/AStarFiles/*.cpp -o puzzle_solver.exe
```

Alternatively, if the terminal is already inside the `src` directory:

```bash
g++ -std=c++17 -O2 *.cpp -o puzzle_solver.exe
```

---

## Running

On Windows:

```bash
.\AStar.exe
```

The program will display:

```text
===== 15-PUZZLE SOLVER =====
1. Enter puzzle
2. Generate random puzzle
3. Choose heuristic
4. Solve puzzle
5. Show current puzzle
6. Exit
Choice:
```

---

## Using the Solver

### Entering a Puzzle

Select:

```text
1. Enter puzzle
```

Then enter the 16 values row by row.

Use `0` for the blank space.

For example:

```text
1 2 3 4
5 6 7 8
9 10 11 12
13 14 0 15
```

This puzzle requires one move to reach the goal.

---

### Generating a Random Puzzle

Select:

```text
2. Generate random puzzle
```

The program starts with the goal state and performs a series of legal moves to create a scrambled puzzle.

This guarantees that the generated puzzle is solvable.

---

### Selecting a Heuristic

Select:

```text
3. Choose heuristic
```

The available heuristics are:

#### Misplaced Tiles

Counts the number of tiles that are not in their goal position.

```text
h(n) = number of misplaced tiles
```

The blank space is ignored.

#### Manhattan Distance

Calculates the total number of horizontal and vertical moves each tile is away from its goal position.

```text
h(n) = sum of Manhattan distances for all tiles
```

The blank space is ignored.

---

### Solving

Select:

```text
4. Solve puzzle
```

The program first checks whether the puzzle is solvable.

If it is solvable, A* searches for a solution and reports the results.

Example:

```text
Solution found!

Number of moves: 1
Moves: R
Nodes expanded: 1
Nodes generated: 3
Maximum frontier size: 3
Runtime: 0 ms
```

---

## A* Search

The solver uses A* search.

For every state, a priority is calculated using:

```text
f(n) = g(n) + h(n)
```

The state with the lowest `f(n)` value is expanded first.

The implementation also keeps track of the lowest known cost for each puzzle state. If a state is reached through a more expensive path than one already found, that path is discarded.

This prevents unnecessary repeated exploration of states.

When the goal is reached, parent pointers stored in each search node are followed backwards to reconstruct the sequence of moves.

---

## Puzzle Solvability

Not every arrangement of the 15-puzzle can be solved.

The implementation checks solvability using:

1. The number of inversions in the puzzle
2. The row containing the blank space, counted from the bottom

For the 4 × 4 puzzle, these values determine whether the configuration belongs to the same reachable state space as the goal configuration.

Unsolvable puzzles are rejected before A* search begins.

---

## Example Puzzles

### Solved Puzzle

```text
1  2  3  4
5  6  7  8
9 10 11 12
13 14 15  0
```

Expected solution length:

```text
0
```

### One-Move Puzzle

```text
1  2  3  4
5  6  7  8
9 10 11 12
13 14  0 15
```

Expected solution:

```text
R
```

### Unsolvable Puzzle

```text
1  2  3  4
5  6  7  8
9 10 11 12
13 15 14  0
```

The `14` and `15` tiles have been swapped, making the puzzle unsolvable.

The solver should report:

```text
This puzzle is not solvable.
```

---

## Move Representation

Moves refer to the direction in which the **blank space moves**:

```text
U = Up
D = Down
L = Left
R = Right
```

For example:

```text
13 14  0 15
```

followed by `R` produces:

```text
13 14 15  0
```

---

## Design Decisions

The puzzle state uses a fixed-size:

```cpp
std::array<int, 16>
```

rather than a dynamically sized container because the 15-puzzle always has exactly 16 positions.

Search nodes use `std::shared_ptr` to maintain parent relationships between nodes. This allows the solution path to be reconstructed after the goal is reached without storing a complete path in every node.

An `std::unordered_map` stores the best known `g` cost for each puzzle state, allowing the implementation to efficiently detect when a newly discovered path is worse than an existing one.

---

## Limitations

The current implementation is designed specifically for the 15-puzzle and therefore assumes:

* A 4 × 4 board
* 15 numbered tiles
* One blank space
* A fixed goal configuration

The command-line interface does not currently provide a graphical representation of the puzzle.

The random puzzle generator also uses a fixed number of scrambling moves rather than generating puzzles according to a specified optimal solution depth.

---

## Possible Extensions

Possible future improvements include:

* Linear conflict heuristic
* Additional heuristic functions
* Configurable scramble depth
* Solution replay
* More detailed search statistics
* Comparison between heuristics
* GUI visualisation
* Support for different puzzle sizes
* Performance comparison between A* and other search algorithms

---

## Author

Developed as part of a University of Technology Sydney Computer Science algorithm implementation project by Finley Ellis.

Language: **C++17**

Algorithm: **A* Search**

Application: **15-Puzzle Solver**
