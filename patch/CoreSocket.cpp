#include "CoreSocket.h"

CoreSocket::CoreSocket() {
    warl = new WARLogin();
    bool canRun = false;
}

CoreSocket::~CoreSocket() 
{
                         
}


void CoreSocket::InitSocket(unsigned short port) {
    WSADATA wsa;
    memset(&wsa, 0x0, sizeof(WSADATA));
    if(WSAStartup(MAKEWORD(2,0),&wsa) != 0x0)
    {
        printf("WSAStartup failed!\n");
    }

	Server_Address.sin_family = AF_INET;
	Server_Address.sin_addr.s_addr = INADDR_ANY;
	Server_Address.sin_port = htons(port);
	 
	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	bind(ListeningSocket, (struct sockaddr *)&Server_Address, sizeof(Server_Address));

	listen(ListeningSocket, 15);
	ClientsConnected = 0;
	canRun = true;
	
	for(int i=0; i < Max_Connections; i++) {
	   Client[i].Disconnect();
    }
}


void CoreSocket::DisconnectClient(int index)
{
	char *ClientIp = inet_ntoa(Client[index].clnt_addr.sin_addr);
	printf("[%i] [%s] Disconnected\n", index, ClientIp);
	Client[index].Disconnect();
	ClientsConnected--;
}

bool CoreSocket::doRun() {
    return canRun;
}

void CoreSocket::CloseSocket()
{
    canRun = false;
	closesocket(ListeningSocket);
	for (int index = 0; index < Max_Connections, index++;)
	{
		DisconnectClient(index);
	}
	WSACleanup();
}

void CoreSocket::Check_Connections()
{
	int s, nfds;
	fd_set conn;
	int Client_Length = sizeof(Client->clnt_addr);

	timeout.tv_sec=0; // 0 for Non Blocking
	timeout.tv_usec=0;
	
	if (ClientsConnected < Max_Connections)
	{
		FD_ZERO(&conn); // Set the data in conn to nothing
		FD_SET(ListeningSocket, &conn); // Tell it to get the data from the Listening Socket
		nfds=ListeningSocket+1; // Up the nfds value by one, shouldnt be the same for each client that connects for compatability reasons
		s = select(nfds, &conn, NULL, NULL, &timeout); // Is there any data coming in?

		if (s > 0) // Someone is trying to Connect
		{
			for (int index = 0; index < Max_Connections; index++)
			{
				if (!Client[index].isInUse())
				{
					Client[index].ClientSocket = accept(ListeningSocket,(struct sockaddr *)&Client[index].clnt_addr, &Client_Length);
					char *ClientIp = inet_ntoa(Client[index].clnt_addr.sin_addr);
					printf("New Client: [%i] [%s]\n", index, ClientIp);
                    //cout << "[" << index << "] Accepted Connection From: " << ClientIp << endl;
					// Now that a client has connected up the number of clients thats connected
					ClientsConnected++;
					// This Client is now in use.
					Client[index].setInUse(true);
					// Exit the For loop that is looking for empty clients.
					break;
				}
			}
		}
	}
}

void CoreSocket::Check_Message()
{
    if(ClientsConnected == 0) {
        return;
    }
    
	fd_set input_set, exc_set;
	int s;

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	
	
	//Set up the input,and exception sets for select().
    FD_ZERO(&input_set);
    FD_ZERO(&exc_set);
    
	
	// Loop makes it so that the Input and Error sets will look at all the Sockets connected
	for(int i = 0; i < Max_Connections; i++)
	{
        if(Client[i].isInUse()) {
		  FD_SET(Client[i].ClientSocket,&input_set);
		  FD_SET(Client[i].ClientSocket,&exc_set);
        }
	}

	s = select(0,&input_set,NULL,&exc_set,&timeout); // The Actual Select Function sees if data coming in and stores it in s
	
	if (s > 0) // Is there data coming in?
	{
		for (int i=0; i < Max_Connections; i++) // Look Through all the clients to see which one is giving error or data
		{
            if(!Client[i].isInUse()) {
                continue;
            }
            
			if (FD_ISSET(Client[i].ClientSocket,&exc_set)) // There was an Error?
			{
				// For this Server we assume it was a disconnection or something so close the socket.
				
				char *ClientIp = inet_ntoa(Client[i].clnt_addr.sin_addr); // Store the IP of the Client that Just Disconnected
				printf("[%i] [%s] Error Disconnected\n", i, ClientIp);
				DisconnectClient(i);
				continue;
			}
			
			if ((FD_ISSET(Client[i].ClientSocket, &input_set)))	
            {
               RecvMsg(i);
            }
		}
	}
}


void CoreSocket::RecvMsg(int index)
{
     char *pData = new char[1452];
     unsigned short len;
     len = recv(Client[index].ClientSocket,pData,1452,0);
     
    if(len == 0) {
        DisconnectClient(index);
        return;
    }
     
     if(Client[index].isInUse()) {
        warl->Handler(this, pData, len, index);
     }
     
     delete [] pData;
}

void CoreSocket::SendMsg(char pData[], unsigned short len, int index)
{
     /*Debug db;
     printf("\nSending packet: size = %i\n", len);
     db.hex_print(pData, len);*/
     send(Client[index].ClientSocket,pData,len,0);
}

void CoreSocket::SendMsg(unsigned char pData[], unsigned short len, int index)
{
     send(Client[index].ClientSocket,(char*)pData,len,0);
}

CClient* CoreSocket::getClient(int index) {
    return &Client[index];
}

