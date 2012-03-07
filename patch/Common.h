#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <wchar.h>

/*#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>*/

#ifdef _WIN32 //Windows
//#include <windows.h>
#include <winsock2.h>
#define close closesocket

#else //BSD/nix
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#define SOCKET int
#endif

#include "debug.h"
#include "ByteBuffer.h"
#include "WARLogin/WARLogin.h"

#endif
