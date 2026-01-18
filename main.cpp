#include <iostream>
#include "headers/manager/GameManager.h"
#include "headers/game/ConnectFour.h"
#include "headers/ai_players/RandomPlayer.h"
#include "headers/ai_players/GreedyPlayer.h"
#include "headers/ai_players/MinimaxPlayer.h"
#include "headers/ai_players/AlphaBetaPlayer.h"

using namespace std;

int main()
{
    GameManager manager(make_unique<ConnectFour>(6, 7));

    // manager.playSingleGame();

    // manager.setPlayer2AI(make_unique<RandomPlayer>());
    // manager.playSingleGame();

    // manager.setPlayer2AI(make_unique<GreedyPlayer>());
    // manager.playSingleGame();

    // manager.setPlayer2AI(make_unique<MinimaxPlayer>(3));
    // manager.playSingleGame();

    // manager.setPlayer2AI(make_unique<AlphaBetaPlayer>(7));
    // manager.playSingleGame();

    manager.setBothAI(make_unique<GreedyPlayer>(),
                      make_unique<MinimaxPlayer>(8));
    manager.playMultipleGames(1);

    // manager.setBothAI(make_unique<MinimaxPlayer>(3),
    //                   make_unique<AlphaBetaPlayer>(3));
    // manager.playMultipleGames(100);
}