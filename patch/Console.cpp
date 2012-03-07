#include "Common.h"
#include "CoreSocket.h"

using namespace std;

int main()
{
    printf("warhammer online login/patch srv - ramsey\n");
    
    CoreSocket CSocket;
    CSocket.InitSocket(8046);

	while(CSocket.doRun())
	{
		CSocket.Check_Connections();
		CSocket.Check_Message();
	}
	
	CSocket.CloseSocket();

    system("pause");
};
