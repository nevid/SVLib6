
#include "SVBufMemFix.hpp"

#include <memory.h>

#include "SVLib6Types.h"

namespace SVLib6{

SVBufMemFix::SVBufMemFix(){buf=0;bufsize=0;objsize=0;nodelflag=0;curreadpos=0;}

SVBufMemFix::~SVBufMemFix(){
    if(nodelflag==1) return;
    if(buf!=0) operator delete (buf);
}

int SVBufMemFix::SetMemSize(unsigned sz)
{
    if(buf!=0) return 1;
    bufsize=0;
    buf=operator new (sz);
    if(buf==0) return 1;
    bufsize=sz;
    return 0;
}

int SVBufMemFix::ReserveObj(unsigned size)
{
    if(size>bufsize)
    {
        if(buf==0) return -1;
        if(buf!=0) return 1;
    }
    objsize=size;
    return 0;
}

int SVBufMemFix::CopyFrom(void *ptr, unsigned size)
{
    int r;
    r=ReserveObj(size);
    if(r!=0) return r;
    memcpy(buf,ptr,size);
    return 0;
}

int SVBufMemFix::CopyFromAdd(void *ptr, unsigned size)
{
    int r;
    unsigned allsz=objsize+size;
    unsigned off=objsize;
    r=ReserveObj(allsz);
    if(r!=0) return r;
    memcpy((char*)buf+off,ptr,size);
    return 0;
}

void *SVBufMemFix::ReserveObjAdd(unsigned size)
{
    int r;
    unsigned allsz=objsize+size;
    unsigned off=objsize;
    r=ReserveObj(allsz);
    if(r!=0) return 0;
    return (char*)buf+off;
}

void *SVBufMemFix::SizeForEnd(unsigned &size)
{
    size=bufsize-objsize;
    unsigned off=objsize;
    return (char*)buf+off;
}



void SVBufMemFix::SetFromPtr(void *_buf, unsigned bufsz)
{
    buf=_buf;
    bufsize=bufsz;
    objsize=bufsz;
    nodelflag=1;    //не будем освобождать
}

void *SVBufMemFix::ReadPtr(unsigned size){
    if(curreadpos+size>objsize)	return 0;
    void* p=((__int8*)buf)+curreadpos;
    curreadpos+=size;
    return p;
}

void *SVBufMemFix::ReadPtr2(unsigned size, unsigned &ret_size){
    if(curreadpos>=objsize){return 0;}
    void* p=((__int8*)buf)+curreadpos;
    if(curreadpos+size>objsize)	{ret_size=objsize-curreadpos;}
    else ret_size=size;
    curreadpos+=size;
    return p;
}

void *SVBufMemFix::GetPtr(unsigned offset, unsigned qsize){
    if(offset+qsize>objsize) return 0;
    return (__int8*)buf+offset;
}


} //namespace
