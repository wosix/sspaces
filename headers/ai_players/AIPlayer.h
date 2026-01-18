#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include "../game/Game.h"
#include "../stats/MoveStats.h"
#include "../stats/GameStats.h"
#include "../stats/SimulationStats.h"

using namespace std;

class AIPlayer
{
protected:
    mt19937 gen;
    vector<int> possibleMoves;

    string playerName;

    int nodesVisited;
    int prunedBranches;

    MoveStats lastMoveStats;
    vector<MoveStats> allMovesStats;
    vector<GameStats> allGamesStats;

public:
    AIPlayer(const string &name = "AI");

    void addNodesVisited();
    void addPrunedBranches();
    void clearNodesBranches();

    void addPosibleMove(int move);
    void clearPossibleMoves();
    int getRandomMove();

    virtual int chooseMove(const Game &game) = 0;

    void clearMoveStats();
    void resetStats();
    MoveStats getLastMoveStats() const;
    const vector<MoveStats> &getAllMovesStats() const;

    void saveGamesStats() const;
    virtual void saveMovesAnalyze() const;
    void printMoveStats() const;
    void printStatsSummary() const;
    string getName() const;
};

AIPlayer::AIPlayer(const string &name)
    : gen(random_device{}()),
      playerName(name)
{
    clearMoveStats();
}

void AIPlayer::clearMoveStats()
{
    possibleMoves.clear();
    nodesVisited = 0;
    prunedBranches = 0;
    lastMoveStats = MoveStats();
    allMovesStats.clear();
}

void AIPlayer::resetStats()
{
    GameStats gameStats = GameStats(getAllMovesStats());
    allGamesStats.push_back(gameStats);
    clearMoveStats();
}

void AIPlayer::saveGamesStats() const
{
    ofstream file(playerName + "_fullGamesStats.csv");
    if (file.is_open())
    {
        file << ";";
        for (int i = 1; i <= 21; i++)
        {
            file << "Move " << i << ";";
        }
        file << "\n";
        int i = 1;
        for (GameStats gameStats : allGamesStats)
        {
            file << "Game " << i << ";";

            for (MoveStats moveStats : gameStats.moves)
            {
                file << moveStats.chosenMove << ";";
            }
            file << "\nNodes visited;";
            for (MoveStats moveStats : gameStats.moves)
            {
                file << moveStats.nodesVisited << ";";
            }
            file << "\nPruned branches;";
            for (MoveStats moveStats : gameStats.moves)
            {
                file << moveStats.prunedBranches << ";";
            }
            file << "\nTime taken [ms];";
            for (MoveStats moveStats : gameStats.moves)
            {
                file << moveStats.timeTaken.count() / 1000.0 << ";";
            }

            i++;
            file << "\n\n\n";
        }

        file.close();
    }
}

void AIPlayer::saveMovesAnalyze() const
{
    SimulationStats stats = SimulationStats(allGamesStats);

    ofstream file(playerName + "_byMoveStats.csv");
    if (file.is_open())
    {
        // for (int i = 1; i <= 21; i++)
        // {
        //     file << ";Move " << i << " avg:;";
        //     file << "nodes visited;";
        //     file << "taken time;";
        // }
        // file << "\n";
        int i = 1;
        for (const MoveAvgStats &avgStats : stats.moveStats)
        {
            file << ";";
            file << avgStats.gamesReached << ";";
            file << avgStats.nodesVisited << ";";
            file << avgStats.timeTaken.count() / 1000.0 << ";";
        }
        file.close();
    }
}

MoveStats AIPlayer::getLastMoveStats() const
{
    return lastMoveStats;
}

const vector<MoveStats> &AIPlayer::getAllMovesStats() const
{
    return allMovesStats;
}

void AIPlayer::printMoveStats() const
{
    MoveStats stats = getLastMoveStats();

    printf("\n[%s] Wybrano ruch %d, sprawdzone stany: %d, ucięte gałęzie: %d, czas: %.1f ms\n",
           playerName.c_str(), stats.chosenMove, stats.nodesVisited, stats.prunedBranches,
           stats.timeTaken.count() / 1000.0);
}

void AIPlayer::printStatsSummary() const
{
    printf("\n=== STATYSTYKI %s ===\n", getName().c_str());
    printf("Liczba ruchów: %zu\n", allMovesStats.size());
}

string AIPlayer::getName() const
{
    return playerName;
}

int AIPlayer::getRandomMove()
{
    uniform_int_distribution<> dist(0, possibleMoves.size() - 1);
    int randomIndex = dist(gen);
    int chosenMove = possibleMoves[randomIndex];
    return chosenMove;
}

void AIPlayer::addPosibleMove(int move)
{
    possibleMoves.push_back(move);
}

void AIPlayer::clearPossibleMoves()
{
    possibleMoves.clear();
}

void AIPlayer::clearNodesBranches()
{
    nodesVisited = 0;
    prunedBranches = 0;
}

void AIPlayer::addNodesVisited()
{
    nodesVisited++;
}

void AIPlayer::addPrunedBranches()
{
    prunedBranches++;
}
