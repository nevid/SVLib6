#ifndef SVIPETHADDR_HPP
#define SVIPETHADDR_HPP

#include "../SVLib6Types.h"

namespace SVLib6 {

class SVIpEthAddr
{
public:

    __uint32 ip;			//в сетевой
    __uint16 port;			//в сетевой

    //заполняется из sockaddr
    //sockaddr
    void FromSockAddr(void* sockaddr);
    //заполн. порт из хост послед. _port
    void Port(__uint16 _port);//{port=htons(_port);}

    //заполн. ip из строки
    void IPS(const char* _ip);//{ip=inet_addr(_ip);}

    //возвр. ip как строку
    const char* IPS();

    //выводит в sockaddr
    void ToSockAddr(void* sockaddr);

    //возвр. ip в хост послед.
    __uint32 IP();   // {return ntohl(ip);}
    //возвр. порт в хост послед.
    __uint16 Port();  // {return ntohs(port);}

};


int SVLibSocketStartup();


} //nmaesp

#endif // SVIPETHADDR_HPP
