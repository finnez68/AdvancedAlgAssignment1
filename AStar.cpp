#include "AStar.hpp"

#include "Solvability.hpp"

#include <chrono>
#include <limits>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

SearchNode::SearchNode(
    const PuzzleState& state,
    int g,
    int h,
    shared_ptr<SearchNode> parent,
    optional<Move> move
)
    : state(state),
      g(g),
      h(h),
      f(g + h),
      parent(parent),
      move(move)
{
}

bool NodeComparator::operator()(
    const shared_ptr<SearchNode>& a,
    const shared_ptr<SearchNode>& b
) const
{
    // priority_queue puts the "largest" element first.
    // Therefore reverse the comparison to get the smallest f first.
    if (a->f != b->f) {
        return a->f > b->f;
    }

    return a->h > b->h;
}

AStarSolver::AStarSolver(HeuristicType heuristicType)
    : heuristicType(heuristicType)
{
}

Solution AStarSolver::solve(const PuzzleState& start)
{
    Solution solution;

    auto startTime = chrono::steady_clock::now();

    // Check whether puzzle can actually be solved.
    if (!isSolvable(start)) {
        auto endTime = chrono::steady_clock::now();

        solution.stats.runtimeMilliseconds =
            chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime
            ).count();

        return solution;
    }

    using NodePtr = shared_ptr<SearchNode>;

    priority_queue<
        NodePtr,
        vector<NodePtr>,
        NodeComparator
    > open;

    unordered_map<
        PuzzleState,
        int,
        PuzzleStateHash
    > bestCost;

    int startH = heuristic(start, heuristicType);

    NodePtr startNode =
        make_shared<SearchNode>(
            start,
            0,
            startH
        );

    open.push(startNode);

    bestCost[start] = 0;

    solution.stats.maximumFrontierSize = 1;

    while (!open.empty()) {

        NodePtr current = open.top();
        open.pop();

        /*
            Ignore this node if we have already found
            a cheaper path to the same state.
        */
        if (current->g > bestCost[current->state]) {
            continue;
        }

        // Goal reached
        if (current->state.isGoal()) {

            vector<Move> reversedMoves;

            NodePtr node = current;

            while (node->parent != nullptr) {
                reversedMoves.push_back(*(node->move));
                node = node->parent;
            }

            reverse(
                reversedMoves.begin(),
                reversedMoves.end()
            );

            solution.moves = reversedMoves;

            auto endTime = chrono::steady_clock::now();

            solution.stats.runtimeMilliseconds =
                chrono::duration_cast<chrono::milliseconds>(
                    endTime - startTime
                ).count();

            return solution;
        }

        solution.stats.nodesExpanded++;

        vector<pair<PuzzleState, Move>> neighbours =
            current->state.getNeighbours();

        for (const auto& neighbour : neighbours) {

            const PuzzleState& nextState = neighbour.first;
            Move move = neighbour.second;

            int newG = current->g + 1;

            solution.stats.nodesGenerated++;

            auto it = bestCost.find(nextState);

            /*
                Only consider this state if:

                1. We have never seen it before, or
                2. We found a cheaper path to it.
            */
            if (it == bestCost.end() || newG < it->second) {

                bestCost[nextState] = newG;

                int newH =
                    heuristic(nextState, heuristicType);

                NodePtr child =
                    make_shared<SearchNode>(
                        nextState,
                        newG,
                        newH,
                        current,
                        move
                    );

                open.push(child);
            }
        }

        solution.stats.maximumFrontierSize =
            max(
                solution.stats.maximumFrontierSize,
                static_cast<int>(open.size())
            );
    }

    auto endTime = chrono::steady_clock::now();

    solution.stats.runtimeMilliseconds =
        chrono::duration_cast<chrono::milliseconds>(
            endTime - startTime
        ).count();

    return solution;
}