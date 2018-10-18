#include "CppSocket.h"
#include <time.h>
#define DefultTimeOut 10000
#define byte char
#define DefultBufferSize 4096

CppSocket::CppSocket(Service _service,int _socketfd,InterAddr _addr){
    timeout=DefultTimeOut;
    localAddress=_addr;
    socketfd=_socketfd;
    service=_service;
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
    SetUp();
    isSetValue=true;

}
CppSocket::CppSocket(Service _service,InterAddr _addr){
    timeout=DefultTimeOut;
    service=_service;
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, 0);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    localAddress=_addr;
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
    SetUp();
    isSetValue=true;

}
CppSocket::CppSocket(Service _service,char* addr,int port){
    timeout=DefultTimeOut;
    service=_service;
    if(service==UDP)
        socketfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    else
        socketfd=socket(AF_INET, SOCK_STREAM, 0);
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
    localAddress.sin_family=AF_INET;
	localAddress.sin_addr.s_addr=inet_addr(addr);
	localAddress.sin_port=htons(port);
	SetUp();

    isSetValue=true;
}

Error CppSocket::sendData(TransData* data){
    if(service==UDP)
        return sendUDPData(data);
    else if(service==TCP_Client)
        return sendTCPClientData(data);
    else if(service==TCP_Server)
        return sendTCPServerData(data);
}
void CppSocket::sendData(TransData* data,ErrorHandler handler){
    Error e=sendData(data);
    handler(e);
}

Error CppSocket::recevieData(int length,TransData* data){
    if(service==UDP)
        return recevieUDPData(length,data);
    else if(service==TCP_Client)
        return recevieTCPClientData(length,data);
    else if(service==TCP_Server)
        return recevieTCPServerData(length,data);
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
                    connect(socketfd, (struct sockaddr *)&remoteAddr, sizeof(struct sockaddr_in));
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
     isConnectedValue=true;
     remoteAddress=remoteAddr;
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

CppSocket* CppSocket::acceptClient(ErrorHandler errorHandler,VerfyHandler verfyHandler){
    int client_sockfd;
    InterAddr remote_addr;
    unsigned int sin_size=sizeof(struct sockaddr_in);
    if((client_sockfd=accept(socketfd,(struct sockaddr *)&remote_addr,&sin_size))<0){
        Error e;
        if(errno==EAGAIN||errno==EWOULDBLOCK){
            e=NON_CLIENT_IN_LINE;
        }else if(errno=ECONNABORTED){
            e=END_CONNECTION;
        }else if(errno=EBADF){
            e=NOT_FD;
        }
        errorHandler(e);
        return NULL;
	}
    CppSocket* socket=new CppSocket(TCP_Client,client_sockfd,localAddress);
    socket->remoteAddress=remote_addr;
    if(verfyHandler(socket)){
        socket->isConnectedValue=true;
        return socket;
    }
    return NULL;
}

Error CppSocket::stopSocket(){
    isClosedValue=true;
}
Error CppSocket::reconnect(){// this function can only use for TCP Client
}
Error CppSocket::setTimeOut(int time){ // time in millier second
    timeout=time;
    return NOERROR;
}

bool CppSocket::isSocketSet(){
    return isSetValue;
}
bool CppSocket::isConnected(){
    return isConnectedValue;
}
bool CppSocket::isClosed(){
    return isClosedValue;
}


Error CppSocket::TCPClientSetUp(){
    return NOERROR;
}
Error CppSocket::TCPServerSetUp(){
    if(bind(socketfd,(struct sockaddr *)&localAddress,sizeof(struct sockaddr))<0)
		return BINDING_ERROR;
    if(listen(socketfd,5)<0)
        return LISTENING_ERROR;
    return NOERROR;
}
Error CppSocket::UDPSetUp(){
    if (bind(socketfd, (sockaddr *)&localAddress, sizeof(localAddress)) == -1) {
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

Error CppSocket::sendTCPClientData(TransData* data){
    char* sendingData=data->dataBuff;
    int sendingLength=data->length;
    InterAddr addr=data->address;
    clock_t init=clock();
    int readLength=0;
    while(1){
         int res=send(socketfd,getRemain(readLength,sendingLength,sendingData),sendingLength,0);
         if(res==sendingLength)
            return NOERROR;
         if(res<0){
            if(errno==EAGAIN){
                if(clock()-init>timeout)
                return TIMEOUT;
            }else{
                return SOCKET_ERROR;
            }
         }else if(res==(sendingLength-readLength)){
            return NOERROR;
         }else{
            readLength=res;
         }
    }
}
Error CppSocket::sendTCPServerData(TransData* data){
    return INCURRECT_SERVICE;
}
Error CppSocket::sendUDPData(TransData* data){
    char* sendingData=data->dataBuff;
    int sendingLength=data->length;
    InterAddr addr=data->address;
    if(sendingLength>DefultBufferSize)
        return INVALID_SIZE;
    clock_t init=clock();
    int readLength=0;
    while(1){
         int res=sendto(socketfd,sendingData,sendingLength,0,(struct sockaddr*)&addr,sizeof(addr));
         if(res==sendingLength)
            return NOERROR;
         if(res<0){
            if(errno==EAGAIN){
                if(clock()-init>timeout)
                return TIMEOUT;
            }else{
                return SOCKET_ERROR;
            }
         }else if(res==(sendingLength-readLength)){
            return NOERROR;
         }else{
            readLength=res;
         }
    }
}

Error CppSocket::recevieTCPClientData(int length,TransData* data){
    Error e=NOERROR;
    char* result=(char*)malloc(sizeof(char)*length);
    char* buff=(char*)malloc(sizeof(char)*length);
    int unreadLength=length;
    clock_t init=clock();
    while(unreadLength>0){
        int len=recv(socketfd,buff,unreadLength,0);
        if(isClosedValue){
            e=SOCKET_CLOSE;
            break;
        }
        if(len<0&&errno==ECONNRESET){
            e=CONNECTING_ERROR;
            break;
        }
        else if(len<=unreadLength){
            unreadLength=unreadLength-len;
            memcpy(result+(length-unreadLength),buff,len);
        }if(clock()-init>timeout){
            e=TIMEOUT;
            break;
        }
    }
    free(buff);
    data->address=remoteAddress;
    data->dataBuff=result;
    data->length=length-unreadLength;
    return e;
}
Error CppSocket::recevieTCPServerData(int length,TransData* data){
    return INCURRECT_SERVICE;
}
Error CppSocket::recevieUDPData(int length,TransData* data){
    byte* buff=(byte*)malloc(sizeof(char)*DefultBufferSize*2);
    InterAddr remoteAddress;
    unsigned int len=sizeof(remoteAddress);
    int n=0;
    clock_t init=clock();
    while(n<1){
        n = recvfrom(socketfd, buff, DefultBufferSize, 0, (sockaddr *)&remoteAddress, &len);

        if(n==-1&&(errno==EAGAIN||errno==EWOULDBLOCK)) {
            if(clock()-init>timeout)
                return TIMEOUT;
        }else if(n==-1){
            return SOCKET_ERROR;
        }
    }
    char* result=(char*)malloc(sizeof(char)*n);
    memcpy(result,buff,n);
    data->address=remoteAddress;
    data->dataBuff=result;
    data->length=n;
    free(buff);
    return NOERROR;
}
char* CppSocket::getRemain(int finishLength,int total,char* sdata){
    char* ddata=(char*)malloc((total-finishLength)*sizeof(char));
    memcpy(ddata,sdata+finishLength,(total-finishLength)*sizeof(char));
    return ddata;
}
CppSocket::~CppSocket(){

}

