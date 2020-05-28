#ifndef SVIOCHAN_ETHTCP_HPP
#define SVIOCHAN_ETHTCP_HPP

#include "SVIOChan.hpp"
#include "SVIpEthAddr.hpp"
#include "../SVBufMemFix.hpp"

namespace SVLib6 {


class SVIOChan_EthTCP:public SVIOChan
{
    SVBufMemFix rbuf;
    SVIpEthAddr iaddr;        //address from init fun
    //SVIpEthAddr fraddr;       //address from last read oper
    int mode=1; //
    int rflag=0;
    int sock_created=0;
    int sock_connected=0;

    SV_SOCKET srvsock;
    int srvtout=1000;
public:
    SV_SOCKET sock;

    int InitCl(const char* ip,unsigned port);
    int InitSock(SV_SOCKET _sock);
    int InitFrSrv(SV_SOCKET _srvsock);
    //int InitSrv(const char* ip,unsigned port,unsigned rdbufsz);

    static int CrSrvSock(const char* ip,unsigned port,SV_SOCKET& _srv)
    {
        int r=0;
        SVIpEthAddr a;
        a.IPS(ip);
        a.Port(port);
        sockaddr sa;
        a.ToSockAddr(&sa);
        _srv=socket(AF_INET,SOCK_STREAM,0);
        if(_srv<=0) return 1;
        r=bind(_srv,&sa,sizeof(sockaddr));
        if(r!=0) return 2;
        r=listen(_srv,5);
        if(r!=0) return 3;
        return 0;
    }

    static int SocketStartup();

    virtual int Open();
    virtual void Close();

    //timeout - ret=0 !
    virtual int Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms);
    virtual int Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms);

    //пакетные функции
    virtual int ReadPk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag=0);
    virtual int WritePk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag=0);

};




} //namesp


#endif // SVIOCHAN_ETHTCP_HPP


