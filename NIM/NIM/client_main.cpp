// client_main.cpp
//   This function serves as the "main" function for the client-side


#include <iostream>
#include <string>
#include <WinSock2.h>
#include "nim.h"

void   client_main(int argc, char* argv[] )
{
	bool readyToQuit = false;
	char datagram [MAX_RECV_BUF];
	std::string response;
	char clientName[ MAX_SERVICE_SIZE ];
	std::cout << "Name? ";
	std::cin >> clientName;
	
	std::string host;
	std::string port;
	ServerStruct server[MAX_SERVERS];

	SOCKET s = connectsock("","","udp");		// Create a socket  (Don't need to designate a host or port for UDP

	// Find all NIM servers
	std::cout << std::endl << "Looking for NIM servers ... " << std::endl;
	int numServers;
	getServers(s,BROADCAST_ADDR,UDPPORT_NIM, server, numServers);

	if (numServers == 0) {
		std::cout << std::endl << "Sorry.  No NIM servers were found.  Try again later." << std::endl << std::endl;
	} else {
		// Display the list of servers found
		std::cout << std::endl << "Found NIM server";
		if (numServers == 1) {
			std::cout << ":" << "  " << server[0].name << std::endl;
		} else {
			std::cout << "s:" << std::endl;
			for (int i=0; i<numServers; i++) {
				std::cout << "  " << i+1 << " - " << server[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers+1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;
	}

	// Allow user to select someone to challenge
	do{
			int answer = 0;
			std::string answer_str;
			if (numServers == 1) {
				std::cout << "Do you want to challenge  " << server[0].name << "? (y/n) ";
				std::getline(std::cin, answer_str);
				if (answer_str[0] == 'y' || answer_str[0] == 'Y') answer = 1;
			} else if (numServers > 1) {
				std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
				std::getline(std::cin,answer_str);
				answer = atoi(answer_str.c_str());
				if (answer > numServers) answer = 0;
			}

		if (answer >= 1 && answer <= numServers) {
				// Extract the opponent's info from the server[] array
				std::string serverName;
				serverName = server[answer-1].name;
				// Adjust for 0-based array
				host = server[answer-1].host;
				port = server[answer-1].port;

				// Append playerName to the NIM_CHALLENGE string & send a challenge to host:port
				char buf[MAX_SEND_BUF];
				strcpy_s(buf,NIM_CHALLENGE);
				strcat_s(buf,clientName);
				
				//bug is somewhere below
				int len = UDP_send(s, buf, strlen(buf)+1,(char*)host.c_str(), (char*)port.c_str());

				int status = wait( s, 20, 0 );

				if(status > 0)
				{
					UDP_recv( s, datagram, MAX_RECV_BUF - 1, (char*)host.c_str(), (char*)port.c_str());
					if( strcmp( datagram, "YES" ) == 0 )
					{
						UDP_send( s, "GREAT!", MAX_SEND_BUF, (char*)host.c_str(), (char*)port.c_str());
						closesocket(s);
						std::cout <<"Creating tcp socket" << std::endl;
						SOCKET cs = connectsock((char*)host.c_str(), TCPPORT_NIM, "tcp" );
						std::cout <<"Ready to play NIM" << std::endl;
						readyToQuit = true;
					}
					else
					{
						//Negative response
						std::cout << "Negative response. Do you want to challenge somebody else? (y/n) " << std::endl;
						
						std::cin >> response;
						//allow your user to challenge some other host or quit the program. 
						if(response == "y")
							readyToQuit = false;
						else{
							readyToQuit = true;
						}

					}
				}
				else
				{
					std::cout << "No response. Try again." << std::endl;
					std::cout << "Do you want to challenge somebody else? (y/n) " << std::endl;
						
						std::cin >> response;
						//allow your user to challenge some other host or quit the program. 
						if(response == "y")
							readyToQuit = false;
						else{
							readyToQuit = true;
						}
				}
		}
	}while( readyToQuit  == false);
	
	


	
}
