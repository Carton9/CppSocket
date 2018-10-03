#include "CppSocket.h"
#include <time.h>
#define DefultTimeOut 10000
CppSocket::CppSocket(Service _service,int _socketfd,InterAddr _addr){
    timeout=DefultTimeOut;
    addr=_addr;
    socketfd=_socketfd;
    service=service;
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}
CppSocket::CppSocket(Service _service,InterAddr addr){
    timeout=DefultTimeOut;
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, 0);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    service=service;
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}
CppSocket::CppSocket(Service _service,char* addr,int port){
    timeout=DefultTimeOut;
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, 0);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    service=service;
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}

Error CppSocket::sendData(TransData* data){
    return NOERROR;
}
void CppSocket::sendData(TransData* data,ErrorHandler handler){
    Error e=sendData(data);
    handler(e);
}

Error CppSocket::recevieData(int length,TransData* data){
    return NOERROR;
}
void CppSocket::recevieData(int length,TransData* data,ErrorHandler handler){
    Error e=recevieData(length,data);
    handler(e);
}

Error CppSocket::connectTo(InterAddr remoteAddr){// this function can only use for TCP Client
    if(service==TCP_Server&&service==UDP)
        return INCURRECT_SERVICE;
    int res = connect(socketfd, (struct sockaddr *)&remoteAddr, sizeof(struct sockaddr_in));
    if (0 == res){
        printf("socket connect succeed immediately.\n");
     }else{
        if (errno == EINPROGRESS){
            fd_set rfds, wfds;
            struct timeval tv;
            FD_ZERO(&rfds);
            FD_ZERO(&wfds);
            FD_SET(socketfd, &rfds);
            FD_SET(socketfd, &wfds);tv.tv_sec = timeout/1000;tv.tv_usec = 0;
            int selres = select(socketfd + 1, &rfds, &wfds, NULL, &tv);
            switch (selres){
            case -1:
                return SOCKET_ERROR;
            case 0:
                return TIMEOUT;
            default:
                if (FD_ISSET(socketfd, &rfds) || FD_ISSET(socketfd, &wfds)){
                    connect(socketfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
                    int err = errno;
                    if(err == EISCONN){
                        return CONNECTING_ERROR;
                    }
                    else{
                        printf("connect failed. errno = %d\n", errno);
                        printf("FD_ISSET(sock_fd, &rfds): %d\n FD_ISSET(sock_fd, &wfds): %d\n", FD_ISSET(socketfd, &rfds) , FD_ISSET(socketfd, &wfds));
                        return CONNECTING_ERROR;
                    }
                }
            }
        }else{
            return CONNECTING_ERROR;
        }
     }
    return NOERROR;
}
void CppSocket::connectTo(InterAddr remoteAddr,ErrorHandler errorHandler,VerfyHandler verfyHandler){// this function can only use for TCP Client
     if(service==TCP_Server&&service==UDP){
        errorHandler(INCURRECT_SERVICE);
        return;
     }
     Error e=connectTo(remoteAddr);
     verfyHandler(this);
     errorHandler(e);
}

CppSocket* CppSocket::accept(ErrorHandler errorHandler,VerfyHandler verfyHandler){
}

Error CppSocket::stopSocket(){
}
Error CppSocket::reconnect(){// this function can only use for TCP Client
}
Error CppSocket::setTimeOut(int time){ // time in millier second
    timeout=time;
    return NOERROR;
}

bool CppSocket::isSocketSet(){
}
bool CppSocket::isConnected(){
}
bool CppSocket::isClosed(){
}
Error CppSocket::TCPClientSetUp(){
    return NOERROR;
}
Error CppSocket::TCPServerSetUp(){
    if(bind(socketfd,(struct sockaddr *)&addr,sizeof(struct sockaddr))<0)
		return BINDING_ERROR;
    if(listen(socketfd,5)<0)
        return LISTENING_ERROR;
    return NOERROR;
}
Error CppSocket::UDPSetUp(){
    if (bind(socketfd, (sockaddr *)&addr, sizeof(addr)) == -1) {
        return BINDING_ERROR;
    }
    return NOERROR;
}
Error CppSocket::SetUp(){
    if(service==UDP)
        UDPSetUp();
    else if(service==TCP_Client)
        TCPClientSetUp();
    else if(service==TCP_Server)
        TCPServerSetUp();
}
CppSocket::~CppSocket(){

}

