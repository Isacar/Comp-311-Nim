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
	int piles = atoi((char*)boardConfig[0]);
	boardConfig = boardConfig.substr(1);
	for(piles; piles > 0; piles--)
	{
		string rocks = boardConfig.substr(0,2);
		int rocks = atoi(rocks.c_str());
		vector<char> newRow 
	}
	/*char initBoard[10] = {'0','1','2','3','4','5','6','7','8','9'};
	for (int i=0; i<10; i++)
		board[i] = initBoard[i];*/
}

void updateBoard(  vector<vector<char>> &board, int move, int player)
{
	/*if (player == CHALLENGER) {
		board[move] = 'X';
	} else if (player == HOST) {
		board[move] = 'O';
	} else
		std::cout << "Problem with updateBoard function!" << std::endl;*/
}

void displayBoard( vector<vector<char>> board )
{
	/*std::cout << std::endl;
	std::cout << board[1] << " | " << board[2] << " | " << board[3] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[4] << " | " << board[5] << " | " << board[6] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[7] << " | " << board[8] << " | " << board[9] << std::endl;
	std::cout << std::endl;*/
}

int check4Win(char board[10])
{
	int winner = noWinner;

	// Check for vertical winners
	int i = 1;
	while (winner == noWinner && i < 4) {
		if (board[i] == board[i+3] && board[i] == board[i+6]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i++;
	}

	// Check for horizontal winners
	i = 1;
	while (winner == noWinner && i < 8) {
		if (board[i] == board[i+1] && board[i] == board[i+2]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i  += 3;
	}

	// Check for diagonal winners
	if (winner == noWinner) {
		if ( (board[1] == board[5] && board[1] == board[9]) ||
			 (board[3] == board[5] && board[3] == board[7]) ) {
			winner = (board[5] == 'X') ? xWinner : oWinner;
		}
	}

	// Check for tie
	i = 1;
	int numMoves = 0;
	while ( i < 10) {
		if ( (board[i] == 'X' || board[i] == 'O') ) {
			numMoves++;
		}
		i++;
	}
	if (winner == noWinner && numMoves == 9)
		winner = TIE;
	

	return winner;
}

int getMove(char board[10], int player)
{
	int move;
	char move_str[80];

	std::cout << "Where do you want to place your ";
	char mark = (player == CHALLENGER) ? 'X' : 'O';
	std::cout << mark << "? " << std::endl;

	do {
		std::cout << "Your move? ";
		std::cin  >> move_str;
		move = atoi(move_str);
		if (board[move] == 'X' || board[move] == 'O') move = 0;
	} while (move < 1 || move > 9);

	return move;
}

int playTicTacToe(SOCKET s, std::string serverName, std::string host, std::string port, int player, std::string boardConfig)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	std::vector<std::vector<char>> board;
	int opponent;
	int move;
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
			move = getMove(board,player);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(board,move,player);
			displayBoard(board);

			// Send move to opponent
			/****
			Student should add code here to send the move to the opponent
			(1) Convert the single digit integer, move, to a char[]
			(2) Send the char[] to the opponent, using UDP_send()
			****/

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
		}
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << "No response from opponent.  Aborting the game..." << std::endl;
		} else {
			winner = check4Win(board);
		}
		
		if (winner == player)
			std::cout << "You WIN!" << std::endl;
		else if (winner == TIE)
			std::cout << "It's a tie." << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}