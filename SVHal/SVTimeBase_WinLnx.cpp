#include "SVTimeBase.hpp"

namespace SVLib6 {

#ifdef _WIN32

int winsleep2(long ms){
    Sleep(ms);
    return 0;
}

void SysTime(sv_time2& t2)
{
        SYSTEMTIME tm;
        GetSystemTime(&tm);
        t2.year=tm.wYear;
        t2.month=(unsigned char)tm.wMonth;
        t2.day=(unsigned char)tm.wDay;
        t2.hour=(unsigned char)tm.wHour;
        t2.minute=(unsigned char)tm.wMinute;
        t2.second=(unsigned char)tm.wSecond;
        t2.milliseconds=tm.wMilliseconds;
    }

//----------

unsigned long long SVGetTimeInterval::_GetIntervalMs()
{
    int_perf=ipc2-ipc1;
    int_ms=int_perf/(pf.QuadPart/1000);
    return int_ms;
}

SVGetTimeInterval::SVGetTimeInterval(){
    pf.QuadPart=0;
    ipc1=0;
    QueryPerformanceFrequency(&pf);
}

void SVGetTimeInterval::GetTime1(){QueryPerformanceCounter(&pc1);ipc1=pc1.QuadPart;int_ms=0;}

unsigned long long SVGetTimeInterval::GetTime2(){
    QueryPerformanceCounter(&pc2);
    ipc2=pc2.QuadPart;
    return _GetIntervalMs();

}

unsigned long long SVGetTimeInterval::GetIntervalMs(){return int_ms;}

bool SVGetTimeInterval::IsFirstTime(){if(ipc1==0) return true; return false;}


#endif //win

//-------------------------------------------------------

#if defined (__linux__)

int winsleep2(long miliseconds)
{
    struct timespec req, rem;

    if(miliseconds > 999)
   {
        req.tv_sec = (int)(miliseconds / 1000);       /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   }
   else
   {
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }

   return nanosleep(&req , &rem);
}

void SysTime(sv_time2& t2)
{
    timeval tv;
    gettimeofday(&tv,0);
    struct tm ct;
    gmtime_r(&tv.tv_sec,&ct);

    t2.year=ct.tm_year+1900;
    t2.month=(__uint8)(ct.tm_mon+1);
    t2.day=(__uint8)ct.tm_mday;
    t2.hour=(__uint8)ct.tm_hour;
    t2.minute=(__uint8)ct.tm_min;
    t2.second=(__uint8)ct.tm_sec;
    t2.milliseconds=tv.tv_usec/1000L;

    //printf("ct: year=%d mon=%d\n",ct.tm_year,ct.tm_mon);
    //printf("svtime SysTime=%s\n",ToString());
}

//------------


SVGetTimeInterval::SVGetTimeInterval(){t1ms=0;}


__uint64 SVGetTimeInterval::GetTimeOfDayMS()
{
    //struct timeval t;
    //gettimeofday( &t, NULL );               //thread safe !
    //return t.tv_sec*1000+t.tv_usec/1000;

    __uint64 tk=0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts );
    tk  = ts.tv_nsec / 1000000;
    tk += ts.tv_sec * 1000;
    return tk;
}

void SVGetTimeInterval::GetTime1()
{
    t1ms=GetTimeOfDayMS();
    diff=0;
}



__uint64 SVGetTimeInterval::GetTime2()
{
    t2ms=GetTimeOfDayMS();
    return (diff=t2ms-t1ms);
}


__uint64 SVGetTimeInterval::GetIntervalMs(){return GetTime2();}

bool SVGetTimeInterval::IsFirstTime(){if(t1ms==0) return true; return false;}




#endif //lin



} //namesp
