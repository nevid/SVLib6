/*
* SV Library 6
* Sergey Voskresensky (C)
* http://svsoft.svhome.org
* nevidprogr@gmail.com
*/


#include "SVBufMem2.hpp"

#include <new>

namespace SVLib6 {


SVBufMem2::SVBufMem2(){buf=0;granul=1024;bufsize=0;objsize=0;
                       //ResetReadPtr();
                      }

SVBufMem2::~SVBufMem2(){if(buf!=0) operator delete (buf);}

void SVBufMem2::SetGranul(unsigned v){granul=v;}

void SVBufMem2::SetMaxSize(unsigned _sz)
{
    maxsize=_sz;
}

int SVBufMem2::ReserveObj(unsigned size){
    if(size>maxsize) return 2;
    if(size>bufsize){
        void* buf1;
        int sz=granul*(size/granul)+granul;
        buf1=operator new (sz,std::nothrow);
        if(buf1==0) return 1;
#ifdef _SVDEBUG_SVBufMem
        printf("new:  ptr=%x size=%d\n",(int)buf1,sz); //debug
#endif
        if(buf!=0){memcpy(buf1,buf,bufsize);operator delete (buf);
#ifdef _SVDEBUG_SVBufMem
            printf("del:  ptr=%x size=%d\n",(int)buf,sz); //debug
#endif
        }
        buf=buf1;
        bufsize=sz;
    }
    objsize=size;
    return 0;
}

void SVBufMem2::CopyFrom(void *ptr, unsigned size)
{
    if(objsize>maxsize) return;
    ReserveObj(size);
    memcpy(buf,ptr,size);
}

void SVBufMem2::CopyFromAdd(void *ptr, unsigned size)
{
    if(objsize>maxsize) return;
    unsigned allsz=objsize+size;
    unsigned off=objsize;
    ReserveObj(allsz);
    memcpy((char*)buf+off,ptr,size);
}

void* SVBufMem2::ReserveObjAdd(unsigned size)
{
    if(objsize>maxsize) return 0;
    unsigned allsz=objsize+size;
    unsigned off=objsize;
    ReserveObj(allsz);
    return (char*)buf+off;
}

void *SVBufMem2::Ptr(){return buf;}

unsigned SVBufMem2::Size(){return objsize;}

int SVBufMem2::CopyToBegin(unsigned _pos)
{
    if(_pos>=objsize) {/*ResetReadPtr();*/ReserveObj(0);return 1;}
    unsigned t=objsize-_pos;
    //printf("cptobg objsz=%d curreadpos=%d sz=%d\n",objsize,curreadpos,t);
    //memcpy(buf,(char*)buf+curreadpos,t); //нельзя использовать на перекрывающихся блоках !
    memmove(buf,(char*)buf+_pos,t);
    ReserveObj(t);
    //ResetReadPtr();
    return 0;
}

/*

void SVBufMem2::ResetReadPtr(){curreadpos=0;}

void *SVBufMem2::ReadPtr(unsigned size){
    if(curreadpos+size>objsize)	return 0;
    void* p=((__int8*)buf)+curreadpos;
    curreadpos+=size;
    return p;
}

void *SVBufMem2::ReadPtrA(unsigned size){
    if(curreadpos+size>objsize)	{curreadpos+=size;return 0;}
    void* p=((__int8*)buf)+curreadpos;
    curreadpos+=size;
    return p;
}

void *SVBufMem2::ReadPtr2(unsigned size, unsigned &ret_size){
    if(curreadpos>=objsize){ret_size=0;return 0;}
    void* p=((__int8*)buf)+curreadpos;
    if(curreadpos+size>objsize)	{ret_size=objsize-curreadpos;}
    else ret_size=size;
    curreadpos+=size;
    return p;
}

void *SVBufMem2::GetPtr(unsigned offset, unsigned qsize){
    if(offset+qsize>objsize) return 0;
    return (__int8*)buf+offset;
}

void *SVBufMem2::ReadPtr3(unsigned &ret_size){
    //printf("cur=%d\n",curreadpos);
    if(curreadpos>=objsize){return 0;}
    void* p=((__int8*)buf)+curreadpos;
    ret_size=objsize-curreadpos;
    return p;
}
*/


}
