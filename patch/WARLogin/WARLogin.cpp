#include "WARLogin.h"
#include "../Client.h"
#include "../CoreSocket.h"

WARLogin::WARLogin()
{
                    
}

WARLogin::~WARLogin()
{
                    
}

void WARLogin::sendConInit(CoreSocket *CSocket, int index) {
    ByteBuffer conInitPkt;
    CClient* tcl = CSocket->getClient(index);
    //printf("putting seq: %X\n", (uint32)tcl->getSequence());
    unsigned int nseq = tcl->clSeq;
    
    conInitPkt << (uint32)nseq;
    conInitPkt << (uint16)0x0000;
    conInitPkt << (uint32)0x04000000;
    
    dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_INIT), conInitPkt.contents(), conInitPkt.size(), index);
}

void WARLogin::sendPassSeed(CoreSocket *CSocket, int index) {
    ByteBuffer passSeedPkt;
    CClient* tcl = CSocket->getClient(index);
    unsigned int nseq = tcl->clSeq;

    char hSeed[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    passSeedPkt << (uint32)nseq;
    passSeedPkt << (uint16)0x0000;
    passSeedPkt << (uint32)htonl(sizeof(hSeed)); //ALWAYS 24 (0x18)
    passSeedPkt.append(hSeed, sizeof(hSeed));
    
    dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_PASS_SEED), passSeedPkt.contents(), passSeedPkt.size(), index);
}

void WARLogin::sendUserAuth(CoreSocket *CSocket, int index, bool isChar) {
    ByteBuffer userAuthPkt;
    CClient* tcl = CSocket->getClient(index);
    unsigned int nseq = tcl->clSeq;

    userAuthPkt << (uint32)nseq;
    userAuthPkt << (uint16)0x0000; //Result code, 0 = good. 0x09 = bad
    if(!isChar) {
        userAuthPkt << (uint8)0x00;
    }

    if(!isChar) {
        dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_USERAUTHD), userAuthPkt.contents(), userAuthPkt.size(), index);
    } else{
        dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_CHARAUTHD), userAuthPkt.contents(), userAuthPkt.size(), index);
    }
}

ByteBuffer WARLogin::addSetting(string setName, string setVal) {
    ByteBuffer spkt;
    spkt << (uint32)htonl(setName.size());
    spkt.append(setName.c_str(), setName.size());
    spkt << (uint32)htonl(setVal.size());
    spkt.append(setVal.c_str(), setVal.size());
    return spkt;
}

void WARLogin::sendServerList(CoreSocket *CSocket, int index) {
    ByteBuffer svListPkt;
    CClient* tcl = CSocket->getClient(index);
    unsigned int nseq = tcl->clSeq;

    svListPkt << (uint32)nseq;
    svListPkt << (uint16)0x0000;
    
    svListPkt << (uint32)0x01000000; //Server Count?
    
    
    svListPkt << (uint32)0x00000102; //02 00 00 00 //byte: server num, byte: misc type 1, short: unk
    svListPkt << (uint32)0x0100; //00 01 //misc type 1
    svListPkt << (uint32)0x0202; //02 02 //byte: server #, byte: server #
    
    //<Server>
    string serverName = "RAMSEY";
    svListPkt << (uint32)0x06000000; //name size
    svListPkt.append(serverName.c_str(), serverName.size());
    
    svListPkt << (uint32)0x06000000; //# of settings set.
    
    //settings:
    ByteBuffer app;
    app = addSetting("setting.language", "EN");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("setting.manualbonus.realm.destruction", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("setting.manualbonus.realm.order", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    app = addSetting("setting.name", "RAMSEY");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    app = addSetting("setting.net.address", "63.117.28.56");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    app = addSetting("setting.net.port", "10622");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    app = addSetting("setting.region", "STR_REGION_NORTHAMERICA");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.queue.Destruction.waiting", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.queue.Order.waiting", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.realm.destruction.density", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.realm.order.density", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.servertype.openrvr", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    //app = addSetting("status.servertype.rp", "0");
    //svListPkt.append(app.contents(), app.size());
    //app.clear();
    app = addSetting("status.status", "0");
    svListPkt.append(app.contents(), app.size());
    //app.clear();
    //</Server>
    
    //printf("size of server list packet: %i\n", svListPkt.size());

    dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_SERVERLIST), svListPkt.contents(), svListPkt.size(), index);
}

