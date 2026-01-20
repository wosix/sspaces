#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Move.h"

using namespace std;

class Game
{
protected:
    int rows;
    int cols;
    vector<vector<char>> board;
    vector<Move> moveHistory;
    char currentPlayer;
    int evalX = 0;
    int evalO = 0;
    char winner = '\0';

public:
    Game(int rows, int cols, char currentPlayer = 'X');
    Game(const Game &other);
    Game &operator=(const Game &other);
    virtual ~Game() = default;

    void setCurrentPlayer(char player);
    char getCurrentPlayer() const;
    void addMove(Move move);
    void undoMove();
    int getMoveCount() const;
    int getMaxMoves() const;
    void printBoard() const;
    void printEval() const;
    void printMoveHistory() const;
    int getEval(char player) const;
    void setWinner(char player);
    char getWinner() const;
    int getRows() const;
    int getCols() const;
    void reset();

    virtual vector<int> getValidMoves() const = 0;
    virtual bool makeMove(int column) = 0;
    virtual bool assumeMove(int column, char player) = 0;
    virtual void checkIsGameOver() = 0;
    virtual bool checkWin(char player) const = 0;
    virtual void calculateEval() = 0;
    virtual int evaluate(char player) const = 0;
    virtual unique_ptr<Game> clone() const = 0;
};

Game::Game(int rows, int cols, char player)
    : rows(rows),
      cols(cols),
      currentPlayer(player)
{
    if (rows < 4 || cols < 4)
    {
        this->rows = max(4, rows);
        this->cols = max(4, cols);
    }

    board.resize(this->rows, vector<char>(this->cols, ' '));
}

Game::Game(const Game &other)
    : rows(other.rows),
      cols(other.cols),
      board(other.board),
      currentPlayer(other.currentPlayer),
      moveHistory(other.moveHistory),
      evalX(other.evalX),
      evalO(other.evalO),
      winner(other.winner)
{
}

Game &Game::operator=(const Game &other)
{
    if (this != &other)
    {
        rows = other.rows;
        cols = other.cols;
        board = other.board;
        currentPlayer = other.currentPlayer;
        moveHistory = other.moveHistory;
        evalX = other.evalX;
        evalO = other.evalO;
        winner = other.winner;
    }
    printf("Game copy assignment called\n");
    return *this;
}

void Game::setCurrentPlayer(char player)
{
    currentPlayer = player;
}

char Game::getCurrentPlayer() const
{
    return currentPlayer;
}

void Game::addMove(Move move)
{
    board[move.row][move.column] = move.player;
    moveHistory.push_back(move);
    setCurrentPlayer(currentPlayer == 'X' ? 'O' : 'X');
}

void Game::undoMove()
{
    Move move = moveHistory.back();
    moveHistory.pop_back();
    board[move.row][move.column] = ' ';
    setCurrentPlayer(currentPlayer == 'X' ? 'O' : 'X');
}

int Game::getMoveCount() const
{
    return moveHistory.size();
}

int Game::getMaxMoves() const
{
    return getRows() * getCols();
}

int Game::getEval(char player) const
{
    if (player == 'X')
        return evalX;
    if (player == 'O')
        return evalO;
    return 0;
}

void Game::setWinner(char player)
{
    winner = player;
}

char Game::getWinner() const
{
    return winner;
}

int Game::getRows() const
{
    return rows;
}

int Game::getCols() const
{
    return cols;
}

void Game::printBoard() const
{
    printf("\n");

    // Wyświetlanie numerów kolumn
    for (int col = 0; col < cols; ++col)
    {
        printf("  %d ", col + 1);
    }
    printf("\n");

    // Wyświetlanie planszy
    for (int row = 0; row < rows; ++row)
    {
        printf("|");
        for (int col = 0; col < cols; ++col)
        {
            printf(" %c |", board[row][col]);
        }
        printf("\n");

        // Linia oddzielająca
        printf("+");
        for (int col = 0; col < cols; ++col)
        {
            printf("---+");
        }
        printf("\n");
    }

    // printf("Aktualny gracz: %c\n", currentPlayer);
    printf("Liczba ruchów: %d\n", getMoveCount());
    // printMoveHistory();
}

void Game::printEval() const
{
    printf("\nOcena pozycji: X=%+d, O=%+d (różnica: %+d)\n",
           evalX, evalO, evalX - evalO);
}

void Game::printMoveHistory() const
{
    printf("\n===== HISOTRIA RUCHÓW =======");
    for (int i = 1; i <= moveHistory.size(); i++)
    {
        auto move = moveHistory[i - 1];
        printf("\n%d. - k:%d,w:%d - %c", i, move.column, move.row, move.player);
    }
}

void Game::reset()
{
    board = vector<vector<char>>(rows, vector<char>(cols, ' '));
    currentPlayer = 'X';
    moveHistory.clear();
    evalX = 0;
    evalO = 0;
    winner = '\0';
}
