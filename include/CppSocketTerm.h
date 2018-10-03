#ifndef CPPSOCKETTERM_H_INCLUDED
#define CPPSOCKETTERM_H_INCLUDED


enum Service{TCP_Client,TCP_Server,UDP}; // this enum is use to indecate the transmisson protocol is using
enum Error{NOERROR,INCURRECT_SERVICE,TIMEOUT,SOCKET_ERROR,CONNECTING_ERROR,ADDRESS_IN_USE};   // this enum is use to show the socket error in the transmission progress

typedef struct sockaddr_in InterAddr;   // define the socket structure to InterAddr
typedef struct _receviedData{char* data;int length;InterAddr address;} TransData; // define a structure to store reviced data

#endif // CPPSOCKETTERM_H_INCLUDED
