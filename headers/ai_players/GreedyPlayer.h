#pragma once
#include <iostream>
#include <climits>
#include "AIPlayer.h"

using namespace std;

class GreedyPlayer : public AIPlayer
{
public:
    GreedyPlayer();

    int chooseMove(const Game &game) override;
};

GreedyPlayer::GreedyPlayer() : AIPlayer("Greedy_AI") {}

int GreedyPlayer::chooseMove(const Game &game)
{
    clearNodesBranches();
    clearPossibleMoves();

    auto startTime = chrono::high_resolution_clock::now();

    char currentPlayer = game.getCurrentPlayer();
    char opponent = (currentPlayer == 'X') ? 'O' : 'X';

    auto validMoves = game.getValidMoves();

    if (validMoves.empty())
    {
        lastMoveStats = MoveStats{
            0, 0,
            chrono::duration_cast<chrono::microseconds>(
                chrono::high_resolution_clock::now() - startTime),
            -1};
        allMovesStats.push_back(lastMoveStats);
        return -1;
    }

    int bestEvalDif = INT_MIN;

    auto gameCopy = game.clone();

    for (int move : validMoves)
    {
        if (!gameCopy->makeMove(move))
        {
            continue;
        }

        int eval = gameCopy->getEval(currentPlayer);
        int evalOpp = gameCopy->getEval(opponent);
        int evalDiff = eval - evalOpp;

        if (evalDiff > bestEvalDif)
        {
            bestEvalDif = evalDiff;
            clearPossibleMoves();
            addPosibleMove(move);
        }
        else if (evalDiff == bestEvalDif)
        {
            addPosibleMove(move);
        }
        addNodesVisited();
        gameCopy->undoMove();
    }

    int chosenMove = getRandomMove();

    auto endTime = chrono::high_resolution_clock::now();

    lastMoveStats = MoveStats{
        nodesVisited,
        0,
        chrono::duration_cast<chrono::microseconds>(endTime - startTime),
        chosenMove};

    allMovesStats.push_back(lastMoveStats);

    // printMoveStats();
    return chosenMove;
}
