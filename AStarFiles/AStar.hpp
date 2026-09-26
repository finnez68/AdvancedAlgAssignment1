#ifndef A_STAR_H
#define A_STAR_H

#include "PuzzleState.hpp"
#include "Heuristic.hpp"

#include <memory>
#include <optional>
#include <vector>

struct SolverStats {
    int nodesExpanded = 0;
    int nodesGenerated = 0;
    int maximumFrontierSize = 0;
    long long runtimeMilliseconds = 0;
};

struct Solution {
    std::vector<Move> moves;
    SolverStats stats;
};

struct SearchNode {
    PuzzleState state;

    int g;
    int h;
    int f;

    std::shared_ptr<SearchNode> parent;
    std::optional<Move> move;

    SearchNode(
        const PuzzleState& state,
        int g,
        int h,
        std::shared_ptr<SearchNode> parent = nullptr,
        std::optional<Move> move = std::nullopt
    );
};

struct NodeComparator {
    bool operator()(
        const std::shared_ptr<SearchNode>& a,
        const std::shared_ptr<SearchNode>& b
    ) const;
};

class AStarSolver {
private:
    HeuristicType heuristicType;

public:
    AStarSolver(HeuristicType heuristicType);

    Solution solve(const PuzzleState& start);
};

#endif