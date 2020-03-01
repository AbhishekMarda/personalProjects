//
//  Board.hpp
//  tictactoe
//
//  Created by Abhishek Marda on 2/29/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#ifndef Board_h
#define Board_h


const int MAX_COLS= 4;
const int MAX_ROWS= 4;

class Board
{
public:
    Board ();
    ~Board ();
    void init();                                                //initialize the board
    void play();                                                //play the game
    int plot(const int row, const int col, char c);             //plot the given point on the board
    void getInput(int& row, int& col);                          //get the point to be plotted from the player
    void display();                                             //display the board on the grid
    bool stillWinnable();                                       //checks if the game can still be won
    bool gameOver();                                            //returns true if game can't be won or has been won
    bool gameWonByPlayer();                                     //returns true if the game has been won by a
                                                                //player and gives back the player number
private:
    bool gameWonByPlayerHelper(char c);
    bool allPlacesFilled();
    char m_board[MAX_ROWS][MAX_COLS];
    bool m_playerChance;                         //true when player 1s chance, false when player
                                                 //2's chance
    char m_plotChar;
    const char M_INIT_CHAR='.';
    
    
};



#endif 
