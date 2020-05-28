#ifndef SVIOCHAN_COM_HPP
#define SVIOCHAN_COM_HPP

#include "SVIOChan.hpp"
#include "SVIpEthAddr.hpp"
#include "../SVBufMemFix.hpp"

#include <string>

namespace SVLib6 {


class SVIOChan_COM:public SVIOChan
{
    int mode=1; //
    int speed;
    std::string comnm;
#ifdef __linux__
    int fh;
#endif
    
#ifdef _WIN32
    int ComPortOpen(const char* cport,HANDLE& hcom,DCB& dcb,COMMTIMEOUTS& touts,int BaudRate,int stopbits=ONESTOPBIT);
    void SetTimeouts(int rd,int wr);
    HANDLE fh;
#endif    

public:

    int Init(const char* _comnm,unsigned _speed = 115200);

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


#endif // SVIOCHAN_COM_HPP


