#include "nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote server.

//    IN parameters:
//    SOCKET s                = Allocate UDP socket handle that can be used for communcations
//    char *broadcastAddress  = The broadcast address that should be used for current LAN
//    char *broadcastPort     = The port that should be used for the broadcast

//    OUT parameters:
//    ServerStruct server[]   = An array (0-based) that contains the structs.  The members of each struct
//                              contain the name, IP Address and port number of remote servers.
//    numServers              = Number of elements in the server[] array

//   Assumptions:  (1) The constant TicTacToe_QUERY contains a string that is recognized by servers as a
//                     request for them to reply with their name, IP address, and port number.
//                 (2) The constant TicTacToe_NAME contains a string that is used as a prefix for datagrams
//                     sent by a server that contain the server's name.

void getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct server[], int &numServers)
{
	
    std::string host;
    std::string port;
    
    char buf[MAX_RECV_BUF];

    numServers = 0;

    // Send TicTacToe_QUERY to broadcastAddress using broadcastPort
    /****
        Student should insert the code necessary to send the TicTacToe_QUERY
        string to the broadcastAddress using the port, broadcastPort
    ****/
    
    UDP_send(s, NIM_QUERY, MAX_SEND_BUF, broadcastAddress, broadcastPort);

    // Receive incoming UDP datagrams (with a maximum of 2 second wait before each UDP_recv() function call)
    // As you read datagrams, if they start with the prefix: TicTacToe_NAME, parse out the server's name
    // and add the name, host address and port number to the server[] array.  Don't forget to increment numServers.
    /****
        Student should insert code here that accomplishes the tasks outlined in the previous
        paragraph.  Namely,
        (1) While there are incoming UDP datagrams (wait a maximum of 2 seconds before each UDP_recv)
        (2) If an incoming datagrams begins with the string defined as TicTacToe_NAME, parse
             out the name that follows the prefix and add it to the ServerStruct array named server.
             
        (3)      Since UDP_recv() also returns the remote server's IP address (host) and port number,
                 store those values in the server[] array as well.
                 
        (4)      As you receive the incoming datagrams that contain server names, increment numServers.
        
        (5) If the incoming datagrams does NOT begin with TicTacToc_NAME, ignore it!
    ****/

    while (wait(s,2,0) == 1)
    {
        UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());

        if(strcmp(buf,NIM_NAME) == 1)
        {
            std::string name = buf;
            name = name.substr(strlen(NIM_NAME));
            server[numServers].name = name;
            server[numServers].host = host;
            server[numServers].port = port;

            numServers++;
            std::cout << name << std::endl;
        }
    }
   
}