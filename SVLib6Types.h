/*
* SV Library 6
* Sergey Voskresensky (C)
* http://svsoft.svhome.org
* nevidprogr@gmail.com
*/

#ifndef __NO_SVLIBTYPES

#ifndef SVLIB6TYPES_H
#define SVLIB6TYPES_H

//------------ GNU ----------------------------------
#if defined (__linux__)  

#include <sys/types.h>
#include <time.h>
#include <memory.h>

typedef u_int32_t __uint32;
typedef u_int16_t __uint16;
typedef u_int8_t __uint8 ;
typedef int32_t __int32;
typedef int16_t __int16;
typedef int8_t __int8;
typedef int64_t __int64;
typedef u_int64_t __uint64;


typedef int SV_HANDLE;
typedef int SV_SOCKET;
const int SV_SOCKET_ERROR = -1;

typedef __uint32			DWORD;
typedef __uint16			WORD;
typedef __uint8			BYTE;
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))


//#define winsleep(tms) svsleep(tms)

#endif

//---------------------------- WIN 32 ---------------------------

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>

typedef unsigned __int64 __uint64; 
typedef unsigned __int32 __uint32; 
typedef unsigned __int16 __uint16; 
typedef unsigned __int8 __uint8; 
typedef unsigned int __uint; 

typedef HANDLE SV_HANDLE;
typedef SOCKET SV_SOCKET;
const int SV_SOCKET_ERROR = -1;

//#define winsleep(tms) Sleep((tms))


#endif  //WIN



//----------------------------- ALL

#define SVNULLSTRUCT memset(this,0,sizeof(*this))

//идентичны sv_time и sv_dtime, вводятся как простые типы
#pragma  pack(1)
struct sv_time2
{
	__uint16 year;
	__uint8 month;
	__uint8 day;
	__uint8 hour;
	__uint8 minute;
	__uint8 second;
	__uint16 milliseconds;
};
#pragma pack()

struct sv_dtime2
{
    __uint64 val;
};


//пример:
//тело: TestSVObjPtr(int a,SVObjPtr& dt) {}
//вызов: TestSVObjPtr(10,SVObjPtr(&a,sizeof(a)));
struct SVObjPtr
{
	void* dt;
	unsigned dtsz;

	SVObjPtr(){dt=0;dtsz=0;}
	SVObjPtr(void* _dt,unsigned _dtsz):dt(_dt),dtsz(_dtsz){}
	bool IsInit(){if(dt==0) return false; else return true;}
};

#endif  //SVTYPESLNX_H

#endif  //__NO_SVLIBTYPES
