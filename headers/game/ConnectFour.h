#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Game.h"
#include "Move.h"

using namespace std;

class ConnectFour : public Game
{
public:
    ConnectFour(int rows, int cols);
    ConnectFour(int rows, int cols, char currentPlayer);
    ConnectFour(const ConnectFour &other);
    ConnectFour &operator=(const ConnectFour &other);
    unique_ptr<Game> clone() const override;

    vector<int> getValidMoves() const override;
    bool makeMove(int column) override;
    bool assumeMove(int column, char player);
    void checkIsGameOver() override;
    bool checkWin(char player) const override;
    void calculateEval() override;
    int evaluate(char player) const override;
    bool canWinNextMove(char player) const;
    int countOpenThrees(char player) const;
    int countOpenTwos(char player) const;
};

ConnectFour::ConnectFour(int rows, int cols)
    : Game(rows, cols) {}

ConnectFour::ConnectFour(int rows, int cols, char currentPlayer)
    : Game(rows, cols, currentPlayer) {}

ConnectFour::ConnectFour(const ConnectFour &other)
    : Game(other)
{
}

ConnectFour &ConnectFour::operator=(const ConnectFour &other)
{
    if (this != &other)
    {
        rows = other.rows;
        cols = other.cols;
        currentPlayer = other.currentPlayer;
        moveHistory = other.moveHistory;
        evalX = other.evalX;
        evalO = other.evalO;
        winner = other.winner;
        board = other.board;
    }
    return *this;
}

unique_ptr<Game> ConnectFour::clone() const
{
    return make_unique<ConnectFour>(*this);
}

vector<int> ConnectFour::getValidMoves() const
{
    vector<int> validMoves;
    for (int col = 0; col < getCols(); col++)
    {
        if (board[0][col] == ' ')
        {
            validMoves.push_back(col + 1);
        }
    }
    return validMoves;
}

bool ConnectFour::makeMove(int column)
{
    if (column < 1 || column > getCols())
    {
        printf("Nieprawidłowy numer kolumny! Wybierz od 1 do %d.", getCols());
        return false;
    }

    int colIndex = column - 1;

    if (board[0][colIndex] != ' ')
    {
        printf("Kolumna %d jest już pełna!", column);
        return false;
    }

    for (int row = getRows() - 1; row >= 0; --row)
    {
        if (board[row][colIndex] == ' ')
        {
            addMove(Move{row, colIndex, getCurrentPlayer()});
            calculateEval();
            return true;
        }
    }

    return false;
}

bool ConnectFour::assumeMove(int column, char player)
{
    if (column < 1 || column > getCols())
    {
        printf("Nieprawidłowy numer kolumny! Wybierz od 1 do %d.", getCols());
        return false;
    }

    int colIndex = column - 1;

    if (board[0][colIndex] != ' ')
    {
        return false;
    }

    for (int row = getRows() - 1; row >= 0; --row)
    {
        if (board[row][colIndex] == ' ')
        {
            addMove(Move{row, colIndex, player});
            return true;
        }
    }

    return false;
}

void ConnectFour::checkIsGameOver()
{
    if (checkWin('X'))
        setWinner('X');
    else if (checkWin('O'))
        setWinner('O');
    else if (getMoveCount() == getMaxMoves())
        setWinner('D');
}

bool ConnectFour::checkWin(char player) const
{
    int rows = getRows();
    int cols = getCols();

    // horizontal
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols - 3; ++c)
            if (board[r][c] == player &&
                board[r][c + 1] == player &&
                board[r][c + 2] == player &&
                board[r][c + 3] == player)
                return true;

    // vertical
    for (int r = 0; r < rows - 3; ++r)
        for (int c = 0; c < cols; ++c)
            if (board[r][c] == player &&
                board[r + 1][c] == player &&
                board[r + 2][c] == player &&
                board[r + 3][c] == player)
                return true;

    // diag down-right
    for (int r = 0; r < rows - 3; ++r)
        for (int c = 0; c < cols - 3; ++c)
            if (board[r][c] == player &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == player)
                return true;

    // diag up-right
    for (int r = 3; r < rows; ++r)
        for (int c = 0; c < cols - 3; ++c)
            if (board[r][c] == player &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == player)
                return true;

    return false;
}

void ConnectFour::calculateEval()
{
    evalX = evaluate('X');
    evalO = evaluate('O');
}

int ConnectFour::evaluate(char player) const
{
    char opponent = (player == 'X') ? 'O' : 'X';

    if (checkWin(player))
        return 1000000;
    if (checkWin(opponent))
        return -1000000;

    int score = 0;

    if (canWinNextMove(player))
        score += 100000;
    if (canWinNextMove(opponent))
        score -= 150000;

    score += countOpenThrees(player) * 50000;
    score -= countOpenThrees(opponent) * 75000;

    score += countOpenTwos(player) * 1000;
    score -= countOpenTwos(opponent) * 1500;

    for (int row = 0; row < rows; row++)
    {
        if (board[row][3] == player)
            score += 100;
        if (board[row][3] == opponent)
            score -= 100;
    }

    return score;
}

bool ConnectFour::canWinNextMove(char player) const
{
    auto moves = getValidMoves();

    auto gameCopy = clone();
    for (int move : moves)
    {
        if (gameCopy->assumeMove(move, player))
        {
            if (gameCopy->checkWin(player))
            {
                return true;
            }
            gameCopy->undoMove();
            continue;
        }
    }

    return false;
}

