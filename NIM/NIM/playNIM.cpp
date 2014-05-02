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
		string rock = boardConfig.substr(0,2);
		int rocks = atoi(rock.c_str());
		vector<char> row;		
		for(rocks; rocks > 0; rocks++)
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

bool check4Win(vector<vector<char>> board )
{
	int winner = noWinner;

	//// Check for vertical winners
	//int i = 1;
	//while (winner == noWinner && i < 4) {
	//	if (board[i] == board[i+3] && board[i] == board[i+6]) {
	//		winner = (board[i] == 'X') ? xWinner : oWinner;
	//	}
	//	i++;
	//}

	//// Check for horizontal winners
	//i = 1;
	//while (winner == noWinner && i < 8) {
	//	if (board[i] == board[i+1] && board[i] == board[i+2]) {
	//		winner = (board[i] == 'X') ? xWinner : oWinner;
	//	}
	//	i  += 3;
	//}

	//// Check for diagonal winners
	//if (winner == noWinner) {
	//	if ( (board[1] == board[5] && board[1] == board[9]) ||
	//		 (board[3] == board[5] && board[3] == board[7]) ) {
	//		winner = (board[5] == 'X') ? xWinner : oWinner;
	//	}
	//}

	//// Check for tie
	//i = 1;
	//int numMoves = 0;
	//while ( i < 10) {
	//	if ( (board[i] == 'X' || board[i] == 'O') ) {
	//		numMoves++;
	//	}
	//	i++;
	//}
	//if (winner == noWinner && numMoves == 9)
	//	winner = TIE;
	//

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
		std::cout << "Waiting on your response: ";
		std::cin  >> move_str;
		if(move_str[0] == 'C' || move_str[0] != 'c')
		{
			for(int i = 1; i < 80; i++)
			{
				move_str[i-1] = move_str[i];
			}
			send(s, move_str, 80, 0);
			//isMessage = true;
		}
		else
		{
			move = atoi(move_str);
			pile = move /100 ;
			numRocks = move % 100;
			
		}
		/*if (board[move] == 'X' || board[move] == 'O') move = 0;*/
	} while (isMessage || pile < 3 || pile > 9);

	return move;
}

int playNIM(SOCKET s, std::string serverName, std::string host, std::string port, int player, std::string boardConfig)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	std::vector<std::vector<char>> board;
	int opponent;
	int move;
	char moveInfo[2];
	bool myMove;

	char* chost = new char[host.length() +1];
	strcpy_s(chost, sizeof(chost), host.c_str());
	char* cserver = new char[serverName.length() + 1];
	strcpy_s(cserver, sizeof(cserver), serverName.c_str());
	char* cport = new char[port.length() +1];
	strcpy_s(cport, sizeof(cport), port.c_str());

	

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
            _itoa_s(move, moveInfo, 1);
            moveInfo[1] = '\0';			

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
                UDP_recv(s, moveInfo, sizeof(moveInfo), chost, cport);
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
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << "No response from opponent.  Aborting the game..." << std::endl;
		} else {
			winner = check4Win(board);
		}
		
		if (winner == player)
			std::cout << "You WIN!" << std::endl;		
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}