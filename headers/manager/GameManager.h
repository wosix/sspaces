#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include "../game/Game.h"
#include "../ai_players/AIPlayer.h"

using namespace std;

class GameManager
{
private:
    unique_ptr<Game> game;
    int gamesPlayed = 0;
    int player1Wins = 0;
    int player2Wins = 0;
    int draws = 0;

    unique_ptr<AIPlayer> player1AI;
    unique_ptr<AIPlayer> player2AI;
    bool player1IsAI;
    bool player2IsAI;

public:
    GameManager(unique_ptr<Game> game);

    void playSingleGame();
    void playMultipleGames(int numGames);

    int getPlayerMove(char currentPlayer);

    void setPlayer1AI(unique_ptr<AIPlayer> ai);
    void setPlayer2AI(unique_ptr<AIPlayer> ai);
    void setBothAI(unique_ptr<AIPlayer> ai1, unique_ptr<AIPlayer> ai2);
    void setHumanVsHuman();

    void processFinishedGame();
    void printStats() const;
    void saveStats() const;
    void resetGame();
};

GameManager::GameManager(unique_ptr<Game> game) : game(move(game))
{
    setHumanVsHuman();
}

void GameManager::playSingleGame()
{
    bool playing = true;

    while (playing)
    {
        printf("\n=== ROZPOCZĘCIE GRY %d ===\n", gamesPlayed + 1);

        printf("Gracz X: %s\n", player1IsAI ? "AI" : "Człowiek");
        printf("Gracz O: %s\n", player2IsAI ? "AI" : "Człowiek");

        while (true)
        {
            game->printBoard();
            game->printEval();

            int move = getPlayerMove(game->getCurrentPlayer());

            if (move == -1)
            {
                return;
            }
            if (!game->makeMove(move))
            {
                printf("Niepoprawny ruch! Spróbuj ponownie.\n");
            }

            game->checkIsGameOver();
            if (game->getWinner())
            {
                processFinishedGame();

                printf("\nCzy chcesz zagrać ponownie? (1 = TAK): ");
                int choice;
                cin >> choice;
                playing = (choice == 1) ? true : false;
                break;
            }
        }
    }
    saveStats();
}

void GameManager::playMultipleGames(int numGames)
{
    printf("\n=== ROZPOCZYNAM %d GIER ===", numGames);
    for (int i = 1; i <= numGames; i++)
    {
        // resetGame();
        printf("\n=== ROZPOCZYNAM %d GRE ===", i);

        while (true)
        {
            char currentPlayer = game->getCurrentPlayer();
            int move = getPlayerMove(currentPlayer);

            if (!game->makeMove(move))
            {
                printf("Niepoprawny ruch! Spróbuj ponownie.\n");
            }

            game->checkIsGameOver();
            if (game->getWinner())
            {
                processFinishedGame();
                break;
            }
        }
    }
    printStats();
    saveStats();
    // player1AI->printStatsSummary();
    // player2AI->printStatsSummary();
}

int GameManager::getPlayerMove(char currentPlayer)
{
    if (currentPlayer == 'X' && player1IsAI && player1AI)
    {
        // printf("AI X myśli...\n");
        return player1AI->chooseMove(*game);
    }
    else if (currentPlayer == 'O' && player2IsAI && player2AI)
    {
        // printf("AI O myśli...\n");
        return player2AI->chooseMove(*game);
    }
    else
    {
        int column;
        printf("Gracz %c, wybierz kolumnę (1-%d, 0 aby wyjść): ",
               currentPlayer, game->getCols());
        cin >> column;

        if (column == 0)
            return -1;

        return column;
    }
}

void GameManager::processFinishedGame()
{
    printf("\n KONIEC GRY\n");
    game->printBoard();
    char winner = game->getWinner();
    if (winner == 'D')
    {
        draws++;
        printf("\n ===== REMIS =====\n");
    }
    else if (winner == 'X')
    {
        player1Wins++;
        printf("\n ===== WYGRYWA GRACZ X =====\n");
    }
    else if (winner == 'O')
    {
        player2Wins++;
        printf("\n ===== WYGRYWA GRACZ O =====\n");
    }
    gamesPlayed++;
    // printStats();
    resetGame();
}

void GameManager::setPlayer1AI(unique_ptr<AIPlayer> ai)
{
    player1AI = move(ai);
    player1IsAI = true;
}

void GameManager::setPlayer2AI(unique_ptr<AIPlayer> ai)
{
    player2AI = move(ai);
    player2IsAI = true;
}

void GameManager::setBothAI(unique_ptr<AIPlayer> ai1, unique_ptr<AIPlayer> ai2)
{
    setPlayer1AI(move(ai1));
    setPlayer2AI(move(ai2));
}

void GameManager::setHumanVsHuman()
{
    player1IsAI = false;
    player2IsAI = false;
    player1AI.reset();
    player2AI.reset();
}

void GameManager::printStats() const
{
    double p1Percent = (gamesPlayed > 0) ? (100.0 * player1Wins / gamesPlayed) : 0.0;
    double p2Percent = (gamesPlayed > 0) ? (100.0 * player2Wins / gamesPlayed) : 0.0;
    double drawPercent = (gamesPlayed > 0) ? (100.0 * draws / gamesPlayed) : 0.0;

    printf("\n");
    printf("========================================\n");
    printf("            STATYSTYKI GIER\n");
    printf("========================================\n");
    printf("Rozegrane gry: %d\n", gamesPlayed);
    printf("----------------------------------------\n");
    printf("Gracz X: %3d wygranych (%5.1f%%)\n", player1Wins, p1Percent);
    printf("Gracz O: %3d wygranych (%5.1f%%)\n", player2Wins, p2Percent);
    printf("Remisy:  %3d remisów   (%5.1f%%)\n", draws, drawPercent);
    printf("========================================\n");
    printf("\n\n");
}

void GameManager::saveStats() const
{
    if (player1IsAI)
    {
        player1AI->saveGamesStats();
        player1AI->saveMovesAnalyze();
    }
    if (player2IsAI)
    {
        player2AI->saveGamesStats();
        player2AI->saveMovesAnalyze();
    }
}

void GameManager::resetGame()
{
    game->reset();

    if (player1IsAI)
    {
        player1AI->resetStats();
    }
    if (player2IsAI)
    {
        player2AI->resetStats();
    }
}