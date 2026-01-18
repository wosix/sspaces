#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include "MoveStats.h"

using namespace std;

struct GameStats
{
    vector<MoveStats> moves;

    GameStats(vector<MoveStats> gameMoves) : moves(gameMoves) {}
};