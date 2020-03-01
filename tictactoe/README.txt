Hey there, thank you for coming to my GitHub! This is a personal project I made during my freshman year at UCLA. 

I copied all of my XCode files to the github, however if you're on Windows or Linux, you could just copy the following files and put them on your desired IDE:
1. main.cpp
2. Board.h
3. Board.cpp

Upon running, you will be able to read the instructions on how to play the game, which is a normal TicTacToe game. 

If you wish to add challenge to the game, i.e. change the dimensions of the board, in Board.h make the following change

const int MAX_ROWS = <your desired rows>;
const int MAX_COLS = <your desired columns>;

Currently it is set to 3 rows and 3 columns as the class TicTacToe game.

If there is a point where there is no possible way either player can win, the game will end on itself. How cool is that!

Hope you enjoy!
