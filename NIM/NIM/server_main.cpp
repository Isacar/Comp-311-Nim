// server_main.cpp
//   This function serves as the "main" function for the server-side
#include "nim.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int server_main(int argc, char *argv[], std::string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	std::string host;
	std::string port;
	char response_str[MAX_SEND_BUF];
	
	s = passivesock(UDPPORT_NIM,"udp");

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
	std::cout << "Received: " << buf << std::endl;

	bool finished = false;
	while (!finished) {
	//If the server receives a datagram that contains the string “Name?\0”
		if ( strcmp(buf,NIM_QUERY) == 0) {
			// Respond to name query
			strcpy_s(response_str,NIM_NAME);
			strcat_s(response_str,playerName.c_str());
			UDP_send(s, response_str, strlen(response_str)+1, (char*)host.c_str(), (char*)port.c_str());
			std::cout << "Sending: " << response_str << std::endl;
	//If the server receives a datagram that contains a string similar to “Play? Name=client_name\0”
		} else if ( strncmp(buf,NIM_CHALLENGE,strlen(NIM_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buf,NIM_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName+strlen(NIM_CHALLENGE) << std::endl;
			}
			
			// Play the game.  You are the 'O' player
			//int winner = playnim(s, (char*) playerName.c_str(), (char*)host.c_str(), (char*)port.c_str(), O_PLAYER);
			finished = true;
		}

		if (!finished) {
			char previous_buf[MAX_RECV_BUF];		strcpy_s(previous_buf,buf);
			std::string previous_host;				previous_host = host;
			std::string previous_port;				previous_port = port;

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s,1,0);	// We'll wait a second to see if we receive another datagram
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

	return 0;
}