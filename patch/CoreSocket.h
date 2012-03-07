#ifndef CORESOCKET_H
#define CORESOCKET_H

#include "Common.h"
#include "Client.h"

#define Max_Connections 5

class CoreSocket
{
public:
    CoreSocket();
    ~CoreSocket();
    void InitSocket(unsigned short port);
    void DisconnectClient(int index);
    void CloseSocket();
    void Check_Connections();
    void Check_Message();
    void RecvMsg(int index);
    void SendMsg(char pData[], unsigned short, int index);
    void SendMsg(unsigned char pData[], unsigned short, int index);

    CClient* getClient(int index);
    
    bool doRun();
    bool canRun;

private:
    WARLogin *warl;

protected:
    SOCKET ListeningSocket;
    struct sockaddr_in Server_Address;
    CClient Client[Max_Connections];
    timeval timeout;
    int ClientsConnected;
};




#endif
