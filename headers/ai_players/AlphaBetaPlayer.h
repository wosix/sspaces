#pragma once
#include <iostream>
#include <random>
#include <climits>
#include "AIPlayer.h"
#include "../game/Game.h"
#include "../stats/MoveStats.h"

using namespace std;

class AlphaBetaPlayer : public AIPlayer
{
private:
    int searchDepth;

public:
    AlphaBetaPlayer(int depth = 3);

    int chooseMove(const Game &game) override;

    void saveMovesAnalyze() const override;

private:
    int minimax(Game &game, int depth, bool myTurn, char myChar, int alpha, int beta);
};

AlphaBetaPlayer::AlphaBetaPlayer(int depth) : AIPlayer("AlphaBeta_AI"), searchDepth(depth) {}

int AlphaBetaPlayer::chooseMove(const Game &game)
{
    clearNodesBranches();
    clearPossibleMoves();

    auto startTime = chrono::high_resolution_clock::now();

    vector<int> validMoves = game.getValidMoves();

    if (validMoves.empty())
    {
        lastMoveStats = MoveStats{
            0, 0,
            chrono::duration_cast<chrono::milliseconds>(
                chrono::high_resolution_clock::now() - startTime),
            -1};
        return -1;
    }

    auto gameCopy = game.clone();
    char currentPlayer = gameCopy->getCurrentPlayer();
    char opponent = (currentPlayer == 'X') ? 'O' : 'X';

    int bestEvalDif = INT_MIN;
    int bestMove = -1;

    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (int move : validMoves)
    {
        if (!gameCopy->makeMove(move))
        {
            continue;
        }

        int evalDiff = minimax(*gameCopy, searchDepth - 1, false, currentPlayer, alpha, beta);

        if (evalDiff > bestEvalDif)
        {
            bestEvalDif = evalDiff;
            bestMove = move;
        }
        alpha = max(alpha, bestEvalDif);
        gameCopy->undoMove();
    }

    auto endTime = chrono::high_resolution_clock::now();

    lastMoveStats = MoveStats{
        nodesVisited,
        prunedBranches,
        chrono::duration_cast<chrono::microseconds>(endTime - startTime),
        bestMove};

    allMovesStats.push_back(lastMoveStats);

    // printMoveStats();

    return bestMove;
}

int AlphaBetaPlayer::minimax(Game &game, int depth, bool myTurn, char myChar, int alpha, int beta)
{
    // printf("\nLOG MINIMAX FUNCTION():");
    addNodesVisited();
    char opponent = (myChar == 'X') ? 'O' : 'X';

    if (depth == 0)
    {
        return game.getEval(myChar) - game.getEval(opponent);
    }

    vector<int> validMoves = game.getValidMoves();

    if (myTurn)
    {
        int maxEvalScore = INT_MIN;
        for (int move : validMoves)
        {
            game.makeMove(move);
            int evalScore = minimax(game, depth - 1, false, myChar, alpha, beta);
            game.undoMove();

            maxEvalScore = max(maxEvalScore, evalScore);
            alpha = max(alpha, maxEvalScore);

            if (beta <= alpha)
            {
                addPrunedBranches();
                break;
            }
        }
        return maxEvalScore;
    }
    else
    {
        int minEvalScore = INT_MAX;
        for (int move : validMoves)
        {
            game.makeMove(move);
            int evalScore = minimax(game, depth - 1, true, myChar, alpha, beta);
            game.undoMove();

            minEvalScore = min(minEvalScore, evalScore);
            beta = min(beta, minEvalScore);

            if (beta <= alpha)
            {
                addPrunedBranches();
                break;
            }
        }
        return minEvalScore;
    }
}

void AlphaBetaPlayer::saveMovesAnalyze() const
{
    SimulationStats stats = SimulationStats(allGamesStats);

    ofstream file(playerName + "_byMoveStats.csv");
    if (file.is_open())
    {
        // for (int i = 1; i <= 21; i++)
        // {
        //     file << ";Move " << i << " avg:;";
        //     file << "nodes visited;";
        //     file << "pruned branches;";
        //     file << "taken time;";
        // }
        // file << "\n";
        int i = 1;
        for (const MoveAvgStats &avgStats : stats.moveStats)
        {
            file << ";";
            file << avgStats.gamesReached << ";";
            file << avgStats.nodesVisited << ";";
            file << avgStats.prunedBranches << ";";
            file << avgStats.timeTaken.count() / 1000.0 << ";";
        }
        file.close();
    }
}