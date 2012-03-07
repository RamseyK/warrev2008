#ifndef PSUGAME_H
#define PSUGAME_H

#include "../Common.h"
#include "opcodes.h"

class CoreSocket;
class CClient;

class WARLogin
{
public:
    WARLogin();
    ~WARLogin();

    void Handler(CoreSocket *CSocket, char *pData, unsigned short len, int index);
private:
    void handleHeader(char *pData, CClient* tempCl);
    void dispatchMsg(CoreSocket *CSocket, char *opcode, char *pData, int pdsize, int index);
    
    //Packet send methods:
    void sendConInit(CoreSocket *CSocket, int index);
    void sendPassSeed(CoreSocket *CSocket, int index);
    void sendUserAuth(CoreSocket *CSocket, int index, bool isChar = false);
    ByteBuffer addSetting(string setName, string setVal);
    void sendServerList(CoreSocket *CSocket, int index);
    void sendZoneOk(CoreSocket *CSocket, int index);
};

#endif
