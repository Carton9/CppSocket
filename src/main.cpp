#include "CppSocket.h"
int main(){
    CppSocket s1(UDP,"127.0.0.1",8053);
    CppSocket s2(UDP,"127.0.0.1",8054);
    TransData td;
    memset(&td,0,sizeof(td));
    td.address.sin_family=AF_INET;
    td.address.sin_addr.s_addr=inet_addr("127.0.0.1");
    td.address.sin_port = htons(8054);
    td.dataBuff="good";
    td.length=4;
    s1.sendData(&td);

    TransData* td2=(TransData*)malloc(sizeof(TransData));
    printf("%d\n",s2.recevieData(4,td2));
     printf("%s\n",td2->dataBuff);
    s2.stopSocket();
}
