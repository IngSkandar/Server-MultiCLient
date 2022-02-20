#include "ClientSocket.h"




int ClientSocket::CreateClient(int domain, int type, int port)
{
    socketId = socket(domain, type, 0); //This call results in a stream socket with the TCP protocol providing the underlying communication
    if (socketId == INVALID)
    {
        std::cerr << "Can't Create a Socket! Quitting" << std::endl;
        return -1;
    }
    //MemorySet
    memset(Buffer,0,120);
    // Connecting
    socketAdress.sin_family = domain;  
    inet_pton(AF_INET, "127.0.0.1", &socketAdress.sin_addr.s_addr);
    socketAdress.sin_port = htons(50001); 
    //Binding
     if (bind(socketId, (sockaddr *)&socketAdress, sizeof(socketAdress)) < 0)
    {
        std::cerr << "Error : couldn't Bind to server !!" << std::endl; 
        std::cerr << errno << std::endl ;
        return INVALID;
    }
    std::cout << "Binded Socket" << std::endl ;

    socketAdress.sin_port = htons(port); 
    while (connect(socketId, (struct sockaddr *)(&socketAdress), sizeof(sockaddr_in)) == -1)
    {
        std::cout << "Error : couldn't connect to server !!" << std::endl;
    }

    std::cout << "Connection established!!" << std::endl;
}

 int ClientSocket::Send(const char* Buffer)
{   
    if(send(socketId, Buffer, 5 , 0) < 0)
    {
        std::cout << "Error : sending failed !!" << std::endl;
        return INVALID ;
    }
         std::cout << "Sending Message !!" << std::endl;
}
int main()
{
    ClientSocket SocClient;
    SocClient.CreateClient(AF_INET, SOCK_STREAM, 50000);
   
    
}
