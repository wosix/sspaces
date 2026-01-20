#pragma once
#include <iostream>
#include <random>
#include <climits>
#include "AIPlayer.h"
#include "../game/Game.h"
#include "../stats/MoveStats.h"

using namespace std;

class MinimaxPlayer : public AIPlayer
{
private:
    int searchDepth;

public:
    MinimaxPlayer(int depth = 3);

    int chooseMove(const Game &game) override;

private:
    int minimax(Game &game, int depth, bool myTurn, char myChar);
};

MinimaxPlayer::MinimaxPlayer(int depth) : AIPlayer("Minimax_AI"), searchDepth(depth) {}

int MinimaxPlayer::chooseMove(const Game &game)
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

    for (int move : validMoves)
    {
        if (!gameCopy->makeMove(move))
        {
            continue;
        }

        int evalDiff = minimax(*gameCopy, searchDepth - 1, false, currentPlayer);

        if (evalDiff > bestEvalDif)
        {
            bestEvalDif = evalDiff;
            bestMove = move;
        }
        gameCopy->undoMove();
    }

    auto endTime = chrono::high_resolution_clock::now();

    lastMoveStats = MoveStats{
        nodesVisited, 0,
        chrono::duration_cast<chrono::microseconds>(endTime - startTime),
        bestMove};

    allMovesStats.push_back(lastMoveStats);

    // printMoveStats();

    return bestMove;
}

int MinimaxPlayer::minimax(Game &game, int depth, bool myTurn, char myChar)
{
    char opponent = (myChar == 'X') ? 'O' : 'X';
    addNodesVisited();

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
            int evalScore = minimax(game, depth - 1, false, myChar);
            game.undoMove();
            if (evalScore > maxEvalScore)
                maxEvalScore = evalScore;
        }
        return maxEvalScore;
    }
    else
    {
        int minEvalScore = INT_MAX;
        for (int move : validMoves)
        {
            game.makeMove(move);
            int evalScore = minimax(game, depth - 1, true, myChar);
            game.undoMove();
            if (evalScore < minEvalScore)
                minEvalScore = evalScore;
        }
        return minEvalScore;
    }
}
