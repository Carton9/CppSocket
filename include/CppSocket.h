#ifndef CPPSOCKET_H
#define CPPSOCKET_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
using namespace std;

enum Service{TCP_Client,TCP_Server,UDP}; // this enum is use to indecate the transmisson protocol is using
enum Error{NOERROR,INCURRECT_SERVICE,TIMEOUT,SOCKET_ERROR,CONNECTING_ERROR,ADDRESS_IN_USE};   // this enum is use to show the socket error in the transmission progress
class CppSocket
{
    public:


        typedef struct sockaddr_in InterAddr;   // define the socket structure to InterAddr
        typedef struct _receviedData{char* data;int length;InterAddr address;} TransData; // define a structure to store reviced data
        typedef void (* ErrorHandler)(Error e); // define the call back function for handle error
        typedef void (* RecevieHandler)(TransData* data,Error e); // define the call back function for handle recived data
        typedef bool (* VerfyHandler)(CppSocket*); // define the verify funcation for TCP Connection


        CppSocket(Service service,int _socketfd);
        CppSocket(Service service,InterAddr addr);
        CppSocket(Service service,char* addr,int port);

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


};

#endif // CPPSOCKET_H
