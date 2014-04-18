#include <iostream>
using namespace std;
#include "nim.h";

void   client_main(int argc, char* argv[] )
{
	char clientName[ MAX_SERVICE_SIZE ];
	cout << "Name?" << endl;
	cin >> clientName;

	SOCKET s = connectsock( BROADCAST_ADDR, UDPPORT_NIM, "udp" );
	std::string host;
	std::string port;
	ServerStruct server[MAX_HOST];
	int numServers = 0;
	UDP_send( s, "Name?", MAX_SEND_BUF - 1, BROADCAST_ADDR, UDPPORT_NIM );

	int status = wait( s, 20, 0 );
	bool serverFound = false;

	while( status > 0 )
	{
		serverFound = true;
		char datagram[MAX_RECV_BUF];

		UDP_recv( s, datagram, MAX_RECV_BUF - 1, server[numServers].host, server[numServers].port );
		if( strncmp( datagram, clientName, 5 ) == 0 )
		{
			char* afterPrefix = strstr( datagram, clientName ) + strlen( clientName );
			strcpy_s( server[numServers].name, afterPrefix );

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
			
			cout << "Choose a server. Enter '0' to quit: " << endl;
			for(int i = 0; i < numServers; i++)
			{
				cout << i + 1 << ". " << server[i].name << endl;
			}
		
			int chosenServerIndex;
			cin >> chosenServerIndex;
			if( chosenServerIndex == 0 )
			{
				readyToQuit = true;
			}
			while( chosenServerIndex > numServers )
			{
				cout << "Incorrect input. Choose again." << endl;
				cin >> chosenServerIndex;
			}
			char challenge [1034] = "Play?Name=";
			strcat( challenge, clientName );
			UDP_send( s, challenge, MAX_SEND_BUF - 1, server[chosenServerIndex - 1].host, UDPPORT_NIM );

			status = wait( s, 20, 0 );

			if(status > 0)
			{
				UDP_recv( s, datagram, MAX_RECV_BUF - 1, server[chosenServerIndex - 1].host, UDPPORT_NIM );
				if( strcmp( datagram, "YES" ) == 0 )
				{
					UDP_send( s, "GREAT!", MAX_SEND_BUF, server[chosenServerIndex - 1].host, UDPPORT_NIM );
					closesocket(s);
					SOCKET s = connectsock( server[chosenServerIndex - 1].host, TCPPORT_NIM, "tcp" );
					//play_Nim( s, ??, ?? );
					readyToQuit = true;
				}
				else
				{
					cout << "Negative response. Try again." << endl;
				}
			}
			else
			{
				cout << "Negative response. Try again." << endl;
			}

		}while( !readyToQuit );

	}
}
