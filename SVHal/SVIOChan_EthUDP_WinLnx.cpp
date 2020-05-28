
#if defined (__linux__) || defined (_WIN32)

#include "SVIOChan_EthUDP.hpp"
#include "SVTimeBase.hpp"

#if defined (__linux__)

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2ipdef.h>
#endif 


namespace  SVLib6 {

int SVIOChan_EthUDP::InitCl(const char *ip, unsigned port, unsigned rdbufsz)
{
    int r=0;
    mode=1;
    //r=usock.CreateTCPUDPSocket();
    //usock.CreateTCPAddress((char*)ip,(int)port);
    //rbuf.SetMemSize(rdbufsz);
    //return r;

   sock=socket(AF_INET,SOCK_DGRAM,0);
   if(sock==0) return 1;

   iaddr.IPS(ip);
   iaddr.Port(port);

   r=rbuf.SetMemSize(rdbufsz);
   if(r!=0) return -1;
   r=wbuf.SetMemSize(rdbufsz);
   if(r!=0) return -1;

   return 0;
}

int SVIOChan_EthUDP::InitSrv(const char *ip, unsigned port, unsigned rdbufsz)
{
    int r=0;
    mode=2;

    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock==0) return 1;

    r=rbuf.SetMemSize(rdbufsz);
    if(r!=0) return -1;
    r=wbuf.SetMemSize(rdbufsz);
    if(r!=0) return -1;

    iaddr.IPS(ip);
    iaddr.Port(port);
    sockaddr addr;
    iaddr.ToSockAddr(&addr);
    r=bind(sock,&addr,sizeof(sockaddr_in));
    if(r!=0) return 2;

    return 0;
}

int SVIOChan_EthUDP::Open(){return 0;}
void SVIOChan_EthUDP::Close(){}

//timeout - ret=0 !
int SVIOChan_EthUDP::Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{
    int r=0;
    szio=0;

    timeval ttimeout;
    ttimeout.tv_usec=timeout_ms*1000L;ttimeout.tv_sec=0;

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sock,&fdset);
    r=select(sock+1,&fdset,0,0,&ttimeout);
    if((r==0)||(FD_ISSET(sock,&fdset)==false)) {return 0;}
    sockaddr from;
    //socklen_t fromlen=sizeof(from);
    #ifdef __WIN32
    int fromlen=sizeof(from);
    #else
    unsigned fromlen=sizeof(from);
    #endif
    r=recvfrom(sock,(char*)buf,dtsz,0,&from,&fromlen);
    if(r<0){
        return 1;
    }
    else{
        szio=r;
        fraddr.FromSockAddr(&from);
        if(szio>0) rflag=1;
        return 0;
    }

    return -1;
}

int SVIOChan_EthUDP::Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{

    int r=0;
    szio=0;
    sockaddr addr;

    if(dtsz<=0) return 0;

    if(mode==1){
        iaddr.ToSockAddr(&addr);
    }
    //в сервер моде посылаем туда, откуда последний раз приняли
    if(mode==2){
        if(rflag==0) return 0;    //не было приема
        fraddr.ToSockAddr(&addr);
    }

    r=sendto(sock,(char*)buf,dtsz,0,&addr,sizeof(addr));
    if(r<0){
        return 1;
    }
    else{
        szio=r;
        return 0;
    }

    return -1;

}

//при op_tout выходит ret=0, szio содержит реальное кол-во
int SVIOChan_EthUDP::ReadPk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag)
{
    int r;
    unsigned wr=0;
    szio=0;

    SVGetTimeInterval tm;
    tm.GetTime1();

    //непосредствено принмиаем пакет только первый раз
    if(prpk_flag==1){
        while(1){
            rbuf.ResetReadPtr();
            r=Read(rbuf.Ptr(),rbuf.BufSize(),wr,op_tout);
            if(r!=0) return 1;
            if(wr==0) {++st_cnts.rd_lowtout;}
            if(wr>0){
                ++st_cnts.rd_low;
                rbuf.ReserveObj(wr);
                break;
            }
            if(tm.GetTime2()>(unsigned)pk_tout) {++st_cnts.rd_bltout;return 0;}
        }
    }
    //затем читаем из буфера
    void* p=rbuf.ReadPtr(pksz);
    if(p==0) {return 1;}
    szio=pksz;
    memcpy(pkbuf,p,pksz);

    return 0;

}

//
int SVIOChan_EthUDP::WritePk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag)
{
    int r=0;
    szio=0;
    unsigned wr=0;

    wbuf.CopyFromAdd(pkbuf,pksz);
    szio=pksz;

    if(prpk_flag==1){
        //r=Write(pkbuf,pksz,wr,op_tout);
        r=Write(wbuf.Ptr(),wbuf.Size(),wr,op_tout);
        wbuf.ReserveObj(0);
        if(r!=0) {szio=0;return 1;}
        if(wr==0) {szio=0;++st_cnts.wr_lowtout;return 0;}
        else ++st_cnts.wr_low;
    }


    return 0;
}



}// namesp


#endif



