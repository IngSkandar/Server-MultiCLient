#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <string.h>
#include <ctime>
#include <sys/time.h>


#define INVALID -1 
#define PORT 3456 
#define MaxConnexionAllowed 2 
class ServerSocket
{
private :
int Port ;
int Domain;
int Protocol;
bool Reuse ;
sockaddr_in socketAdress;
int ClientsocketId;
public:
char BytesReceived[120];
int Done;
int ServersocketId;
int ClientSocket[5];
int Adress;
int Success;
public:
int CreateServer (int domain,int type,int port);
int ServerMultiClient();
int RecieveData(int index);
char * GetMessage(int index=0);
void CloseClient(int index=0);
void CloseMaster();
void Close();
};