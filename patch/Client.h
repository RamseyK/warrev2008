#ifndef CLIENT_H
#define CLIENT_H

#include "Common.h"

class CClient
{
public:
	CClient(void);
	~CClient(void);
	void Disconnect(void);
	bool isInUse();
	bool setInUse(bool siu);
	bool ConToSrv;
	SOCKET ClientSocket;
	struct sockaddr_in clnt_addr;
	
public:
    void setResponseId(int tri);
    int getResponseId();
    
    unsigned char nextOpcode;
    unsigned char nextSize;
    unsigned int clSeq;
    
private:
    bool InUse;
    int pktResponseId;
    
};

#endif
