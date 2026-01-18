#pragma once
#include <iostream>
#include "AIPlayer.h"
#include "../game/Game.h"
#include "../stats/MoveStats.h"

using namespace std;

class RandomPlayer : public AIPlayer
{
public:
    RandomPlayer();

    int chooseMove(const Game &game) override;
};

RandomPlayer::RandomPlayer() : AIPlayer("Random_AI") {}

int RandomPlayer::chooseMove(const Game &game)
{
    clearNodesBranches();
    clearPossibleMoves();

    auto startTime = chrono::high_resolution_clock::now();

    for (int move : game.getValidMoves())
    {
        addPosibleMove(move);
    }

    if (possibleMoves.empty())
    {
        lastMoveStats = MoveStats{
            0, 0,
            chrono::duration_cast<chrono::milliseconds>(
                chrono::high_resolution_clock::now() - startTime),
            -1};
        return -1;
    }

    int chosenMove = getRandomMove();

    auto endTime = chrono::high_resolution_clock::now();

    lastMoveStats = MoveStats{
        0, 0,
        chrono::duration_cast<chrono::microseconds>(endTime - startTime),
        chosenMove};

    allMovesStats.push_back(lastMoveStats);

    // printMoveStats();
    return chosenMove;
}
