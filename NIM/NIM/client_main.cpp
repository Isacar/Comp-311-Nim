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
	std::string clientName;
	//Prompt for name
	std::cout << "Name? ";
	std::getline(std::cin,clientName );
	
	std::string host;
	std::string port;
	std::string boardConfig;
	ServerStruct server[MAX_SERVERS];
	// Create a socket  (UDP)
	SOCKET s = connectsock("","","udp");		

	std::cout << std::endl << "Looking for NIM servers ... " << std::endl;
	int numServers;
	//broadcast datagram to the network
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
	while( readyToQuit  == false && numServers > 0){

			int answer = 0;
			std::string answer_str;
			if (numServers == 1) {
				std::cout << "Do you want to challenge  " << server[0].name << "? (y/n) ";
				std::getline(std::cin, answer_str);
				if (answer_str[0] == 'y' || answer_str[0] == 'Y') answer = 1;
				else readyToQuit = true;
			} else if (numServers > 1) {
				std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
				std::getline(std::cin,answer_str);
				answer = atoi(answer_str.c_str());
				if (answer > numServers){ 
					answer = 0;
					readyToQuit = true;
				}
			}

	

		if (answer >= 1 && answer <= numServers) {
				// Extract the opponent's info from the server[] array
				std::string serverName;
				serverName = server[answer-1].name;
				// Adjust for 0-based array
				host = server[answer-1].host;
				port = server[answer-1].port;

				// Append playerName  & send a challenge datagram to the chosen server
				char buf[MAX_SEND_BUF];
				strcpy_s(buf,NIM_CHALLENGE);
				strcat_s(buf,(char*)clientName.c_str());
				int len = UDP_send(s, buf, strlen(buf)+1,(char*)host.c_str(), (char*)port.c_str());

				int status = wait( s, 20, 0 );
				bool serverAcceptedChallenge = false;
				
				if(status == 1){
					UDP_recv( s, datagram, MAX_RECV_BUF - 1, (char*)host.c_str(), (char*)port.c_str());
					if( strcmp( datagram, NIM_ACCEPT_CHALLENGE ) == 0 ){
						serverAcceptedChallenge = true;
						UDP_send( s, NIM_RESPONSE_CHALLENGE, MAX_SEND_BUF, (char*)host.c_str(), (char*)port.c_str());
						//close the UDP socket

						//Wait for board config.
						status = wait( s, 5, 0 );
						UDP_recv( s, datagram, MAX_RECV_BUF - 1, (char*)host.c_str(), (char*)port.c_str());
						if ( strncmp(datagram,NIM_BOARDCONFIG,strlen(NIM_BOARDCONFIG)) == 0){
							char *startOfName = strstr(datagram,NIM_BOARDCONFIG);
							boardConfig = startOfName+strlen(NIM_BOARDCONFIG);
							
							closesocket(s);


							std::cout <<"Ready to play NIM" << std::endl;
							
							std::cout <<"Creating tcp socket" << std::endl;

							//allocate and connect a TCP client-socket 
							SOCKET cs = connectsock((char*)host.c_str(), TCPPORT_NIM, "tcp" );
					
							std::cout <<"Ready to play NIM" << std::endl;
							playNIM(cs, CHALLENGER, boardConfig);
							//playNIM(cs, (char*) playerName.c_str(), (char*)host.c_str(),TCPPORT_NIM , HOST, boardConfig);

						}else{
							//Didn't recieve board configuration
						}
					
						
						readyToQuit = true;
					}
					
				}

				//Manage negative response
				if(serverAcceptedChallenge == false){
					//challenge some other host or quit the program
					std::cout << serverName <<" declined challenge " << std::endl;
					std::cout << "Do you want to challenge somebody else? (y/n) ";
						
					//allow your user to challenge some other host or quit the program. 
						std::getline(std::cin,answer_str);
						if(answer_str[0] == 'y' || answer_str[0] == 'Y')
							readyToQuit = false;
						else
							readyToQuit = true;
				}
				
		}

	}
	
}
