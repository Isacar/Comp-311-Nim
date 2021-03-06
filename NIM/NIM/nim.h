// nim.h    Header file for Nim project

#include <winsock2.h>
#include <string>
static char BROADCAST_ADDR[] = "10.1.255.255" ; //"10.2.95.255"; //"169.254.255.255"; //; 
static char UDPPORT_NIM[] = "29333";
static char TCPPORT_NIM[] = "29334";

#define NIM_QUERY  "Name?"
#define NIM_NAME	 "Name="
#define NIM_CHALLENGE "Play? Name="
#define NIM_ACCEPT_CHALLENGE "YES"
#define NIM_REJECT_CHALLENGE "NO"
#define NIM_RESPONSE_CHALLENGE "GREAT!"
#define NIM_BOARDCONFIG  "CONFIG="



const int CHALLENGER = 1;
const int HOST = 2;
const int ABORT    = -1;
const int FORFEIT = 3;
const int noWinner = 0;
const int hostWinner  = 2;
const int challengerWinner  = 1;

const int MAX_SEND_BUF     = 2048;
const int MAX_RECV_BUF     = 2048;
const int MAX_HOSTNAME     = 1024;
const int MAX_HOST_SIZE    = 16;
const int MAX_SERVICE_SIZE = 80;
const int MAX_LINE      = 1024;
const int MAX_NAME      = 1024;
const int MAX_HOST      = 100;
const int MAX_SERVERS   = 100;
struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};
void   chomp (char*);
char*  timestamp(void);
void   server_main(int,char*[]);
void   client_main(int,char*[]);
//void   play_Nim(SOCKET, bool, char*);
int		playNIM(SOCKET, int, std::string);
int		UDP_recv (SOCKET, char*, int, char*, char*);
int		UDP_send (SOCKET, char*, int, char*, char*);
int		wait(SOCKET, int, int);
SOCKET	passivesock (char*, char*);
SOCKET	connectsock (char*, char*, char*);
void	getServers(SOCKET, char *, char *, ServerStruct [], int&);


