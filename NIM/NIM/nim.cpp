/************************************************************************************
* nim.cpp                                                                           *
*                                                                                   *
* This program plays the math game, Nim, over the network (both as client & server). *
*************************************************************************************/
#include "nim.h"

#include <WinSock2.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")

void main(int argc, char* argv[])
{
	int  choice;
	char choice_str[MAX_NAME];
	char newline;

	WORD wVersionRequired = 0x0202;
	WSADATA wsaData;
	int iResult = WSAStartup(wVersionRequired, &wsaData);
	if (iResult != 0)
	{
		std::cout << "Unable to initialize Windows Socket library." << std::endl;
//		return 0;
	}

	do {
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "Do you want to:" << std::endl;
		std::cout << "    (1) Host a game" << std::endl;
		std::cout << "    (2) Connect to someone else's game?" << std::endl;
		std::cout << "    (3) Quit" << std::endl;
		std::cout << "Enter 1, 2 or 3: ";
		std::cin  >> choice_str; std::cin.get(newline);
		choice = atoi(choice_str);

		if (choice == 1)
			server_main(argc,argv);
		else if (choice == 2)
			client_main(argc,argv);
		else if (choice != 3)
			std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;

	} while (choice != 3);
}