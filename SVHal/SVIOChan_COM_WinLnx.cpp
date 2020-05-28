
#if defined (__linux__) || defined (_WIN32)

#include "SVIOChan_COM.hpp"
#include "SVTimeBase.hpp"


#if defined (__linux__) 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif 


namespace  SVLib6 {

int SVIOChan_COM::Init(const char* _comnm,unsigned _speed)
{
    //int r=0;
    mode=1;

    comnm=_comnm;
    speed=_speed;

    return 0;
}


#if defined (__linux__)

//таймаут в линукс здесь это как в tcp, до первого полученного байта
//а в виндоус это видимо вообще время на получение

int SVIOChan_COM::Open()
{
    struct termios /*oldtio,*/ newtio;
    int r=0;

    fh = open(comnm.c_str(), O_RDWR | O_NOCTTY);
    //printf("open %s r=%d\n", sport.c_str(), fh); //DEBUG
    if (fh <= 0) {
        return 1;
    }

    bzero(&newtio, sizeof (newtio));
    int f=0;
    if (speed == 9600) {
        newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        f=1;
        printf("speed: 9600\n");
    }
    if (speed == 115200) {
        newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        f=1;
        printf("speed: 115200\n");
    }
    if (speed == 230400){
        newtio.c_cflag = B230400 | CS8 | CLOCAL | CREAD;
        f=1;
        printf("speed: 230400\n");
    }
    if(f==0){
        return -1;
    }

    //newtio.c_iflag = IGNBRK | IXOFF;
    //newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    //вроде так должен работать timeout
    //newtio.c_lflag &= ~ICANON; /* Set non-canonical mode */

    //newtio.c_cc[VTIME] = timeout_sec; /* timeout */
    //newtio.c_cc[VMIN] = 0;

    r = tcsetattr(fh, TCSANOW, &newtio);
    if (r != 0) {
        return -2;
    }

    return 0;

}

void SVIOChan_COM::Close(){
    close(fh);
}

//timeout - ret=0 !
int SVIOChan_COM::Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{
    fd_set set;
    struct timeval timeout;
    int rv=0;
    int r=0;
    szio=0;

    FD_ZERO(&set);
    FD_SET(fh,&set);
    timeout.tv_sec=0;
    timeout.tv_usec=timeout_ms*1000L;
    rv=select(fh+1,&set,0,0,&timeout);
    if(rv<0) return 1;
    if(rv==0) return 0;

    r=read(fh,buf,dtsz);
    if(r<0) return 2;
    szio=r;
    ++st_cnts.rd_low;

    return 0;
}


int SVIOChan_COM::Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{
    int r=0;
    szio=0;

    r=write(fh,buf,dtsz);
    if(r<0) return 1;
    szio=r;
    ++st_cnts.wr_low;

    return 0;
}

#endif //GNU


#if defined (_WIN32)

//dcb,hcom заполн. не надо
//touts НАДО
//cport=COM1,COM2 ...
int SVIOChan_COM::ComPortOpen(const char* cport,HANDLE& hcom,DCB& dcb,COMMTIMEOUTS& touts,int BaudRate,int stopbits)
{
	int r=1;
	char* pref=(char*)"\\\\.\\";
	//char port[30];
	std::string sport;
	sport=std::string(pref)+std::string(cport);
	//strncpy(port,sport.c_str(),30);
	SetDefaultCommConfigA(sport.c_str(),NULL,0);
	hcom=CreateFileA(sport.c_str(),
    GENERIC_READ | GENERIC_WRITE,
    0,    // exclusive access
    NULL, // default security attributes
    OPEN_EXISTING,
    NULL, //FILE_FLAG_OVERLAPPED,
    NULL
    );
	if (hcom == INVALID_HANDLE_VALUE){
		return 1;
	}
	//GetCommState(hcom,&dcb);
	memset(&dcb,0,sizeof(DCB));
	dcb.DCBlength=sizeof(DCB);
	dcb.BaudRate=BaudRate;//CBR_115200;
	dcb.fBinary=1;
	dcb.fParity=0;
	dcb.ByteSize=8; //5
	dcb.StopBits=stopbits;//ONESTOPBIT;
	dcb.fDtrControl=DTR_CONTROL_DISABLE;
	dcb.fRtsControl=RTS_CONTROL_DISABLE; //RTS_CONTROL_ENABLE
	dcb.fNull=0;
	r=SetCommState(hcom,&dcb);
	if (r==0) {
		return 2;
	}
	r=SetCommTimeouts(hcom,&touts);
	if(r==0){
		return 3;
	}

	return 0;
}

void SVIOChan_COM::SetTimeouts(int rd,int wr)
	{
		COMMTIMEOUTS touts;
		memset(&touts,0,sizeof(touts));
		touts.ReadTotalTimeoutConstant=rd;
		touts.WriteTotalTimeoutConstant=wr;
		SetCommTimeouts(fh,&touts);
	}

int SVIOChan_COM::Open()
{
    DCB dcb;
    COMMTIMEOUTS touts;
    int r;

     memset(&touts,0,sizeof(touts));
     /*
     if(timeout_ms==0){
      touts.ReadTotalTimeoutConstant=timeout_sec*1000;
                    touts.WriteTotalTimeoutConstant=timeout_sec*1000;
                }
                else{
                    touts.ReadTotalTimeoutConstant=timeout_ms;
                    touts.WriteTotalTimeoutConstant=timeout_ms;
                }
                if(interval_timeout_ms!=0){
                    touts.ReadIntervalTimeout=interval_timeout_ms;
                    //printf("intervaltimeout=%d\n",touts.ReadIntervalTimeout); //SVDBG
                }
                          */

    
    r=ComPortOpen(comnm.c_str(),fh,dcb,touts,speed);
    //if(r==0) Purge();
    //SetCommMask(hcom,EV_RXCHAR);

    
    
    return r;
}

void SVIOChan_COM::Close(){
    CloseHandle(fh);
}

//timeout - ret=0 !
int SVIOChan_COM::Read(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
	{
		int r;
		szio=0;
		unsigned long sz=0;
        SetTimeouts(timeout_ms,timeout_ms);
		r=ReadFile(fh,buf,dtsz,&sz,0);
		szio=sz;
		if(r==0) return 1;
		if(szio==0) return 0;
		
		//++stat.lowrd_count;
		return 0;
	}

int SVIOChan_COM::Write(void* buf,int dtsz,unsigned& szio,unsigned timeout_ms)
{

        int r;
		szio=0;
		unsigned long sz=0;
        SetTimeouts(timeout_ms,timeout_ms);
		r=WriteFile(fh,buf,dtsz,&sz,0);
		szio=sz;
		if(r==0) return 1;
		if(szio==0) return 0;

		//++stat.lowwr_count;
		return 0;
}


#endif //WIN


//при op_tout выходит ret=0, szio содержит реальное кол-во
int SVIOChan_COM::ReadPk(void *pkbuf, unsigned pksz, unsigned &szio, unsigned pk_tout, unsigned op_tout, int prpk_flag)

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
int SVIOChan_COM::WritePk(void *pkbuf, unsigned pksz, unsigned &szio,unsigned pk_tout, unsigned op_tout, int prpk_flag)
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



}// namesp


#endif



