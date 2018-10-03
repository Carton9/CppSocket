#include "CppSocket.h"

CppSocket::CppSocket(Service _service,int _socketfd){
    socketfd=_socketfd;
    service=service;
}
CppSocket::CppSocket(Service _service,InterAddr addr){
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, 0);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    service=service;
}
CppSocket::CppSocket(Service _service,char* addr,int port){
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, 0);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    service=service;
}

Error CppSocket::sendData(TransData* data){
    return NOERROR;
}
void CppSocket::sendData(TransData* data,ErrorHandler handler){
    Error e=sendData(data);
    handler(e);
}

Error CppSocket::recevieData(int length,TransData* data){
}
void CppSocket::recevieData(int length,TransData* data,ErrorHandler handler){
    Error e=recevieData(length,data);
    handler(e);
}

Error CppSocket::connectTo(InterAddr addr){// this function can only use for TCP Client

}
void CppSocket::connectTo(InterAddr addr,ErrorHandler errorHandler,VerfyHandler verfyHandler){// this function can only use for TCP Client
    Error e=connectTo(addr,data);
    handler(e);
}

CppSocket* CppSocket::accept(ErrorHandler errorHandler,VerfyHandler verfyHandler){
}

Error CppSocket::stopSocket(){
}
Error CppSocket::reconnect(){// this function can only use for TCP Client
}
Error CppSocket::setTimeOut(int time){ // time in millier second
}

bool CppSocket::isSocketSet(){
}
bool CppSocket::isConnected(){
}
bool CppSocket::isClosed(){
}

CppSocket::~CppSocket(){
}

