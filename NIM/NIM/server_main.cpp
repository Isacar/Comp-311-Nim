// server_main.cpp
//   This function serves as the "main" function for the server-side
#include "nim.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

void server_main(int argc, char *argv[])
{
	SOCKET s, ps;
	char buf[MAX_RECV_BUF];
	std::string host;
	std::string port;
	std::string boardConfig;
	char response_str[MAX_SEND_BUF];
	
	s = passivesock(UDPPORT_NIM,"udp");
	//Prompt for the name of the player
	std::string playerName;
	std::cout << "What is your name? ";
	std::getline(std::cin, playerName);

	//Listen for incoming traffic
	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
	std::cout << "Received: " << buf << std::endl;
	int status;
	bool finished = false;
	std::string acceptChallenge;
	while (!finished) {
	//If the server receives a datagram that contains the string "Name?\0"
		if ( strcmp(buf,NIM_QUERY) == 0) {
			// Respond to name query
			strcpy_s(response_str,NIM_NAME);
			strcat_s(response_str,playerName.c_str());
			UDP_send(s, response_str, strlen(response_str)+1, (char*)host.c_str(), (char*)port.c_str());
			std::cout << "Sending: " << response_str << std::endl;

	//If the server receives a datagram that contains a string similar to "Play? Name=client_name\0"
		} else if ( strncmp(buf,NIM_CHALLENGE,strlen(NIM_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buf,NIM_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName+strlen(NIM_CHALLENGE) << std::endl;
				std::cout << std::endl << "Do you want to accept the challenge? (y/n) ";
				std::cin>>acceptChallenge;

				if(acceptChallenge == "n"){
					strcpy_s(response_str,NIM_REJECT_CHALLENGE);
					UDP_send(s, response_str, strlen(response_str)+1, (char*)host.c_str(), (char*)port.c_str());
					std::cout << "Sending: " << response_str << std::endl;
					//**Continue to Listen for incoming traffic**
				}else if(acceptChallenge == "y"){
					// Play the game
					//allocate and bind a TCP server-socket (ie. a listening socket) 
					ps = passivesock(UDPPORT_NIM,"tcp");
					strcpy_s(response_str,NIM_ACCEPT_CHALLENGE);
					UDP_send(s, response_str, strlen(response_str)+1, (char*)host.c_str(), (char*)port.c_str());
					std::cout << "Sending: " << response_str << std::endl;

					status = wait(s,5,0);
					if(status == 1){
						len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
						std::cout << "Received: " << buf << std::endl;
						if(strcmp(buf, NIM_RESPONSE_CHALLENGE) == 0){
							//Close the UDP socket (#29333) 
							closesocket(s);
							//wait for a TCP connection request from the client on port #29334.
							ps = passivesock(TCPPORT_NIM , "tcp");

							std::cout <<"Ready to play NIM" << std::endl;
							//std::cout <<"Close tcp socket" << std::endl;
							//closesocket(ps);
							//Once the TCP connection is established,
							//the server code is ready to play the game.
							//playNIM(ps, (char*) playerName.c_str(), (char*)host.c_str(),TCPPORT_NIM , HOST, boardConfig);
							finished = true;
						}
					}
				}
			}
		}

		if (!finished) {
			char previous_buf[MAX_RECV_BUF];		strcpy_s(previous_buf,buf);
			std::string previous_host;				previous_host = host;
			std::string previous_port;				previous_port = port;

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			status = wait(s,1,0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << "Received: " << buf << std::endl;
				if (strcmp(buf,previous_buf)==0 &&		// If this datagram is identical to previous one, ignore it.
					host == previous_host && 
					port == previous_port) {
						status = wait(s,1,0);			// Wait another second (still more copies?)
				} else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}

			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram ) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << "Received: " << buf << std::endl;
			}
		}
	}
	closesocket(s);
}