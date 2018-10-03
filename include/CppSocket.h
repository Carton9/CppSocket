#ifndef CPPSOCKET_H
#define CPPSOCKET_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <CppSocketTerm.h>
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

//inet_addr()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

class CppSocket
{
    public:
        typedef void (* ErrorHandler)(Error e); // define the call back function for handle error
        typedef void (* RecevieHandler)(TransData* data,Error e); // define the call back function for handle recived data
        typedef bool (* VerfyHandler)(CppSocket* self); // define the verify funcation for TCP Connection
        CppSocket(Service _service,int _socketfd);
        CppSocket(Service _service,InterAddr _addr);
        CppSocket(Service _service,char* addr,int port);

        Error sendData(TransData* data);
        void sendData(TransData* data,ErrorHandler handler);

        Error recevieData(int length,TransData* data);
        void recevieData(int length,TransData* data,ErrorHandler handler);

        Error connectTo(InterAddr addr);// this function can only use for TCP Client
        void connectTo(InterAddr addr,ErrorHandler errorHandler,VerfyHandler verfyHandler);// this function can only use for TCP Client

        CppSocket* accept(ErrorHandler errorHandler,VerfyHandler verfyHandler);

        Error stopSocket();
        Error reconnect(); // this function can only use for TCP Client
        Error setTimeOut(int time); // time in millier second

        bool isSocketSet();
        bool isConnected();
        bool isClosed();

        virtual ~CppSocket();

    protected:

    private:
        mutex socketLock;
        int socketfd;
        InterAddr localAddress;
        Service service;
        InterAddr addr;
        void _TCPClientSetUp();
        void _TCPServerSetUp();
        void _UDPSetUp();
        void _SetUp();
};

#endif // CPPSOCKET_H
