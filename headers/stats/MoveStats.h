#pragma once
#include <iostream>
#include <chrono>

using namespace std;

struct MoveStats
{
  int nodesVisited;
  int prunedBranches;
  chrono::microseconds timeTaken;
  int chosenMove;
  int evalScore;

  MoveStats() : nodesVisited(0), prunedBranches(0),
                timeTaken(0), chosenMove(-1), evalScore(0) {}

  MoveStats(int nodes, int pruned,
            chrono::microseconds time,
            int move)
      : nodesVisited(nodes),
        prunedBranches(pruned),
        timeTaken(time),
        chosenMove(move),
        evalScore(0) {}
};