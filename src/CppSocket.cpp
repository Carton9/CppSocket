#include "CppSocket.h"

CppSocket::CppSocket(Service service,int _socketfd){
}
CppSocket::CppSocket(Service service,InterAddr addr){
}
CppSocket::CppSocket(Service service,char* addr,int port){
}

Error CppSocket::sendData(TransData* data){
    return NOERROR;
}
void CppSocket::sendData(TransData* data,ErrorHandler handler){
}

Error CppSocket::recevieData(int length,TransData* data){
}
void CppSocket::recevieData(int length,TransData* data,ErrorHandler handler){
}

Error CppSocket::connectTo(InterAddr addr){// this function can only use for TCP Client
}
void CppSocket::connectTo(InterAddr addr,ErrorHandler errorHandler,VerfyHandler verfyHandler){// this function can only use for TCP Client
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

