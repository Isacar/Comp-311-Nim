// playTicTacToe.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below

#include "nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void initializeBoard( vector<vector<char>> &board, string boardConfig)
{
	string numPiles = boardConfig.substr(0,1);
	int piles = atoi(numPiles.c_str());
	boardConfig = boardConfig.substr(1);
	for(piles; piles > 0; piles--)
	{
		string rock = boardConfig.substr(0,2);
		int rocks = atoi(rock.c_str());
		vector<char> row;		
		for(rocks; rocks > 0; rocks--)
		{
			row.push_back('0');
		}
		board.push_back(row);
	}
	/*char initBoard[10] = {'0','1','2','3','4','5','6','7','8','9'};
	for (int i=0; i<10; i++)
		board[i] = initBoard[i];*/
}

void updateBoard(vector<vector<char>> &board, int move)
{
	int pile = move/100;
	int rocks = move %100;

	for(rocks; rocks > 0; rocks)
	{
		board.at(pile - 1).pop_back();
	}
	/*if (player == CHALLENGER) {
		board[move] = 'X';
	} else if (player == HOST) {
		board[move] = 'O';
	} else
		std::cout << "Problem with updateBoard function!" << std::endl;*/
}

void displayBoard( vector<vector<char>> board )
{
	int piles = board.size();

	cout << endl;
	for(piles; piles > 0 ; piles--)
	{		
		for(int i = board.at(piles - 1).size(); i > 0; i--)
		{
			cout << board.at(piles -1).at(i-1) << "|";
		}
		cout << endl;
	}
	/*std::cout << std::endl;
	std::cout << board[1] << " | " << board[2] << " | " << board[3] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[4] << " | " << board[5] << " | " << board[6] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[7] << " | " << board[8] << " | " << board[9] << std::endl;
	std::cout << std::endl;*/
}

bool check4Win(vector<vector<char>> board, bool myMove, int opponent)
{
	int winner;

	if(opponent == HOST)
	{
		if(myMove)
			winner = hostWinner;
		else
			winner = challengerWinner;
	}
	else
	{
		if(myMove)
			winner = challengerWinner;
		else
			winner = hostWinner;
	}

	for (int i = 0; i < board.size(); i++)
	{
		if(board[i].size() > 0)
		{
			winner = noWinner;
			return winner;
		}
	}

	return winner;
	
}

int getMove(vector<vector<char>> board ,  SOCKET s)
{
	int move;
	int pile;
	int numRocks;
	//bool isMessage = false;
	char move_str[80];

	cout << "Make your move or send a message:" << endl;
	/*std::cout << "Where do you want to place your ";
	char mark = (player == CHALLENGER) ? 'X' : 'O';
	std::cout << mark << "? " << std::endl;*/

	do {
		std::cout << "Waiting on your move or chat message: ";
		std::cin  >> move_str;
		if(move_str[0] == 'C' || move_str[0] == 'c')
		{
			for(int i = 1; i < 80; i++)
			{
				move_str[i-1] = move_str[i];
			}
			send(s, move_str, 80, 0);
			pile = 0;
			//isMessage = true;
		}
		else if(move_str[0] == 'F' || move_str[0]  == 'f')
		{
			std:: cout << "You are forfeiting." << endl;
		}
		else
		{
			move = atoi(move_str);
			pile = move /100 ;
			numRocks = move % 100;
			if( pile < 1 || pile > 9)
			{
				cout << "Pile # must be between 1 and 9." << endl;
				pile = 0;
			}
			else if(board[pile - 1].size() < 1)
			{
				cout << "Pile #" << pile << " is empty!" << endl;
				pile = 0;
			}
			else if(board[pile - 1].size() < numRocks)
			{
				cout << "Pile #" << pile << " only has " << board[pile - 1].size() << " left!" << endl;
				pile = 0;
			}
		}
		/*if (board[move] == 'X' || board[move] == 'O') move = 0;*/
	} while (pile < 1 || pile > 9);

	return move;
}

int playNIM(SOCKET s, const int player, std::string boardConfig)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	std::vector<std::vector<char>> board;
	int opponent;
	int move;
	char moveInfo[MAX_SEND_BUF];
	bool myMove;

	

	

	if (player == CHALLENGER) {
		std::cout << "Playing as Challenger" << std::endl;
		opponent = HOST;
		myMove = true;
	} else {
		std::cout << "Playing as Host" << std::endl;
		opponent = CHALLENGER;
		myMove = false;
	}

	initializeBoard(board, boardConfig);
	displayBoard(board);

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board
			move = getMove(board, s);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(board,move);
			displayBoard(board);

			// Send move to opponent
			/****
			Student should add code here to send the move to the opponent
			(1) Convert the single digit integer, move, to a char[]
			(2) Send the char[] to the opponent, using UDP_send()
			****/			
            _itoa_s(move, moveInfo, 3);
            //moveInfo[] = '\0';			

            send(s, moveInfo, 2, 0);

		} else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			/****
			Student should add code here to retrieve the opponent's move
			(1) Wait for a maximum of 20 seconds for a response from the opponent
			(2) If a response is detected, retrieve the opponent's move, using UDP_recv()
			(3)      Update the board to reflect the move
			(4)      Display the new board
			(5) If no response is detected within 20 seconds, set winner equal to ABORT
			****/

			if(wait(s, 20, 0) != 0)
            {
                recv(s, moveInfo, sizeof(moveInfo), 0);
                move = atoi(moveInfo);

                std::cout << "Board after your opponent's move:" << std::endl;
                updateBoard(board,move);
                displayBoard(board);
            }
            else
            {
                winner = ABORT;
            }

		}
		

		if (winner == ABORT) {
			std::cout << "No response from opponent.  Aborting the game..." << std::endl;
		} 
		else {
			winner = check4Win(board,myMove, opponent);
		}
		
		if (winner == player)
			std::cout << "You WIN!" << std::endl;		
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;

		myMove = !myMove;
	}

	return winner;
}