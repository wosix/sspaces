#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include "MoveStats.h"
#include "GameStats.h"

using namespace std;

struct MoveAvgStats
{
    double nodesVisited;
    double prunedBranches;
    chrono::microseconds timeTaken;
    int gamesReached;

    MoveAvgStats() : nodesVisited(0),
                     prunedBranches(0),
                     timeTaken(0),
                     gamesReached(0) {}
};

struct SimulationStats
{
    vector<GameStats> gamesStats;
    vector<MoveAvgStats> moveStats;

    SimulationStats(vector<GameStats> gamesStats) : gamesStats(gamesStats)
    {

        size_t maxMoves = 0;
        for (const GameStats &gStats : gamesStats)
        {
            maxMoves = max(maxMoves, gStats.moves.size());
        }

        moveStats.resize(maxMoves);

        for (const GameStats &gStats : gamesStats)
        {
            for (size_t i = 0; i < gStats.moves.size(); i++)
            {
                const MoveStats &mStats = gStats.moves[i];
                MoveAvgStats &avg = moveStats[i];

                avg.nodesVisited += mStats.nodesVisited;
                avg.prunedBranches += mStats.prunedBranches;
                avg.timeTaken += mStats.timeTaken;
                avg.gamesReached++;
            }
        }

        for (MoveAvgStats &avg : moveStats)
        {
            if (avg.gamesReached > 0)
            {
                avg.nodesVisited /= avg.gamesReached;
                avg.prunedBranches /= avg.gamesReached;
                avg.timeTaken = chrono::microseconds(
                    avg.timeTaken.count() / avg.gamesReached);
            }
        }
    }
};