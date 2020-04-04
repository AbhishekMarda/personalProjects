
#include "Board.h"
#include <iostream>

using namespace std;



void Board::play()
{
    cout << "Instructions: Enter the row (1-"<<MAX_ROWS<<") and column (1-"<<MAX_COLS<<") where you want move." <<endl;
    display();
    while (!gameOver())
    {
        if (!m_playerChance)
            cout << "Player 1's chance:"<<endl;
        else
            cout <<"Player 2's chance:"<<endl;
        
        int row=-1, col=-1;
        getInput(row, col);
        int successfulPlot= plot(row-1, col-1, m_plotChar);
        display();
        if (successfulPlot==-1)
        {
            cout << "Invalid move, position out of bounds. Try again."<<endl;
            continue;
        }
        
        if (successfulPlot==0)
        {
            cout << "Invalid move, this position has already been plotted. Try again."<<endl;
            continue;
        }
        
        if (m_plotChar=='X')
            m_plotChar='O';
        else if (m_plotChar=='O')
            m_plotChar='X';
        m_playerChance=!m_playerChance;
    }
    
}

void Board::getInput(int& row, int& col)
{
    cout << "Please enter row and column. \nRow: ";
    cin >> row;
    cout << "Column: ";
    cin >> col;
    
}

void Board::display()
{
    
    cout << endl;
    for (int i=0; i<MAX_ROWS; i++)
    {
        for (int j=0; j<MAX_COLS; j++)
            cout << m_board[i][j];
        cout <<endl;
    }
    cout << endl;
}

int Board::plot(const int row, const int col, char c)
{
    //return 0 if the point is already plotted
    //return 1 if the point is successfully plotted
    //return -1 if the point is out of bounds
    if (row < 0 || row >MAX_ROWS-1 || col < 0 || col > MAX_COLS-1)
        return -1;
    
    if (m_board[row][col]!=M_INIT_CHAR && c==m_plotChar)
        return 0;
    
    m_board[row][col]=c;
    
    return 1;
}

bool Board::gameOver()
{
    bool possibleToWin=stillWinnable();
    
    if (!possibleToWin)
        cout << "Game can't be won by either one of you. Let's end this right here shall we?"<<endl;
    else if (gameWonByPlayer())
    {
        if (m_plotChar=='X')
            cout << "Player 2 won! Congrats."<<endl;
        else if (m_plotChar=='O')
            cout << "Player 1 won! Congrats."<<endl;
    }
    
    return gameWonByPlayer() || !possibleToWin;
}

bool Board::gameWonByPlayer()
{
    return gameWonByPlayerHelper('X') || gameWonByPlayerHelper('O');
}

bool Board::gameWonByPlayerHelper(char c)
{
    //won by horizontal
    for (int i=0; i<MAX_ROWS; i++)
        for (int j=0; j<MAX_COLS; j++)
        {
            if (m_board[i][j]!=c)
                break;
            if (j==MAX_COLS-1)
                return true;
        }
    //won by vertical
    for (int j=0; j<MAX_COLS; j++)
        for (int i=0; i<MAX_ROWS; i++)
        {
            if (m_board[i][j]!=c)
                break;
            if (i==MAX_ROWS-1)
                return true;
        }
    
    //won by diagonal
    
    for (int i=0; i<MAX_ROWS; i++)
    {
        if (m_board[i][i]!=c)
            break;
        if (i==MAX_ROWS-1)
            return true;
    }
    
    for (int i=0; i<MAX_ROWS; i++)
    {
        if (m_board[i][MAX_ROWS-1-i]!=c)
            break;
        if (i==MAX_ROWS-1)
            return true;
    }
    
    
    return false;
}

bool Board::stillWinnable()
{
    if (gameWonByPlayer())
        return true;
    
    if (allPlacesFilled())
        return false;
    bool won= false;
    for (int i=0; i<MAX_ROWS; i++)
        for (int j=0; j<MAX_COLS; j++)
        {
            if (plot(i, j, m_plotChar)==1)
            {
                char tempChar= m_plotChar;
                if (m_plotChar=='X')
                    m_plotChar='O';
                else
                    m_plotChar='X';
                if(stillWinnable())
                    won=true;
                m_plotChar= tempChar;
                plot(i, j, M_INIT_CHAR);
            }
            if (won)
                return won;
        }
    
    return false;
}

Board::~Board()
{
    cout <<"Good game! See you again soon!"<<endl;
}

Board::Board()
{
    init();
}

void Board::init()
{
    for (int i=0; i<MAX_ROWS; i++)
        for (int j=0; j<MAX_COLS; j++)
            m_board[i][j]=M_INIT_CHAR;
    m_playerChance=false;
    m_plotChar='X';
    
}

bool Board::allPlacesFilled()
{
    for (int i=0; i<MAX_ROWS; i++)
        for (int j=0; j<MAX_COLS; j++)
            if(m_board[i][j]==M_INIT_CHAR)
                return false;
    return true;
}

