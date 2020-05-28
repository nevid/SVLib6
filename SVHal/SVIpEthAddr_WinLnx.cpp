
#if defined (__linux__) || defined (_WIN32)

#include "SVIpEthAddr.hpp"

//#ifdef _WIN32

#if defined (__linux__) 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif 

#ifdef _WIN32
#include <winsock2.h>
#include <ws2ipdef.h>
#endif 

namespace SVLib6 {

void SVIpEthAddr::FromSockAddr(void* paddr){
    sockaddr* sa=(sockaddr*)paddr;
    sockaddr& addr=*sa;
    sockaddr_in& scaddr=(sockaddr_in&)addr;
    ip=scaddr.sin_addr.s_addr;
    port=scaddr.sin_port;
}

void SVIpEthAddr::Port(__uint16 _port)
{
    port=htons(_port);
}

void SVIpEthAddr::IPS(const char *_ip)
{
    ip=inet_addr(_ip);
}

const char *SVIpEthAddr::IPS(){
    static char sip[16];
    in_addr addr;
    addr.s_addr=ip;
    char* s=inet_ntoa(addr);
    strcpy(sip,s);
    return sip;
}

void SVIpEthAddr::ToSockAddr(void* paddr){
    sockaddr_in saddr;
    in_addr addr;
    addr.s_addr=ip;
    saddr.sin_family=AF_INET;
    saddr.sin_port=port;//htons(port);
    saddr.sin_addr=addr;
    memcpy(paddr,&saddr,sizeof(sockaddr_in));
}

__uint32 SVIpEthAddr::IP()
{
    return ntohl(ip);
}

__uint16 SVIpEthAddr::Port()
{
    return ntohs(port);
}



int SVLibSocketStartup()
{
	#if defined (__linux__) 
	return 0;
	#endif
	#ifdef _WIN32
	WORD vers = MAKEWORD(2, 2);
	WSADATA wdata;
	return WSAStartup(vers, &wdata);
	#endif
}




} //namespace


#endif







