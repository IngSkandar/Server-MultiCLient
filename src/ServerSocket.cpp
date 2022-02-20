#include "ServerSocket.h"

int ServerSocket::CreateServer(int domain, int type, int port)
{
    //Creating Socket
    ServersocketId = socket(domain, type , 0); //This call results in a stream socket with the TCP protocol providing the underlying communication
    if(ServersocketId < 0)
    {
        std::cerr << "Error : couldn't create socket !!" << std::endl;

    }
    else
    {
        if (Reuse)
        {
            int enable = 1;
            if (setsockopt(ServersocketId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            {
                std::cerr << "Error : socket reusisability failed !!" << std::endl;
                std::cout << errno << std::endl;

            }
        }
    }
    std::cout << "Socket Created ...  \n\n"<< std::endl;
    //Bind an Ip address and port to a socket
    sockaddr_in SOC;                                       //Creating an Object (Struc sockaddr_in)
    SOC.sin_family = domain;                               // Type of addresses that a socket can communicate with
    SOC.sin_port = htons(PORT);                            // Port Number
    inet_pton(AF_INET, "127.0.0.1", &SOC.sin_addr.s_addr); //Your connection will accept all incoming addresses, in our case we are specifying to accept localhost
    if (bind(ServersocketId, (sockaddr *)&SOC, sizeof(SOC)) < 0)
    {
        std::cerr << "Error : couldn't connect to server !!" << std::endl;
        std::cout << errno << std::endl;
    }
    //Listening
    std::cout << "Listnening for Clients ...  \n\n"<< std::endl;

    if (listen(ServersocketId, MaxConnexionAllowed) < 0)
    {
        std::cerr << "Error : listening failed !!" << std::endl;
        std::cout << errno << std::endl;
        return INVALID;
    }
    std::cout << "Listnening for Clients  Done...  \n\n" << std::endl;
    //Connecting
    socketAdress.sin_addr.s_addr = Adress;
    if(connect(ServersocketId, (struct sockaddr *)(&socketAdress), sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Error : couldn't connect to server !!" << std::endl;
        std::cout << errno << std::endl;
    }
    std::cout << "Server Connected !!" << std::endl;
    //Accept
    int addressLength = sizeof(sockaddr);
    if(accept(ServersocketId, (struct sockaddr *)&socketAdress, (socklen_t *)&addressLength); < 0)
    {
        std::cerr << "Error : connection failed !!" << std::endl;
         std::cout << errno << std::endl;

    }
    std::cout << "Client Connected" << std::endl;
    //Getpeername : The name of the peer connected to socket descriptor socket
    int index ;
    int addressLength = sizeof(sockaddr);
    int Done = getpeername(ClientSocket[index], (struct sockaddr *)&socketAdress, (socklen_t *)&addressLength);

    std::cout << "Host disconnected, Socket Id = " << ClientSocket[index] << std::endl;
    ClientSocket[index] = 0;
    CloseClient(index);
    return Done;
}

   

int ServerSocket::ServerMultiClient()
{
    for(int i = 0; i < 10; i++)
        ClientSocket[i] = 0;

    if (ServersocketId < 0)
    {
        std::cerr << "Error : couldn't create socket !!" << std::endl;
        std::cout << errno << std::endl;  
    }

    if(bind(INADDR_ANY) < 0)
    {
        std::cerr << "Error : couldn't bind the socket !!" << std::endl;
        std::cout << errno << std::endl;
    }

    if (listen(3) < 0)
    {
        std::cerr << "Error : couldn't listen to the socket !!" << std::endl;
        std::cout << errno << std::endl;
    }

    std::cout << "Waiting for clients to connect ..." << std::endl;
}

int ServerSocket::RecieveData(int index)
{
    if(read(ClientSocket[index], BytesReceived, 120) < 0)
    {
        std::cout << "Error : reception failed !!" << std::endl;
        std::cout << errno << std::endl;
    }
    std::cout << "Data Getting Recieved !!" << std::endl;
}
char * ServerSocket::GetMessage(int index=0)
{
    memset(BytesReceived, 0, 120);
    RecieveData(index);
    return BytesReceived;
}
void ServerSocket::CloseClient(int index=0)
{
    close(ClientSocket[index]);

}
void ServerSocket::CloseMaster()
{
    close(ServersocketId);;
}

void ServerSocket::Close()
{
    CloseClient();
    CloseMaster();
}

void DiplayData(ServerSocket &SocSer)
{
    uint8_t *(data) = (uint8_t *)SocSer.BytesReceived;

    std::cout << "| > Message received : "
              << "Frame number : " << (uint32_t)((*(data) << 24) | (*(data + 1) << 16) | (*(data + 2) << 8) | (*(data + 3))) << " | "
              << "PID : " << (uint16_t)((*(data + 4) << 8) | (*(data + 5))) << " | "
              << "Timestamp : " << (uint32_t)((*(data + 6) << 24) | (*(data + 7) << 16) | (*(data + 8) << 8) | (*(data + 9))) << " | "
              << "Data : " << *(data + 10) << *(data + 11) << *(data + 12) << *(data + 13) << *(data + 14) << std::endl;
}




int main()
{
ServerSocket SocSer;
SocSer.CreateServer(AF_INET, SOCK_STREAM, 3456);
int frameNumber, PID, timeStamp;
char *data;
int activity, maxSocketId, socketId;
fd_set readfds;

    if (SocSer.Success == -1)
        exit(EXIT_FAILURE);


do
    {

        FD_ZERO(&readfds); //Initializes the file descriptor set fdset to have zero bits for all file descriptors. 

        FD_SET(SocSer.ServersocketId, &readfds);//
        maxSocketId = SocSer.ServersocketId;

        for (int i = 0; i < 10; i++)
        {
            if (SocSer.ClientSocket[i] > 0)
                FD_SET(SocSer.ClientSocket[i], &readfds);

            if (SocSer.ClientSocket[i] > maxSocketId)
                maxSocketId = SocSer.ClientSocket[i];
        }

        activity = select(maxSocketId + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            std::cout << "Error : Socket selection !!" << std::endl;
        }

        if (FD_ISSET(SocSer.ServersocketId, &readfds))
        {
            socketId = SocSer.accept();
            if (socketId < 0)
            {
                std::cout << "Error : Establishing connection !!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::cout << "New Connection Established, Socket ID = " << socketId << std::endl;

            for (int i = 0; i < 10; i++)
            {
                if (SocSer.ClientSocket[i] == 0)
                {
                    SocSer.ClientSocket[i] = socketId;
                    std::string response2 = "Welcome !!";
                    SocSer.send(response2, i);
                    break;
                }
            }
        }

        for (int i = 0; i < 10; i++)
        {

            if (FD_ISSET(SocSer.ClientSocket[i], &readfds))
            {
                if (SocSer.GetMessage(i) == 0)
                {
                    SocSer.GetPeerName(i);
                }

                else
                {
                    DiplayData(SocSer);
                    std::string response2 = "Acknowleged !";
                    SocSer.send(response2, i);
                }
            }
        }
    } while (true);

    SocSer.Close();

   /* int Socket::m_send(const std::string& message, int index=0)
{
    int s_return;
    
    if(m_socketType == SERVER)
    {   
        s_return = send(m_clientSocketId[index], message.c_str(), message.size(), 0);
    }
    else
    {
        s_return = send(m_masterSocketId, message.c_str(), 16, 0);
    }
    

    if(s_return < 0)
    {
        std::cout << "Error : sending failed !!" << std::endl;
    }
    return s_return;*/
}