int ConnectFour::countOpenThrees(char player) const
{
    int count = 0;

    // horizontal
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols - 3; ++c)
        {
            // XXX _
            if (board[r][c] == player &&
                board[r][c + 1] == player &&
                board[r][c + 2] == player &&
                board[r][c + 3] == ' ')
            {
                count++;
            }
            // XX _ X
            if (board[r][c] == player &&
                board[r][c + 1] == player &&
                board[r][c + 2] == ' ' &&
                board[r][c + 3] == player)
            {
                count++;
            }
            // X _ XX
            if (board[r][c] == player &&
                board[r][c + 1] == ' ' &&
                board[r][c + 2] == player &&
                board[r][c + 3] == player)
            {
                count++;
            }
            // _ XXX
            if (board[r][c] == ' ' &&
                board[r][c + 1] == player &&
                board[r][c + 2] == player &&
                board[r][c + 3] == player)
            {
                count++;
            }
        }
    }

    // vertical - XXX_
    for (int r = 0; r < rows - 3; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (board[r][c] == player &&
                board[r + 1][c] == player &&
                board[r + 2][c] == player &&
                board[r + 3][c] == ' ')
            {
                count++;
            }
        }
    }

    // diag down-right
    for (int r = 0; r < rows - 3; ++r)
    {
        for (int c = 0; c < cols - 3; ++c)
        {
            // XXX _
            if (board[r][c] == player &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == ' ')
            {
                count++;
            }
            // XX _ X
            if (board[r][c] == player &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == ' ' &&
                board[r + 3][c + 3] == player)
            {
                count++;
            }
            // X _ XX
            if (board[r][c] == player &&
                board[r + 1][c + 1] == ' ' &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == player)
            {
                count++;
            }
            // _ XXX
            if (board[r][c] == ' ' &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == player)
            {
                count++;
            }
        }
    }

    // diag up-right
    for (int r = 3; r < rows; ++r)
    {
        for (int c = 0; c < cols - 3; ++c)
        {
            // XXX _
            if (board[r][c] == player &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == ' ')
            {
                count++;
            }
            // XX _ X
            if (board[r][c] == player &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == ' ' &&
                board[r - 3][c + 3] == player)
            {
                count++;
            }
            // X _ XX
            if (board[r][c] == player &&
                board[r - 1][c + 1] == ' ' &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == player)
            {
                count++;
            }
            // _ XXX
            if (board[r][c] == ' ' &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == player)
            {
                count++;
            }
        }
    }

    return count;
}

int ConnectFour::countOpenTwos(char player) const
{
    int count = 0;
    // horizontal
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols - 3; ++c)
        {
            // XX _ _
            if (board[r][c] == player &&
                board[r][c + 1] == player &&
                board[r][c + 2] == ' ' &&
                board[r][c + 3] == ' ')
            {
                count++;
            }
            // _ XX _
            if (board[r][c] == ' ' &&
                board[r][c + 1] == player &&
                board[r][c + 2] == player &&
                board[r][c + 3] == ' ')
            {
                count++;
            }
            // _ _ XX
            if (board[r][c] == ' ' &&
                board[r][c + 1] == ' ' &&
                board[r][c + 2] == player &&
                board[r][c + 3] == player)
            {
                count++;
            }
            // X _ X _
            if (board[r][c] == player &&
                board[r][c + 1] == ' ' &&
                board[r][c + 2] == player &&
                board[r][c + 3] == ' ')
            {
                count++;
            }
            // X _ _ X
            if (board[r][c] == player &&
                board[r][c + 1] == ' ' &&
                board[r][c + 2] == ' ' &&
                board[r][c + 3] == player)
            {
                count++;
            }
        }

    // vertical - XX__
    for (int r = 0; r < rows - 3; ++r)
        for (int c = 0; c < cols; ++c)
            if (board[r][c] == player &&
                board[r + 1][c] == player &&
                board[r + 2][c] == ' ' &&
                board[r + 3][c] == ' ')
                count++;

    // diag down-right
    for (int r = 0; r < rows - 3; ++r)
        for (int c = 0; c < cols - 3; ++c)
        {
            // XX _ _
            if (board[r][c] == player &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == ' ' &&
                board[r + 3][c + 3] == ' ')
            {
                count++;
            }
            // _ XX _
            if (board[r][c] == ' ' &&
                board[r + 1][c + 1] == player &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == ' ')
            {
                count++;
            }
            // _ _ XX
            if (board[r][c] == ' ' &&
                board[r + 1][c + 1] == ' ' &&
                board[r + 2][c + 2] == player &&
                board[r + 3][c + 3] == player)
            {
                count++;
            }
        }

    // diag up-right
    for (int r = 3; r < rows; ++r)
        for (int c = 0; c < cols - 3; ++c)
        {
            // XX _ _
            if (board[r][c] == player &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == ' ' &&
                board[r - 3][c + 3] == ' ')
            {
                count++;
            }
            // _ XX _
            if (board[r][c] == ' ' &&
                board[r - 1][c + 1] == player &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == ' ')
            {
                count++;
            }
            // _ _ XX
            if (board[r][c] == ' ' &&
                board[r - 1][c + 1] == ' ' &&
                board[r - 2][c + 2] == player &&
                board[r - 3][c + 3] == player)
            {
                count++;
            }
        }

    return count;
}
