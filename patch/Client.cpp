#include "Client.h"

CClient::CClient()
{
	InUse = false;
	nextOpcode = 0;
	nextSize = 0;
	clSeq = 0x01;
}

CClient::~CClient()
{
}

void CClient::Disconnect(void)
{
	closesocket(ClientSocket);
	InUse = false;
}

bool CClient::isInUse() {
    return InUse;
}

bool CClient::setInUse(bool siu) {
    InUse = siu;
}

