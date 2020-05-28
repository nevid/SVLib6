
#if defined (__linux__) || defined (_WIN32)

#include "SVIOChan_EthTCP.hpp"
#include "SVTimeBase.hpp"


#if defined (__linux__)

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2ipdef.h>
#endif 


namespace  SVLib6 {

int SVIOChan_EthTCP::InitCl(const char *ip, unsigned port)
{
    //int r=0;
    mode=1;

    iaddr.IPS(ip);
    iaddr.Port(port);

    return 0;
}

int SVIOChan_EthTCP::InitSock(SV_SOCKET _sock)
{
    mode=2;
    sock=_sock;
    return 0;
}

int SVIOChan_EthTCP::InitFrSrv(SV_SOCKET _srvsock)
{
    mode=3;
    srvsock=_srvsock;
    return 0;
}

/*
int SVIOChan_EthTCP::InitSrv(const char *ip, unsigned port, unsigned rdbufsz)
{
    int r=0;
    mode=2;

    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock==0) return 1;

    r=rbuf.SetMemSize(rdbufsz);
    if(r!=0) return -1;

    iaddr.IPS(ip);
    iaddr.Port(port);
    sockaddr addr;
    iaddr.ToSockAddr(&addr);
    r=bind(sock,&addr,sizeof(sockaddr_in));
    if(r!=0) return 2;

    return 0;
}
*/

int SVIOChan_EthTCP::Open()
{
    if(mode==3){
        int r,rv=0;
        timeval tm;
        tm.tv_sec = 0;
        tm.tv_usec = srvtout*1000L;
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(srvsock,&fdset);
        r=select(srvsock+1,&fdset,0,0,&tm);
        if((r==0)||(FD_ISSET(srvsock,&fdset)==false)) {return 1;}
        sock=accept(srvsock,NULL,NULL);
        if(sock>0){
              sock_created=1;
              sock_connected=1;
              return 0;
        }
        return 2;
    }

    if(sock_created==0){
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==0) return -1;
        sock_created=1;
    }
    if(sock_connected==1){
        return 0;
    }
    int r=0;
    sockaddr sa;
    iaddr.ToSockAddr(&sa);
    r=connect(sock,&sa,sizeof(sockaddr));
    if(r!=0) return 1;
    sock_connected=1;
    return 0;
}
void SVIOChan_EthTCP::Close()
{
    #if defined (__GNUC__) && ! defined (_WIN32)
    shutdown(sock,SHUT_RDWR);
    close(sock);
    #endif
    #ifdef _WIN32
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    #endif
    
    sock_connected=0;
    sock_created=0;
}

//timeout - ret=0 !
int SVIOChan_EthTCP::Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{
    timeval ttimeout;
    ttimeout.tv_usec=timeout_ms*1000L;ttimeout.tv_sec=0;
    fd_set fdset;
    int r=0;
    szio=0;
    FD_ZERO(&fdset);
    FD_SET(sock,&fdset);

    r=select(sock+1,&fdset,0,0,&ttimeout);

    if((r==0)||(FD_ISSET(sock,&fdset)==false)) {
        //timeout
        return 0;
    }
    r = recv(sock, (char*)buf, dtsz, 0);
    if (r == SV_SOCKET_ERROR){
        return 1;
    }

    szio=r;

    //++stat.lowrd_count;
    return 0;
}

int SVIOChan_EthTCP::Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{
    timeval ttimeout;
    fd_set fdset;
    ttimeout.tv_usec=timeout_ms*1000L;ttimeout.tv_sec=0;
    int r=0;
    szio=0;
    FD_ZERO(&fdset);
    FD_SET(sock,&fdset);

    r=select(sock+1,0,&fdset,0,&ttimeout);

    if((r==0)||(FD_ISSET(sock,&fdset)==false)) {
        return 0;
    }

    r = send(sock, (char*)buf, dtsz, 0);
    if (r == SV_SOCKET_ERROR){
        return 1;
    }
    //if(r==0) {rv=2;return rv;}
    szio=r;

    //++stat.lowwr_count;
    return 0;
}

//при op_tout выходит ret=0, szio содержит реальное кол-во
int SVIOChan_EthTCP::ReadPk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag)

{
    int r=0;
    SVGetTimeInterval tm;
    tm.GetTime1();

    unsigned recpos=0,recsz;
    unsigned wr=0;
    unsigned size=pksz;
    szio=0;
    while(recpos<size)
    {
        recsz=size-recpos;

        r=Read((char*)pkbuf+recpos,recsz,wr,op_tout);
        if(r!=0) return 1;
        if(wr==0) ++st_cnts.rd_lowtout;
        else ++st_cnts.rd_low;

        //printf("low_read: recsz=%d, wr=%d r=%d\n",recsz,wr,r);  //SVDBG
        szio+=wr;
        recpos+=wr;

        if(tm.GetTime2()>(unsigned)pk_tout) {++st_cnts.rd_bltout;return 0;}
    }
    ++st_cnts.rd_bl;
    return 0;
}


//
int SVIOChan_EthTCP::WritePk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag)
{
    //unsigned szio,sz;
    int r=0;
    SVGetTimeInterval tm;
    tm.GetTime1();

    unsigned recpos=0,recsz;
    unsigned wr=0;
    unsigned size=pksz;
    szio=0;
    while(recpos<size)
    {
        recsz=size-recpos;
        r=Write((char*)pkbuf+recpos,recsz,wr,op_tout);
        if(r!=0) return 1;
        //printf("low_write: recsz=%d, wr=%d r=%d\n",recsz,wr,r);  //SVDBG
        szio+=wr;
        recpos+=wr;

        if(tm.GetTime2()>(unsigned)pk_tout) {++st_cnts.wr_bltout;return 0;}
    }

    ++st_cnts.wr_bl;
    return 0;
}

int SVIOChan_EthTCP::SocketStartup()
{
    #ifdef _WIN32
        WORD vers = MAKEWORD(2, 2);
        WSADATA wdata;
        return WSAStartup(vers, &wdata);
    #else
        return 1;
    #endif
}


}// namesp


#endif



