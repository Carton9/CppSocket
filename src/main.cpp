#include "CppSocket.h"
int main(){
    CppSocket s1(UDP,"127.0.0.1",8001);
    CppSocket s2(UDP,"127.0.0.1",8002);
    TransData td;
    td.address.sin_family=AF_INET;
    td.address.sin_addr.s_addr=inet_addr("127.0.0.1");
    td.address.sin_port = htons(8002);
    td.dataBuff="good";
    td.length=4;
    s1.sendData(&td);
    TransData td2;
    s2.recevieData(4,&td2);
    printf("%s",td.dataBuff);
}