void WARLogin::sendZoneOk(CoreSocket *CSocket, int index) {
    ByteBuffer zoneOkPkt;
    CClient* tcl = CSocket->getClient(index);
    unsigned int nseq = tcl->clSeq;

    zoneOkPkt << (uint32)nseq;
    zoneOkPkt << (uint16)0x0000;

    dispatchMsg(CSocket, (char*)LOGIN_OPCODES(SV_ZONEOK), zoneOkPkt.contents(), zoneOkPkt.size(), index);
}

void WARLogin::dispatchMsg(CoreSocket *CSocket, char *opcode, char *pData, int pdsize, int index) {
    //Send Size, then opcode
    unsigned char hmsg[1];
    
    if(pdsize > 127) {
        hmsg[0] = (127/(pdsize%127))*100;
        CSocket->SendMsg(hmsg, (unsigned short)0x01, index);
        hmsg[0] = ((pdsize-hmsg[0])/127)+1;
        CSocket->SendMsg(hmsg, (unsigned short)0x01, index);
        //hmsg[0] = 0x01;
        //CSocket->SendByteMsg(hmsg, (unsigned short)0x01, index);
    } else {
        hmsg[0] = (unsigned char)pdsize;
        CSocket->SendMsg(hmsg, (unsigned short)0x01, index);
    }
    hmsg[0] = (unsigned char)opcode;
    CSocket->SendMsg(hmsg, (unsigned short)0x01, index);
    
    //send packet data:
    CSocket->SendMsg(pData, pdsize, index);
}

void WARLogin::handleHeader(char *pData, CClient* tempCl) {
    if((unsigned char)pData[0] != 0) {
        tempCl->nextSize = (unsigned char)pData[0];
        tempCl->nextOpcode = (unsigned char)pData[1];
        printf("set nextOpcode = 0x%X , nextSize = 0x%X \n", tempCl->nextOpcode, tempCl->nextSize);
    }
}

void WARLogin::Handler(CoreSocket *CSocket, char *pData, unsigned short len, int index)
{
    CClient* tempCl = CSocket->getClient(index);
    
    printf("Handling packet, size = %i . Data:\n", len);
    Debug db;
    db.hex_print(pData, len);
    
    handleHeader(pData, tempCl);
    
    if(len == 2) { //size+opcode for following packet
        printf("only packet header was sent\n");
        return;
    } else {
        if(tempCl->nextSize != (unsigned char)len) {
            printf("header was combined with packet, offsetting data\n");
            pData+=2;
        }
        printf("parsing current packet, info: size = 0x%X \n", len);
        
        //Get packet sequence
        unsigned int *seq = (unsigned int*)pData;
        
        //Debug db;
        //db.hex_print((char*)seq, 4);
        
        printf("stored seq: %X, recv'd seq: %X\n", tempCl->clSeq, seq);
        
        /*//If client has resent a packet with a used sequence return
        if(tempCl->getSequence() == (seq)) {
            //CSocket->DisconnectClient(index);
            printf("repeated packet\n");
            return;
        }*/
        
        //Set the sequence
        tempCl->clSeq = *seq;
        
        printf("switching on opcode: 0x%X \n", tempCl->nextOpcode);
        
        switch(tempCl->nextOpcode) {
        case (LOGIN_OPCODES(CL_INIT)):
            sendConInit(CSocket, index);
        break;
        
        case (LOGIN_OPCODES(CL_USERNAME_MSG)):
            sendPassSeed(CSocket, index);
        break;
        
        case (LOGIN_OPCODES(CL_PASS_MSG)):
            sendUserAuth(CSocket, index);
            //sendServerList(CSocket, index); //size testing
        break;
        
        case (LOGIN_OPCODES(CL_CHARPASS_MSG)):
            sendUserAuth(CSocket, index, true);
        break;
        
        case (LOGIN_OPCODES(CL_REQSERVERLIST)):
            sendServerList(CSocket, index);
        break;
        
        case (LOGIN_OPCODES(CL_ISZONEOK)):
            sendZoneOk(CSocket, index);
        break;

        default:
            printf("unknown opcode: %X, packet size: %i \n", tempCl->nextOpcode, len);
        break;
        }
    }
}
     
