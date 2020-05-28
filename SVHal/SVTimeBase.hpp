#ifndef SVTIMEBASE_HPP
#define SVTIMEBASE_HPP

#include "../SVLib6Types.h"


//linux в опции линкера возможно потребуется добавить: -lrt

#if defined (__linux__)
#include <sys/time.h>
#endif
#if defined (_WIN32)
#include <windows.h>
#endif



namespace SVLib6 {

int winsleep2(long miliseconds);
void SysTime(sv_time2& t2);

//---------


#if defined (__linux__)

class SVGetTimeInterval
{
        __uint64 t1ms;
    __uint64 t2ms;

        __uint64 diff;

    int r;

public:
        SVGetTimeInterval();

        __uint64 GetTimeOfDayMS();

        void GetTime1();
    //при выводе использовать формат: %lld
        __uint64 GetTime2();

         __uint64 Diff(){return diff;}
         __uint64 GetIntervalMs();
        //возращ. true, если предыдущ. значения нет и соответств. интервал нельзя измерить
        bool IsFirstTime();

};

#endif //linux

//--------------------------------------------------

#ifdef _WIN32


class SVGetTimeInterval
{
    LARGE_INTEGER pf,pc1,pc2;
    unsigned __int64 ipc1,ipc2;

    unsigned __int64 int_ms;
    unsigned __int64 int_perf;

     unsigned __int64 _GetIntervalMs();

public:

    SVGetTimeInterval();

     void GetTime1();
     unsigned __int64  GetTime2();
     unsigned __int64  GetIntervalMs();
     //возращ. true, если предыдущ. значения нет и соответств. интервал нельзя измерить
     bool IsFirstTime();

};

#endif  //win



}  //namesp





#endif // SVTIMEBASE_HPP
