// client_main.cpp
//   This function serves as the "main" function for the client-side
#include "nim.h";
#include <iostream>
#include <string>
#include <WinSock2.h>

void   client_main(int argc, char* argv[] )
{
	char clientName[ MAX_SERVICE_SIZE ];
	std::cout << "Name?" << std::endl;
	std::cin >> clientName;

	SOCKET s = connectsock( BROADCAST_ADDR, UDPPORT_NIM, "udp" );
	std::string host;
	std::string port;
	ServerStruct server[MAX_HOST];
	int numServers = 0;
	UDP_send( s, NIM_QUERY, MAX_SEND_BUF - 1, BROADCAST_ADDR, UDPPORT_NIM );

	int status = wait( s, 20, 0 );
	bool serverFound = false;

	while( status > 0 )
	{
		serverFound = true;
		char datagram[MAX_RECV_BUF];

		UDP_recv( s, datagram, MAX_RECV_BUF - 1, (char*) server[numServers].host.c_str(), (char*)server[numServers].port.c_str() );
		if( strncmp( datagram, clientName, 5 ) == 0 )
		{
			char* afterPrefix = strstr( datagram, clientName ) + strlen( clientName );
			strcpy_s(server[numServers].name.c_str(), afterPrefix );
		
			numServers++;
		}
		else
		{ 
			//ignore 
		}
		status = wait( s, 20, 0 );
	}

	if(serverFound == true)
	{
		bool readyToQuit = false;
		char datagram [MAX_RECV_BUF];
		do{
			
			std::cout << "Choose a server. Enter '0' to quit: " << std::endl;
			for(int i = 0; i < numServers; i++)
			{
				std::cout << i + 1 << ". " << server[i].name << std::endl;
			}
		
			int chosenServerIndex;
			std::cin >> chosenServerIndex;
			if( chosenServerIndex == 0 )
			{
				readyToQuit = true;
			}
			while( chosenServerIndex > numServers )
			{
				std::cout << "Incorrect input. Choose again." << std::endl;
				std::cin >> chosenServerIndex;
			}
			char challenge [1034] = "Play?Name=";
			strcat( challenge, clientName );
			UDP_send( s, challenge, MAX_SEND_BUF - 1, (char*)server[chosenServerIndex - 1].host.c_str(), UDPPORT_NIM );

			status = wait( s, 20, 0 );

			if(status > 0)
			{
				UDP_recv( s, datagram, MAX_RECV_BUF - 1, (char*)server[chosenServerIndex - 1].host.c_str(), UDPPORT_NIM );
				if( strcmp( datagram, "YES" ) == 0 )
				{
					UDP_send( s, "GREAT!", MAX_SEND_BUF, (char*)server[chosenServerIndex - 1].host.c_str(), UDPPORT_NIM );
					closesocket(s);
					SOCKET s = connectsock( (char*)server[chosenServerIndex - 1].host.c_str(), TCPPORT_NIM, "tcp" );
					//play_Nim( s, ??, ?? );
					readyToQuit = true;
				}
				else
				{
					std::cout << "Negative response. Try again." << std::endl;
				}
			}
			else
			{
				std::cout << "Negative response. Try again." << std::endl;
			}

		}while( !readyToQuit );

	}
}
