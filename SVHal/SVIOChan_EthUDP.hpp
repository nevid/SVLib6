#ifndef SVIOCHAN_ETHUDP_HPP
#define SVIOCHAN_ETHUDP_HPP

#include "SVIOChan.hpp"
#include "SVIpEthAddr.hpp"
#include "../SVBufMemFix.hpp"

namespace SVLib6 {


class SVIOChan_EthUDP:public SVIOChan
{
    SVBufMemFix rbuf;
    SVBufMemFix wbuf;
    SVIpEthAddr iaddr;        //address from init fun
    SVIpEthAddr fraddr;       //address from last read oper
    int mode=1; //1-cl 2-srv
    int rflag=0;
public:
    SV_SOCKET sock;

    int InitCl(const char* ip,unsigned port,unsigned rdbufsz);
    int InitSrv(const char* ip,unsigned port,unsigned rdbufsz);

    virtual int Open();
    virtual void Close();

    //timeout - ret=0 !
    virtual int Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms);
    virtual int Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms);

    //пакетные функции
    virtual int ReadPk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag=0);
    //чтоьы пакет udp отправился нужно prpk_flag=1
    virtual int WritePk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag=0);

};




} //namesp


#endif // SVIOCHAN_ETHUDP_HPP


