/*
* SV Library 6
* Sergey Voskresensky (C)
* http://svsoft.svhome.org
* nevidprogr@gmail.com
*/

#ifndef SVIOCHAN_HPP
#define SVIOCHAN_HPP


namespace  SVLib6 {


//любой канал обмена
class SVIOChan
{
public:

    struct _cnts{
        unsigned rd_lowtout=0;
        unsigned rd_low=0;
        unsigned rd_bltout=0;
        unsigned rd_bl=0;

        unsigned wr_lowtout=0;
        unsigned wr_low=0;
        unsigned wr_bltout=0;
        unsigned wr_bl=0;
    }st_cnts;

    //unsigned sysid;

    //ret: 0=OK  
    virtual int Open() = 0;
    virtual void Close() = 0;
    //dtsz-кол-во байт, которое хотим принять/отправить
    //ret: 0-OK, !=0-error, szio=read/write bytes (0-timeout)
    //     (timeout не считается ошибкой, ret=0)
    virtual int Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms) = 0;
    virtual int Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms) = 0;

    //prpk_flag - 1=начало пакета протокола (использ в UDP)
    //при op_tout выходит ret=0, szio содержит реальное кол-во
    virtual int ReadPk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag) = 0;
     //prpk_flag - 1=конец пакета протокола (использ в UDP)
    virtual int WritePk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag) = 0;

};

} //namespace

#endif // SVIOLIB3_HPP
